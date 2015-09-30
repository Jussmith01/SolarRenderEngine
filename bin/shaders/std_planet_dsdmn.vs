#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

layout (std140) uniform GlobalVars
{
    float GLtime;
    vec3 cPos;
};

layout (std140) uniform Matrices
{
	mat4 projMat;
	mat4 viewMat;
};

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec3 CameraPos;
out vec3 VT;
//out vec3 VB;
out vec3 VN;

uniform mat4 modelmat;

void main()
{
	vec3 norm = normalize(normal);

	//GL OUTPUT
	mat4 VMM = viewMat * modelmat;
	gl_Position = projMat * VMM * vec4(position, 1.0f);

	//FRAGMENT OUTPUT
	FragPos = vec3(modelmat * vec4(position,1.0));
	TexCoord = vec2(texCoord.x, 1 - texCoord.y);
	Normal = mat3(transpose(inverse(modelmat))) * normal;
	CameraPos = cPos;

	vec3 WSNormal = vec3(modelmat * vec4(normalize(normal),0.0f));
	vec3 WSTangent = vec3(modelmat * vec4(normalize(tangent),0.0f));
	//vec3 WSBitangent = vec3(modelmat * vec4(normalize(bitangent),0.0f));

	VT = WSTangent;
	//VB = WSBitangent;
	VN = WSNormal;
}

