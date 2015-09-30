#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform sampler2D displacementMap;

vec3 DisplaceVertexByMap(vec3 vPos,vec3 Norm)
{
        vec3 dispMap = vec3(texture(displacementMap,texCoord));
        float dispIntensity = (2.0f * (dispMap.x + dispMap.y + dispMap.z)/3.0f) - 1;

	vec3 DispPos = vPos + (0.02 * dispIntensity * Norm);

	return DispPos;
};

layout (std140) uniform Matrices
{
    float GLtime;
    vec3 cPos;
};

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec3 CameraPos;

uniform mat4 transmat;
uniform mat4 modelmat;

void main()
{
	vec3 norm = normalize(normal);
	vec3 PosDisp = DisplaceVertexByMap(position,norm);

	gl_Position = transmat * modelmat * vec4(PosDisp, 1.0f);
	FragPos = vec3(modelmat * vec4(PosDisp,1.0f));
	TexCoord = vec2(texCoord.x, 1 - texCoord.y);
	Normal = mat3(transpose(inverse(modelmat))) * normal;
	CameraPos = cPos;
}

