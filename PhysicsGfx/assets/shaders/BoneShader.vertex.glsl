#version 400

layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_UV_x2;
layout(location=2) in vec4 in_Normal;
layout(location=3) in vec4 in_BoneIds;
layout(location=4) in vec4 in_BoneWeights;

out vec4 ex_Position;			
out vec4 ex_PositionWorld;
out vec4 ex_Normal;
out vec4 ex_UV_x2;

uniform mat4 ModelMatrix;	
uniform mat4 RotationMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

// BoneMatrix is Transforms, Rotations, and Scale
uniform mat4 BoneMatrices[4];

// Only include Rotations, for updating the normals
uniform mat4 BoneRotationMatrices[4];

void main(void)
{
	mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	vec4 position = vec4(0,0,0,0);
	position += BoneMatrices[int(in_BoneIds[0])] * in_Position * in_BoneWeights[0];
	position += BoneMatrices[int(in_BoneIds[1])] * in_Position * in_BoneWeights[1];
	position += BoneMatrices[int(in_BoneIds[2])] * in_Position * in_BoneWeights[2];
	position += BoneMatrices[int(in_BoneIds[3])] * in_Position * in_BoneWeights[3];

	// Next step
	// Include Rotations
	vec4 normal = vec4(0,0,0,0);
	normal += BoneMatrices[int(in_BoneIds[0])] * normalize(in_Normal) * in_BoneWeights[0];
	normal += BoneMatrices[int(in_BoneIds[1])] * normalize(in_Normal) * in_BoneWeights[1];
	normal += BoneMatrices[int(in_BoneIds[2])] * normalize(in_Normal) * in_BoneWeights[2];
	normal += BoneMatrices[int(in_BoneIds[3])] * normalize(in_Normal) * in_BoneWeights[3];

	gl_Position = MVP * position;
	ex_Position = in_Position;
	ex_PositionWorld = ModelMatrix * in_Position;
	ex_Normal = RotationMatrix * normalize(normal);
	ex_UV_x2 = in_UV_x2;
}
