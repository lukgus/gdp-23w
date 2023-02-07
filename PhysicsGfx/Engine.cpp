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

#include "GL.h"

//#include <physics/gdp/PhysicsFactory.h>

namespace gdp
{
    // Initialize a Physics Factory
   // physics::iPhysicsFactory* g_PhysicsFactory;
    //physics::iPhysicsWorld* g_PhysicsWorld;

    // Managers
    AnimationManager g_AnimationManager;

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

    unsigned int gBoneShaderId;
    unsigned int gSimpleShaderId;

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

       // g_PhysicsWorld->TimeStep(elapsedTimeInSeconds);
        g_AnimationManager.Update(gGameObjectVec, elapsedTimeInSeconds);

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

        glm::mat4 worldOrigin(1.f); // Identity Matrix
        for (int i = 0; i < gGameObjectVec.size(); i++)
        {
            GameObject* go = gGameObjectVec[i];
            if (!go->Enabled || go->HasParent)
                continue;

            RenderGameObject(go, worldOrigin);
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

        // Create physics factory and the world
        //g_PhysicsFactory = new PhysicsFactoryType();
       // g_PhysicsWorld = g_PhysicsFactory->CreateWorld();
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


        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    //physics::iPhysicsWorld* GDP_GetPhysicsWorld()
    //{
    //    return g_PhysicsWorld;
    //}

    //physics::iRigidBody* GDP_CreateRigidBody(const physics::RigidBodyDesc& desc, physics::iShape* shape)
    //{
    //    return g_PhysicsFactory->CreateRigidBody(desc, shape);
    //}

    //physics::iSoftBody* GDP_CreateSoftBody(const physics::SoftBodyDesc& desc)
    //{
    //    return g_PhysicsFactory->CreateSoftBody(desc);
    //}


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

        GDP_CreateShaderProgramFromFiles(gBoneShaderId, programId,
            "assets/shaders/BoneShader.vertex.glsl",
            "assets/shaders/BoneShader.fragment.glsl");

        ShaderProgram* shader;
        shader = GetShaderProgram(gSimpleShaderId);
        glUseProgram(shader->id);
        gSimpleShader.ModelMatrix = glGetUniformLocation(shader->id, "ModelMatrix");
        gSimpleShader.ViewMatrix = glGetUniformLocation(shader->id, "ViewMatrix");
        gSimpleShader.ProjectionMatrix = glGetUniformLocation(shader->id, "ProjectionMatrix");
        gSimpleShader.RotationMatrix = glGetUniformLocation(shader->id, "RotationMatrix");
        gSimpleShader.Texture00 = glGetUniformLocation(shader->id, "Texture00");
        gSimpleShader.Color = glGetUniformLocation(shader->id, "Color");

        shader = GetShaderProgram(gBoneShaderId);
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