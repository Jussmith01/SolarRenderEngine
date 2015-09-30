#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 Position;
in vec3 CameraPos;

struct Material {
    sampler2D diffuseMap;
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

struct PointLightVars {
    	vec3 lightPos;
	float constant;
	float linear;
	float quadratic;
};

struct FlashLightVars {
    	vec3 lightPos;
        
	float constant;
        float linear;
        float quadratic;

        vec3 spotDir;
        float spotCutOff;
        float spotOuterCutOff;
};

vec3 DirectionalLight(vec3 Position,vec3 norm,Material material,Light light,DirectionalLightVars Dlight,vec3 cPos)
{
        //Properties
	vec3 output;
	if (Dlight.intensity > 1.0E-4)
	{
	        vec3 lightDir = normalize(-Dlight.direction);
	        vec3 viewDir = normalize(cPos - Position);
	        vec3 reflectDir = reflect(-lightDir, norm);
	
	        //Shading
	        float diffuse = max(dot(norm, lightDir), 0.0) * Dlight.intensity;
	
	        //Combining
	        //vec3 PointambientColor = light.ambient * vec3(mix(texture(material.diffuseMap1,TexCoord),texture(material.diffuseMap2,TexCoord), 0.2));
	        //vec3 PointdiffuseColor = diffuse * light.diffuse * vec3(mix(texture(material.diffuseMap1,TexCoord),texture(material.diffuseMap2,TexCoord), 0.2)); //* light.intensity;
	        //vec3 PointspecularColor = specular * light.specular * vec3(mix(texture(material.specularMap1,TexCoord),texture(material.specularMap2,TexCoord), 0.2));  //* light.intensity;
     
                //Combining
		//vec3 ambient = vec3(0.2f,0.2f,0.2f);
                vec3 PointambientColor = light.ambient * vec3(texture(material.diffuseMap,TexCoord));
                vec3 PointdiffuseColor = diffuse * light.diffuse * vec3(texture(material.diffuseMap,TexCoord)); //* light.intensity;
 
                //Combining
                //vec3 PointambientColor = light.ambient;
                //vec3 PointdiffuseColor = diffuse * light.diffuse; //* light.intensity;
                //vec3 PointspecularColor = specular * light.specular;  //* light.intensity;
 
	        output = vec3(PointambientColor + PointdiffuseColor);
	}

        return output;
};

vec3 PointLight(vec3 Position,vec3 norm,Material material,Light light,PointLightVars Plight,vec3 cPos)
{
        //Properties
        vec3 lightDir = normalize(Plight.lightPos - Position);
        vec3 viewDir = normalize(cPos - Position);
        vec3 reflectDir = reflect(-lightDir, norm);

        //Shading
        float diffuse = max(dot(norm, lightDir), 0.0);

        //Attenuation
        float distance = length(Plight.lightPos - Position);
        float attenuation = 1.0f / (Plight.constant + Plight.linear * distance + Plight.quadratic * (distance * distance));

        //Combining
        //vec3 PointambientColor = light.ambient * vec3(mix(texture(material.diffuseMap1,TexCoord),texture(material.diffuseMap2,TexCoord), 0.2));
        //vec3 PointdiffuseColor = diffuse * light.diffuse * vec3(mix(texture(material.diffuseMap1,TexCoord),texture(material.diffuseMap2,TexCoord), 0.2));
        //vec3 PointspecularColor = specular * light.specular  * vec3(mix(texture(material.specularMap1,TexCoord),texture(material.specularMap2,TexCoord), 0.2));

        //Combining
        vec3 PointambientColor = light.ambient * vec3(texture(material.diffuseMap,TexCoord));
        vec3 PointdiffuseColor = diffuse * light.diffuse * vec3(texture(material.diffuseMap,TexCoord));
 
        PointambientColor *= attenuation;
        PointdiffuseColor *= attenuation;
	
	vec3 output = vec3(PointambientColor + PointdiffuseColor);

	return output;
};

/*vec3 SpotLight(vec3 Position,vec3 norm,Material material,Light light,FlashLightVars Flight)
{
        vec3 lightDir = normalize(Flight.lightPos - Position);

	//Checking Shader Intensity for Soft Edges
	float theta = dot(lightDir, normalize(-Flight.spotDir));
	float epsilon = (Flight.spotCutOff - Flight.spotOuterCutOff);
	float intensity = clamp((theta - Flight.spotOuterCutOff) / epsilon, 0.0, 1.0);

	vec3 output;    

        // Do lighting calculations
        //Properties
        vec3 viewDir = normalize(light.viewPos - Position);
        vec3 reflectDir = reflect(-lightDir, norm);

        //Shading
        float diffuse = max(dot(norm, lightDir), 0.0);

        //Attenuation
        float distance = length(light.viewPos - Position);
        float attenuation = 1.0f / (Flight.constant + Flight.linear * distance + Flight.quadratic * (distance * distance));

        //Combining
        vec3 SpotambientColor = light.ambient * vec3(mix(texture(material.diffuseMap1,TexCoord),texture(material.diffuseMap2,TexCoord), 0.2));
        vec3 SpotdiffuseColor = diffuse * light.diffuse * vec3(mix(texture(material.diffuseMap1,TexCoord),texture(material.diffuseMap2,TexCoord), 0.2)) * intensity;
       
        SpotambientColor *= attenuation;
        SpotdiffuseColor *= attenuation;

        //Create output
        output = vec3(SpotambientColor + SpotdiffuseColor);

        return output;
};*/

#define NR_POINT_LIGHTS 10

out vec4 color;

uniform Material material;
uniform Light light;
uniform PointLightVars Plight[NR_POINT_LIGHTS];
uniform FlashLightVars Flight;
uniform DirectionalLightVars Dlight;

void main()
{
        vec3 norm = normalize(Normal);
	
	vec3 output;
	
	//Directional Lighting
	output += DirectionalLight(Position,norm,material,light,Dlight,CameraPos);

	//Point Lights
	for (int i = 0; i < NR_POINT_LIGHTS;++i)
	{
		if ((Plight[i].constant + Plight[i].linear + Plight[i].quadratic) > 1.0E-5)
			output += PointLight(Position,norm,material,light,Plight[i],CameraPos);
	}
	//Spot lights
	//output += SpotLight(Position,norm,material,light,Flight);


	color = vec4(output,1.0f);
};

