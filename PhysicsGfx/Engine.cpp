#include "Engine.h"

#include <vector>
#include <assert.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "AnimationManager.h"
#include "Texture.h"
#include "GameObject.h"
#include "Model.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Material.h"
#include "CheckGLError.h"

#include "vertex_types.h"

#include "Animation.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "GL.h"

namespace gdp
{
    // Initialize a Physics Factory
    physics::iPhysicsFactory* g_PhysicsFactory;
    physics::iPhysicsWorld* g_PhysicsWorld;

    // Managers
    AnimationManager g_AnimationManager;

    Assimp::Importer g_Importer;

    bool RenderObjects;
    bool RenderPhysicsDebug;

    void Update();
    void Render();

    float g_PrevTime;
    float g_CurrTime;

    struct SimpleShader {
        GLuint ProjectionMatrix;
        GLuint ViewMatrix;
        GLuint ModelMatrix;
        GLuint RotationMatrix;

        GLuint Texture00;
        GLuint Color;
    } gSimpleShader;

    struct BoneShader {
        GLuint ProjectionMatrix;
        GLuint ViewMatrix;
        GLuint ModelMatrix;
        GLuint RotationMatrix;
        GLuint BoneMatrices[4];
        GLuint BoneRotationMatrices[4];

        GLuint Texture00;
        GLuint Color;
    } gBoneShader;

    struct LineShader {
        GLuint ProjectionMatrix;
        GLuint ViewMatrix;
    } gLineShader;

    unsigned int gBoneShaderId;
    unsigned int gSimpleShaderId;
    unsigned int gLineShaderId;

    glm::mat4 gProjectionMatrix;
    glm::mat4 gViewMatrix;

    Window gWindow;
    Camera gCamera;
    GameObject* gCameraTarget;

    std::vector<Model*> gModelVec;
    std::vector<GameObject*> gGameObjectVec;
    std::vector<Texture*> gTextureVec;
    std::vector<ShaderProgram*> gShaderVec;
    std::vector<Material*> gMaterialVec;

    bool gKeyStates[255];
    bool gLastKeyStates[255];
    int gMouseX, gMouseY;
    int gMouseButtonLeft, gMouseButtonRight;

    void (*Updatecallback)(float dt);
    void (*Rendercallback)(void);

    void GLUTCALLBACK_Resize(int width, int height) {
        gWindow.width = width;
        gWindow.height = height;
        glViewport(0, 0, width, height);
    }

    void GLUTCALLBACK_Idle() {
        Update();
    }

    void GLUTCALLBACK_Display() {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        Render();

        glutPostRedisplay();
        glutSwapBuffers();
    }

    void Update() {
        // Get the curren time in milliseconds
        g_CurrTime = glutGet(GLUT_ELAPSED_TIME);
        float elapsedTimeInMilliseconds = g_CurrTime - g_PrevTime;
        g_PrevTime = g_CurrTime;

        float elapsedTimeInSeconds = elapsedTimeInMilliseconds / 1000.f;
        if (elapsedTimeInSeconds > 0.1f)
            elapsedTimeInSeconds = 0.1f;

        g_PhysicsWorld->TimeStep(elapsedTimeInSeconds);
        g_AnimationManager.Update(gGameObjectVec, elapsedTimeInSeconds);

        if (GDP_IsKeyPressed('n'))
        {
            RenderObjects = !RenderObjects;
        }
        if (GDP_IsKeyPressed('m'))
        {
            RenderPhysicsDebug = !RenderPhysicsDebug;
        }

        Updatecallback(elapsedTimeInSeconds);
        memcpy(&(gLastKeyStates[0]), &(gKeyStates[0]), 255);
    }

    Model* GetModel(unsigned int id) {
        return gModelVec[id];
    }

    Material* GetMaterial(unsigned int id) {
        return gMaterialVec[id];
    }

    Texture* GetTexture(unsigned int id) {
        return gTextureVec[id];
    }

    ShaderProgram* GetShaderProgram(unsigned int id) {
        return gShaderVec[id];
    }

    void RenderGameObject(GameObject* go, const glm::mat4& parentModelMatrix)
    {
        glm::mat4 TranslationMatrix = glm::translate(parentModelMatrix, go->Position);
        glm::mat4 RotationMatrix = glm::mat4_cast(go->Rotation);
        glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.0f), go->Scale);
        glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;

        if (go->HasBones)
        {
            ShaderProgram* shader;
            shader = GetShaderProgram(gBoneShaderId);
            glUseProgram(shader->id);
            glUniformMatrix4fv(gBoneShader.ProjectionMatrix, 1, GL_FALSE, glm::value_ptr(gProjectionMatrix));
            glUniformMatrix4fv(gBoneShader.ViewMatrix, 1, GL_FALSE, glm::value_ptr(gViewMatrix));

            Material* material = GetMaterial(go->Renderer.MaterialId);
            Texture* texture = GetTexture(material->TextureId);

            glActiveTexture(texture->glEnum);
            glBindTexture(GL_TEXTURE_2D, texture->id);
            glUniform1i(gBoneShader.Texture00, texture->id - 1);

            glUniform3fv(gBoneShader.Color, 1, glm::value_ptr(material->Color));

            glUniformMatrix4fv(gBoneShader.ModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
            glUniformMatrix4fv(gBoneShader.RotationMatrix, 1, GL_FALSE, glm::value_ptr(RotationMatrix));

            for (int i = 0; i < 4; i++)
            {
                glUniformMatrix4fv(gBoneShader.BoneMatrices[i], 1, GL_FALSE, glm::value_ptr(go->BoneModelMatrices[i]));
                glUniformMatrix4fv(gBoneShader.BoneRotationMatrices[i], 1, GL_FALSE, glm::value_ptr(go->BoneRotationMatrices[i]));
            }

            Model* model = GetModel(go->Renderer.MeshId);
            glBindVertexArray(model->Vbo);
            glDrawElements(GL_TRIANGLES, model->NumTriangles * 3, GL_UNSIGNED_INT, (GLvoid*)0);
        }
        else
        {
            ShaderProgram* shader;
            shader = GetShaderProgram(gSimpleShaderId);
            glUseProgram(shader->id);

            glUniformMatrix4fv(gSimpleShader.ProjectionMatrix, 1, GL_FALSE, glm::value_ptr(gProjectionMatrix));
            glUniformMatrix4fv(gSimpleShader.ViewMatrix, 1, GL_FALSE, glm::value_ptr(gViewMatrix));

            Material* material = GetMaterial(go->Renderer.MaterialId);
            Texture* texture = GetTexture(material->TextureId);

            glActiveTexture(texture->glEnum);
            glBindTexture(GL_TEXTURE_2D, texture->id);
            glUniform1i(gSimpleShader.Texture00, texture->id - 1);

            glUniform3fv(gSimpleShader.Color, 1, glm::value_ptr(material->Color));

            glUniformMatrix4fv(gSimpleShader.ModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
            glUniformMatrix4fv(gSimpleShader.RotationMatrix, 1, GL_FALSE, glm::value_ptr(RotationMatrix));

            Model* model = GetModel(go->Renderer.MeshId);
            glBindVertexArray(model->Vbo);
            glDrawElements(GL_TRIANGLES, model->NumTriangles * 3, GL_UNSIGNED_INT, (GLvoid*)0);
        }


        for (int i = 0; i < go->Children.size(); i++)
        {
            RenderGameObject(go->Children[i], ModelMatrix);
        }
    }

    void Render() {
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LEQUAL);
        glCullFace(GL_BACK);

        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        glm::vec3 direction = gCamera.direction;
        if (gCamera.targetting == true && gCameraTarget != nullptr)
            direction = glm::normalize(gCameraTarget->Position - gCamera.position);


        gProjectionMatrix = glm::perspective(glm::radians(45.0f), ((GLfloat)gWindow.width) / ((GLfloat)gWindow.height), 0.1f, 1000.0f);
        gViewMatrix = glm::lookAt(gCamera.position, gCamera.position + direction, glm::vec3(0.0f, 1.0f, 0.0f));

        if (RenderObjects)
        {
            glm::mat4 worldOrigin(1.f); // Identity Matrix
            for (int i = 0; i < gGameObjectVec.size(); i++)
            {
                GameObject* go = gGameObjectVec[i];
                if (!go->Enabled || go->HasParent)
                    continue;

                RenderGameObject(go, worldOrigin);
            }
        }

        if (RenderPhysicsDebug)
        {
            glUseProgram(GetShaderProgram(gLineShaderId)->id);
            glUniformMatrix4fv(gLineShader.ProjectionMatrix, 1, GL_FALSE, glm::value_ptr(gProjectionMatrix));
            glUniformMatrix4fv(gLineShader.ViewMatrix, 1, GL_FALSE, glm::value_ptr(gViewMatrix));
            GDP_GetPhysicsWorld()->DebugDraw();
            CheckGLError();
        }
        
        if (Rendercallback != nullptr)
            Rendercallback();
    }

    void GLUTCALLBACK_KeyboardFunc(unsigned char key, int x, int y) {
        gKeyStates[key] = true;


    }

    void GLUTCALLBACK_KeyboardUpFunc(unsigned char key, int x, int y) {
        gKeyStates[key] = false;
    }

    void GLUTCALLBACK_SpecialFunc(int key, int x, int y) {
        printf("Special: %d\n", key);
    }

    void GLUTCALLBACK_SpecialUpFunc(int key, int x, int y) {
        printf("Special: %d\n", key);
    }

    void CALLBACK_MouseMotion(int x, int y) {
        gMouseX = x;
        gMouseY = y;
    }

    void CALLBACK_MouseButton(int button, int state, int x, int y) {
        if (button == GLUT_LEFT_BUTTON)
            gMouseButtonLeft = state;
        else if (button == GLUT_RIGHT_BUTTON)
            gMouseButtonRight = state;
        gMouseX = x;
        gMouseY = y;
    }

    // Engine
    void GDP_Initialize() {
        int unused1 = 0;
        char** unused2 = { };
        glutInit(&unused1, unused2);

        glutInitContextVersion(4, 0);
        glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
        glutInitContextProfile(GLUT_CORE_PROFILE);
        glutSetOption(
            GLUT_ACTION_ON_WINDOW_CLOSE,
            GLUT_ACTION_GLUTMAINLOOP_RETURNS
        );
    }

    void GDP_Destroy() {

    }

    void GDP_CreateWindow(const char* title, const int width, const int height) {
        glutInitWindowSize(width, height);
        glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
        glutCreateWindow(title);

        glutIdleFunc(GLUTCALLBACK_Idle);
        glutReshapeFunc(GLUTCALLBACK_Resize);
        glutDisplayFunc(GLUTCALLBACK_Display);

        glutKeyboardFunc(GLUTCALLBACK_KeyboardFunc);
        glutKeyboardUpFunc(GLUTCALLBACK_KeyboardUpFunc);

        glutSpecialFunc(GLUTCALLBACK_SpecialFunc);
        glutSpecialUpFunc(GLUTCALLBACK_SpecialUpFunc);

        glutMouseFunc(CALLBACK_MouseButton);
        glutMotionFunc(CALLBACK_MouseMotion);

        glutPassiveMotionFunc(CALLBACK_MouseMotion);

        glClearColor(0.3f, 0.3f, 0.3f, 1.f);

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LEQUAL);
        glCullFace(GL_BACK);

        glewInit();

        // Create physics factory and the world
        g_PhysicsFactory = new PhysicsFactoryType();
        g_PhysicsWorld = g_PhysicsFactory->CreateWorld();

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    physics::iPhysicsWorld* GDP_GetPhysicsWorld()
    {
        return g_PhysicsWorld;
    }

    physics::iRigidBody* GDP_CreateRigidBody(const physics::RigidBodyDesc& desc, physics::iShape* shape)
    {
        return g_PhysicsFactory->CreateRigidBody(desc, shape);
    }

    physics::iSoftBody* GDP_CreateSoftBody(const physics::SoftBodyDesc& desc)
    {
        return g_PhysicsFactory->CreateSoftBody(desc);
    }


    bool LoadAssimpMesh(unsigned int& id, const aiMesh* mesh)
    {
        if (mesh == nullptr)
            return false;
        printf("LoadAssimpMesh %s\n", mesh->mName.C_Str());

        unsigned int numFaces = mesh->mNumFaces;

        unsigned int numIndicesInIndexArray = numFaces * 3;

        sVertex_p4t4n4* pTempVertArray = new sVertex_p4t4n4[numIndicesInIndexArray * 2];
        GLuint* pIndexArrayLocal = new GLuint[numIndicesInIndexArray * 2];

        std::vector<unsigned int> triangles;
        int vertArrayIndex = 0;
        for (int faceIndex = 0; faceIndex < numFaces; faceIndex++)
        {
            const aiFace& face = mesh->mFaces[faceIndex];

            // You can assert here to ensure mNumIndices is 3
            // Unless you support non 3 face models 

            for (int indicesIndex = 0; indicesIndex < face.mNumIndices; indicesIndex++)
            {
                unsigned int vertexIndex = face.mIndices[indicesIndex];
                triangles.push_back(vertexIndex);

                const aiVector3D& vertex = mesh->mVertices[vertexIndex];

                pTempVertArray[vertArrayIndex].Pos.x = vertex.x;
                pTempVertArray[vertArrayIndex].Pos.y = vertex.y;
                pTempVertArray[vertArrayIndex].Pos.z = vertex.z;
                pTempVertArray[vertArrayIndex].Pos.w = 1.0f;

                if (mesh->HasNormals())
                {
                    const aiVector3D& normal = mesh->mNormals[vertexIndex];
                    pTempVertArray[vertArrayIndex].Normal.x = normal.x;
                    pTempVertArray[vertArrayIndex].Normal.y = normal.y;
                    pTempVertArray[vertArrayIndex].Normal.z = normal.z;
                    pTempVertArray[vertArrayIndex].Normal.w = 0.f;
                }
                else
                {
                    pTempVertArray[vertArrayIndex].Normal.x = 1.f;
                    pTempVertArray[vertArrayIndex].Normal.y = 0.f;
                    pTempVertArray[vertArrayIndex].Normal.z = 0.f;
                    pTempVertArray[vertArrayIndex].Normal.w = 0.f;
                }

                if (mesh->HasTextureCoords(0))
                {
                    const aiVector3D& uvCoord = mesh->mTextureCoords[0][vertexIndex];
                    pTempVertArray[vertArrayIndex].TexUVx2.x = uvCoord.x;
                    pTempVertArray[vertArrayIndex].TexUVx2.y = uvCoord.y;
                }
                if (mesh->HasTextureCoords(1))
                {
                    const aiVector3D& uvCoord = mesh->mTextureCoords[1][vertexIndex];
                    pTempVertArray[vertArrayIndex].TexUVx2.z = uvCoord.x;
                    pTempVertArray[vertArrayIndex].TexUVx2.w = uvCoord.y;
                }

                vertArrayIndex++;
            }
        }


        printf("  - Done loading Model data");

        Model* model = new Model();
        model->NumTriangles = numFaces;
        glGenVertexArrays(1, &model->Vbo);
        glBindVertexArray(model->Vbo);
        CheckGLError();

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        CheckGLError();

        glGenBuffers(1, &model->VertexBufferId);
        glGenBuffers(1, &model->IndexBufferId);
        CheckGLError();

        glBindBuffer(GL_ARRAY_BUFFER, model->VertexBufferId);
        CheckGLError();

        unsigned int totalVertBufferSizeBYTES = numIndicesInIndexArray * sizeof(sVertex_p4t4n4);
        glBufferData(GL_ARRAY_BUFFER, totalVertBufferSizeBYTES, pTempVertArray, GL_STATIC_DRAW);
        CheckGLError();

        unsigned int bytesInOneVertex = sizeof(sVertex_p4t4n4);
        unsigned int byteOffsetToPosition = offsetof(sVertex_p4t4n4, Pos);
        unsigned int byteOffsetToNormal = offsetof(sVertex_p4t4n4, Normal);
        unsigned int byteOffsetToUVCoords = offsetof(sVertex_p4t4n4, TexUVx2);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToPosition);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToUVCoords);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToNormal);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->IndexBufferId);

        unsigned int sizeOfIndexArrayInBytes = numIndicesInIndexArray * sizeof(GLuint);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexArrayInBytes, pIndexArrayLocal, GL_STATIC_DRAW);

        glBindVertexArray(0);

        delete[] pTempVertArray;
        delete[] pIndexArrayLocal;

        printf("  - Done Generating Buffer data");

        printf("  - Finished Loading model \"%s\" with %d vertices, %d triangles, id is: %d\n", mesh->mName.C_Str(), model->vertices.size(), model->NumTriangles, model->Vbo);
        id = gModelVec.size();
        gModelVec.push_back(model);
        return true;
    }

    void LoadAssimpAnimation(const aiAnimation* animation)
    {
        if (animation == nullptr)
            return;

        printf("LoadAssimpAnimation %s\n", animation->mName.C_Str());

        gdp::AnimationData animationData;
        animationData.Name = std::string(animation->mName.C_Str());
        animationData.Duration = animation->mDuration;
        animationData.TicksPerSecond = animation->mTicksPerSecond;

        unsigned int numChannels = animation->mNumChannels;

        printf("- Loading %d channels\n", numChannels);


        for (int i = 0; i < numChannels; i++)
        {

            const aiNodeAnim* nodeAnim = animation->mChannels[i];

            std::string name(nodeAnim->mNodeName.C_Str());

            unsigned int numPositionKeys = nodeAnim->mNumPositionKeys;
            unsigned int numRotationKeys = nodeAnim->mNumRotationKeys;
            unsigned int numScalingKeys = nodeAnim->mNumScalingKeys;

            animationData.PositionKeyFrames.resize(numPositionKeys);
            animationData.RotationKeyFrames.resize(numRotationKeys);
            animationData.ScaleKeyFrames.resize(numScalingKeys);

            for (int keyIdx = 0; keyIdx < numPositionKeys; keyIdx++)
            {
                const aiVectorKey& posKey = nodeAnim->mPositionKeys[keyIdx];
                animationData.PositionKeyFrames[keyIdx].time = posKey.mTime;
                animationData.PositionKeyFrames[keyIdx].value.x = posKey.mValue.x;
                animationData.PositionKeyFrames[keyIdx].value.y = posKey.mValue.y;
                animationData.PositionKeyFrames[keyIdx].value.z = posKey.mValue.z;
            }

            for (int keyIdx = 0; keyIdx < numRotationKeys; keyIdx++)
            {
                const aiQuatKey& rotKey = nodeAnim->mRotationKeys[keyIdx];
                animationData.RotationKeyFrames[keyIdx].time = rotKey.mTime;
                animationData.RotationKeyFrames[keyIdx].value.x = rotKey.mValue.x;
                animationData.RotationKeyFrames[keyIdx].value.y = rotKey.mValue.y;
                animationData.RotationKeyFrames[keyIdx].value.z = rotKey.mValue.z;
                animationData.RotationKeyFrames[keyIdx].value.w = rotKey.mValue.w;
            }

            for (int keyIdx = 0; keyIdx < numScalingKeys; keyIdx++)
            {
                const aiVectorKey& scaleKey = nodeAnim->mScalingKeys[keyIdx];
                animationData.ScaleKeyFrames[keyIdx].time = scaleKey.mTime;
                animationData.ScaleKeyFrames[keyIdx].value.x = scaleKey.mValue.x;
                animationData.ScaleKeyFrames[keyIdx].value.y = scaleKey.mValue.y;
                animationData.ScaleKeyFrames[keyIdx].value.z = scaleKey.mValue.z;
            }

            // Store AnimationData inside our animation manager
            g_AnimationManager.LoadAnimation(animationData.Name, animationData);
        }

        printf("- Done!\n");
    }

    void GDP_LoadFBXFile(unsigned int& meshId, std::string& animationName, const std::string& filename)
    {
        const aiScene* scene = g_Importer.ReadFile(filename,
            aiProcess_GenNormals |
            aiProcess_LimitBoneWeights |
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices);

        printf("GDP_LoadFBXFile: %s\n", filename.c_str());

        // Create our Bone Hierarchy
        aiNode* node = scene->mRootNode;
        for (int i = 0; i < node->mNumChildren; i++)
        {
            aiNode* child = node->mChildren[i];

            //Find channel data from our node name:
            child->mName;
        }


        if (scene->HasMeshes())
        {
            // Load all the meshes
            unsigned int numMeshes = scene->mNumMeshes;
            printf("-Loading %d meshes!\n", numMeshes);
            for (int i = 0; i < numMeshes; i++)
            {
                aiMesh* mesh = scene->mMeshes[i];
                if (!LoadAssimpMesh(meshId, mesh))
                {
                    printf("Failed to load mesh! \n");
                }
            }

            printf("\n");

            unsigned int numAnimations = scene->mNumAnimations;
            printf("-Loading %d animations!\n", numAnimations);
            for (int i = 0; i < numAnimations; i++)
            {
                aiAnimation* animation = scene->mAnimations[i];


                animationName = animation->mName.C_Str();
                LoadAssimpAnimation(animation);
            }
        }

        printf("Done loading FBX file!\n");

        system("Pause");
    }

    void GDP_LoadAnimation(const char* name, AnimationData animation)
    {
        g_AnimationManager.LoadAnimation(name, animation);
    }

    void GDP_UpdateCallback(void (*callback)(float)) {
        Updatecallback = callback;
    }

    void GDP_RenderCallback(void (*callback)(void)) {
        Rendercallback = callback;
    }

    void GDP_Run() {
        unsigned int textureId;
        unsigned int modelId;
        unsigned int materialId;
        unsigned int programId;

        GDP_CreateShaderProgramFromFiles(gSimpleShaderId, programId,
            "assets/shaders/SimpleShader.vertex.glsl",
            "assets/shaders/SimpleShader.fragment.glsl");
        CheckGLError();

        GDP_CreateShaderProgramFromFiles(gBoneShaderId, programId,
            "assets/shaders/BoneShader.vertex.glsl",
            "assets/shaders/BoneShader.fragment.glsl");
        CheckGLError();

        GDP_CreateShaderProgramFromFiles(gLineShaderId, programId,
            "assets/shaders/LineShader.vertex.glsl",
            "assets/shaders/LineShader.fragment.glsl");
        CheckGLError();


        ShaderProgram* shader;
        shader = GetShaderProgram(gSimpleShaderId);
        printf("gSimpleShaderId id: %d\n", shader->id);
        glUseProgram(shader->id);
        gSimpleShader.ModelMatrix = glGetUniformLocation(shader->id, "ModelMatrix");
        gSimpleShader.ViewMatrix = glGetUniformLocation(shader->id, "ViewMatrix");
        gSimpleShader.ProjectionMatrix = glGetUniformLocation(shader->id, "ProjectionMatrix");
        gSimpleShader.RotationMatrix = glGetUniformLocation(shader->id, "RotationMatrix");
        gSimpleShader.Texture00 = glGetUniformLocation(shader->id, "Texture00");
        gSimpleShader.Color = glGetUniformLocation(shader->id, "Color");
        CheckGLError();

        shader = GetShaderProgram(gBoneShaderId);
        printf("gBoneShaderId id: %d\n", shader->id);
        glUseProgram(shader->id);
        gBoneShader.ModelMatrix = glGetUniformLocation(shader->id, "ModelMatrix");
        gBoneShader.ViewMatrix = glGetUniformLocation(shader->id, "ViewMatrix");
        gBoneShader.ProjectionMatrix = glGetUniformLocation(shader->id, "ProjectionMatrix");
        gBoneShader.RotationMatrix = glGetUniformLocation(shader->id, "RotationMatrix");
        gBoneShader.Texture00 = glGetUniformLocation(shader->id, "Texture00");
        gBoneShader.Color = glGetUniformLocation(shader->id, "Color");
        gBoneShader.BoneMatrices[0] = glGetUniformLocation(shader->id, "BoneMatrices[0]");
        gBoneShader.BoneMatrices[1] = glGetUniformLocation(shader->id, "BoneMatrices[1]");
        gBoneShader.BoneMatrices[2] = glGetUniformLocation(shader->id, "BoneMatrices[2]");
        gBoneShader.BoneMatrices[3] = glGetUniformLocation(shader->id, "BoneMatrices[3]");
        gBoneShader.BoneRotationMatrices[0] = glGetUniformLocation(shader->id, "BoneRotationMatrices[0]");
        gBoneShader.BoneRotationMatrices[1] = glGetUniformLocation(shader->id, "BoneRotationMatrices[1]");
        gBoneShader.BoneRotationMatrices[2] = glGetUniformLocation(shader->id, "BoneRotationMatrices[2]");
        gBoneShader.BoneRotationMatrices[3] = glGetUniformLocation(shader->id, "BoneRotationMatrices[3]");
        CheckGLError();

        shader = GetShaderProgram(gLineShaderId);
        printf("gLineShaderId id: %d\n", shader->id);
        glUseProgram(shader->id);
        gLineShader.ViewMatrix = glGetUniformLocation(shader->id, "ViewMatrix");
        gLineShader.ProjectionMatrix = glGetUniformLocation(shader->id, "ProjectionMatrix");
        CheckGLError();
        //GDP_LoadTexture(textureId, "assets/textures/MetalPipeWallRusty_basecolor.png");

        //GDP_LoadModel(modelId, "assets/models/cube.fbx");

        //GDP_CreateMaterial(materialId, textureId, glm::vec3(1, 1, 1));

        //GameObject* go = GDP_CreateGameObject();
        //go->Renderer.ShaderId = shaderId;
        //go->Renderer.MeshId = modelId;
        //go->Renderer.MaterialId = materialId;

        gCamera.position = glm::vec3(0.0f, 0.0f, -10.0f);
        gCamera.direction = glm::vec3(0.0f, 0.0f, 1.0f);
        glutMainLoop();
    }

    void GDP_LoadTexture(unsigned int& id, const char* filepath) {
        Texture* texture = new Texture(filepath);
        id = gTextureVec.size();
        gTextureVec.push_back(texture);
    }

    void GDP_LoadModel(unsigned int& id, const char* filepath, bool withBones) {
        Model* model = nullptr;
        if (withBones)
            model = new Model(filepath, true);
        else
            model = new Model(filepath);
        printf("Loaded model \"%s\" with %d vertices, %d triangles, id is: %d\n", filepath, model->vertices.size(), model->NumTriangles, model->Vbo);
        id = gModelVec.size();
        gModelVec.push_back(model);
    }


    void GDP_CreateModel(unsigned int& id, const std::vector<glm::vec3>& vertices, const std::vector<int>& triangles) {
        Model* model = new Model(vertices, triangles);
        printf("Created model with %d triangles, id is: %d\n", model->NumTriangles, model->Vbo);
        id = gModelVec.size();
        gModelVec.push_back(model);
    }

    void GDP_GetModelData(unsigned int id,
        std::vector<glm::vec3>& vertices, std::vector<int>& triangles,
        unsigned int& numTriangles, unsigned int& vbo) {
        assert(id < gModelVec.size());
        const Model* model = gModelVec[id];
        numTriangles = model->NumTriangles;
        vbo = model->Vbo;
        vertices = model->vertices;
        triangles = model->triangles;
    }

    void GDP_CreateMaterial(unsigned int& id, unsigned int textureId, const glm::vec3& color) {
        Material* material = new Material();
        material->TextureId = textureId;
        material->Color = color;
        id = gMaterialVec.size();
        gMaterialVec.push_back(material);
    }

    void GDP_CreateShaderProgramFromFiles(unsigned int& id, unsigned int& shaderProgramId,
        const char* vertexFilepath, const char* fragmentFilepath) {
        ShaderProgram* shaderProgram = new ShaderProgram(vertexFilepath, fragmentFilepath);
        shaderProgramId = shaderProgram->id;
        id = gShaderVec.size();
        gShaderVec.push_back(shaderProgram);
    }

    GameObject* GDP_CreateGameObject(bool addToWorld /* = true */) {
        GameObject* go = new GameObject();
        go->Position = glm::vec3(0.0f);
        go->Scale = glm::vec3(1.0f);
        go->Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        if (addToWorld)
            gGameObjectVec.push_back(go);

        return go;
    }

    void GDP_SetCameraTarget(GameObject* go) {
        gCameraTarget = go;
        gCamera.targetting = true;
    }
     
    void SetCameraPosition(const glm::vec3& position)
    {
        gCamera.position = position;
    }

    void SetCameraFacingDirection(const glm::vec3& direction)
    {
        gCamera.direction = direction;
        gCamera.targetting = false;
    }

    void GetCameraPosition(glm::vec3& position)
    {
        position = gCamera.position;
    }

    void GetCameraFacingDirection(glm::vec3& direction)
    {
        direction = gCamera.direction;
    }


    bool GDP_IsKeyHeldDown(unsigned char key) {
        return gLastKeyStates[key] && gKeyStates[key];
    }

    bool GDP_IsKeyPressed(unsigned char key) {
        return !gLastKeyStates[key] && gKeyStates[key];
    }

    bool GDP_IsKeyReleased(unsigned char key) {
        return gLastKeyStates[key] && !gKeyStates[key];
    }

    void GDP_GetMousePosition(int& x, int& y) {
        x = gMouseX;
        y = gMouseY;
    }

    bool GDP_GetMouseButtonState(int button, int& state) {
        if (button == GLUT_LEFT_BUTTON)
            return state == gMouseButtonLeft;
        if (button == GLUT_RIGHT_BUTTON)
            return state == gMouseButtonRight;
        return false;
    }
}