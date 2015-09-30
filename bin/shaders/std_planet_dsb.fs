#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 CameraPos;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D bumpMap;
};

struct Light {
    //Material Lighting Properties
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirectionalLightVars {
	vec3 direction;
	float intensity;
};

vec3 DirectionalLight(vec3 Position,vec3 norm,Material material,Light light,DirectionalLightVars Dlight,vec3 cPos)
{
        //Properties
	vec3 output;

	vec3 lightDir = normalize(-Dlight.direction);
	vec3 viewDir = normalize(cPos - Position);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	//Shading
	float diffuse = max(dot(norm, lightDir), 0.0) * Dlight.intensity;
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), light.shininess) * Dlight.intensity;
	
	//Combining
        vec3 PointambientColor = light.ambient * vec3(texture(material.diffuseMap,TexCoord));
        vec3 PointdiffuseColor = diffuse * light.diffuse * vec3(texture(material.diffuseMap,TexCoord)); //* light.intensity;

        vec3 greyscaleMap = vec3(texture(material.specularMap,TexCoord));
        float specIntensity = (greyscaleMap.x + greyscaleMap.y + greyscaleMap.z)/3.0f;
        vec3 PointspecularColor = specular * light.specular * specIntensity * vec3(texture(material.diffuseMap,TexCoord));  //* light.intensity;
 
	output = vec3(PointambientColor + PointdiffuseColor + PointspecularColor);

        return output;
};

vec3 bumpNormal (Material material,vec3 norm,vec3 cPos)
{
	vec3 bNorm;

	bNorm = 2.0f * vec3(texture(material.bumpMap,TexCoord)) - 1.0f;
	float scale = abs((bNorm.x + bNorm.x + bNorm.x)/3.0f);
	bNorm = 0.15f*(1.0f-scale)*(normalize(-cPos) - norm) + norm;
	bNorm = normalize(bNorm);

	return bNorm;
};

out vec4 color;

uniform Material material;
uniform Light light;
uniform DirectionalLightVars Dlight;

void main()
{
        vec3 norm = normalize(Normal);
		
	vec3 bumpedNorm = bumpNormal(material,norm,CameraPos);

	vec3 output;
	
	//Directional Lighting
	output = DirectionalLight(FragPos,bumpedNorm,material,light,Dlight,CameraPos);

	color = vec4(output,1.0f);
};

