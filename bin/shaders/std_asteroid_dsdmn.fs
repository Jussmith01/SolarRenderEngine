#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 CameraPos;
in vec3 ObjPos;
in vec3 VT;
//in vec3 VB;
in vec3 VN;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D displacementMap;
    sampler2D normalMap;

    float offsetNormalization;
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

vec2 CalcVertexDisplacement(mat3 TBN,Material material,vec3 viewDir)
{
        int NUM_ITERATIONS = 16;

	vec3 offset = normalize(viewDir);
	offset *= material.offsetNormalization;

        vec2 texCoord = TexCoord;
        for (int i = 0; i < NUM_ITERATIONS; i++) 
        {
                float distance = texture2D(material.displacementMap, texCoord).x;
                texCoord = texCoord + (distance * offset).xy;
        }
	
        return texCoord;
};

vec3 CalcBumpedNormal(mat3 TBN,Material material,vec2 texCoord,vec2 dx,vec2 dy)
{
        vec3 BumpMapNormal = vec3(textureGrad(material.normalMap, texCoord,dx,dy));
        BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
        vec3 NewNormal;
        NewNormal = TBN * vec3(BumpMapNormal.r,BumpMapNormal.g,BumpMapNormal.b);
        NewNormal = normalize(NewNormal);
        return NewNormal;
};

mat3 CalcTBN(vec3 Normal0,vec3 Tangent0)
{
        vec3 Normal = normalize(Normal0);
        vec3 Tangent = normalize(Tangent0);
        Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
        vec3 Bitangent = cross(Tangent, Normal);
        mat3 TBN = mat3(Tangent, Bitangent, Normal);
        return TBN;
};

vec3 DirectionalLight(Material material,Light light,DirectionalLightVars Dlight,vec3 LightDirection)
{
	vec3 output;

	//Calculate Pixel Displacement
	mat3 TBN = CalcTBN(VN,VT);

        vec3 viewDir = normalize(CameraPos - FragPos);
	vec2 texCoord = CalcVertexDisplacement(TBN,material,viewDir);
	//vec2 texCoord = TexCoord;

	//Calculate Derivative
        vec2 dx = dFdx(TexCoord);
        vec2 dy = dFdy(TexCoord);

	//Calculate Bumped Normal
	vec3 TransNormal = CalcBumpedNormal(TBN,material,texCoord,dx,dy);

	//Calculate vectors needed for lighting
        vec3 lightDir = -normalize(LightDirection);
	vec3 reflectDir = reflect(-lightDir, TransNormal);
	
	//Shading
	float diffuse = max(dot(TransNormal, lightDir), 0.0);// * Dlight.intensity;
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), light.shininess);// * Dlight.intensity;
	
        //Combining
	vec3 DiffMapColor = vec3(textureGrad(material.diffuseMap,texCoord,dx,dy)); 

        vec3 PointambientColor = light.ambient * DiffMapColor;
        vec3 PointdiffuseColor = diffuse * light.diffuse * DiffMapColor; //* light.intensity;

	vec3 greyscaleMap = vec3(textureGrad(material.specularMap,texCoord,dx,dy));
	float specIntensity = (greyscaleMap.x + greyscaleMap.y + greyscaleMap.z)/3.0f;
        vec3 PointspecularColor = specular * light.specular * specIntensity * DiffMapColor;  //* light.intensity;
 
	output = vec3(PointambientColor + PointdiffuseColor + PointspecularColor);

        return output;
};

out vec4 color;

uniform Material material;
uniform Light light;
uniform DirectionalLightVars Dlight;

void main()
{
	vec3 LightDirection = ObjPos - Dlight.direction;

	//Directional Lighting
	vec3 output = DirectionalLight(material,light,Dlight,LightDirection);
	//vec3 output = vec3(1.0f);

	color = vec4(output,1.0f);
};

