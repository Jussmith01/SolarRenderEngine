#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

layout (std140) uniform Matrices
{
    float GLtime;
    vec3 cPos;
};

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec3 CameraPos;

//uniform mat4 proj;
uniform mat4 transmat;
uniform mat4 modelmat;
//uniform mat4 view;

void main()
{
	gl_Position = transmat * modelmat * vec4(position, 1.0f);
	FragPos = vec3(modelmat * vec4(position,1.0f));
	TexCoord = vec2(texCoord.x, 1 - texCoord.y);
	Normal = mat3(transpose(inverse(modelmat))) * normal;
	CameraPos = cPos;
}

