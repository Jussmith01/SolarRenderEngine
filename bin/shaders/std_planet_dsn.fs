#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 CameraPos;
in vec3 VT;
in vec3 VB;
in vec3 VN;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D displacementMap;
    sampler2D normalMap;
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

vec3 DirectionalLight(vec3 norm,Material material,Light light,DirectionalLightVars Dlight)
{
        //Properties
	vec3 output;

	vec3 lightDir = -normalize(Dlight.direction);
	//vec3 lightDir = tanLightVec;
	vec3 viewDir = normalize(CameraPos - FragPos);
	//vec3 viewDir = tanEyeVec;
	vec3 reflectDir = reflect(-lightDir, norm);
	
	//Shading
	float diffuse = max(dot(norm, lightDir), 0.0);// * Dlight.intensity;
	//float diffuse = clamp(dot(norm, lightDir), 0.0,1.0);// * Dlight.intensity;
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), light.shininess);// * Dlight.intensity;
	//float specular = pow(clamp(dot(viewDir, reflectDir), 0.0,1.0), light.shininess);// * Dlight.intensity;
	
        //Combining
        vec3 PointambientColor = light.ambient * vec3(texture(material.diffuseMap,TexCoord));
        vec3 PointdiffuseColor = diffuse * light.diffuse * vec3(texture(material.diffuseMap,TexCoord)); //* light.intensity;

	vec3 greyscaleMap = vec3(texture(material.specularMap,TexCoord));
	float specIntensity = (greyscaleMap.x + greyscaleMap.y + greyscaleMap.z)/3.0f;
        vec3 PointspecularColor = specular * light.specular * specIntensity * vec3(texture(material.diffuseMap,TexCoord));  //* light.intensity;
 
	output = vec3(PointambientColor + PointdiffuseColor + PointspecularColor);
	//output = vec3(PointambientColor + PointdiffuseColor);

        return output;
};

vec3 CalcBumpedNormal(vec3 Normal0,vec3 Tangent0,Material material)
{
    vec3 Normal = normalize(Normal0);
    vec3 Tangent = normalize(Tangent0);
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
    vec3 Bitangent = cross(Tangent, Normal);
    vec3 BumpMapNormal = texture(material.normalMap, TexCoord).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(Tangent, Bitangent, Normal);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}

out vec4 color;

uniform Material material;
uniform Light light;
uniform DirectionalLightVars Dlight;

void main()
{
	vec3 TransNormal = CalcBumpedNormal(VN,VT,material);
	vec3 output;
	
	//Directional Lighting
	output = DirectionalLight(TransNormal,material,light,Dlight);

	color = vec4(output,1.0f);
};

