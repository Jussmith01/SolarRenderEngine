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
out vec3 Position;
out vec3 CameraPos;

uniform mat4 model;

void main()
{
	gl_Position = model * vec4(position, 1.0f);
	Position = vec3(model * vec4(position,1.0f));
	TexCoord = vec2(texCoord.x, 1 - texCoord.y);
	Normal = mat3(transpose(inverse(model))) * normal;
	CameraPos = cPos;
}

