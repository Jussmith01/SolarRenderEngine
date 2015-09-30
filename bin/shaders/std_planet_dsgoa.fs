#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in float Time;
in vec3 CameraPos;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D glowMap;
    sampler2D overlayMap;
    sampler2D alphaoverlayMap;
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

vec3 DirectionalLight(vec3 Position,vec3 norm,Material material,Light light,DirectionalLightVars Dlight,float time,vec3 cPos)
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
	float offset = abs(sin(time * 0.0016));
	vec2 OLTexCoord = vec2(TexCoord.x - offset,TexCoord.y);

	vec3 aolTex = vec3(texture(material.alphaoverlayMap,OLTexCoord));
	float Mix = (aolTex.x + aolTex.y + aolTex.z) / 3.0f;

        vec3 PointambientColor = light.ambient * 
				vec3(mix(texture(material.overlayMap,OLTexCoord),texture(material.diffuseMap,TexCoord), Mix));
        vec3 PointdiffuseColor = diffuse * light.diffuse *
				vec3(mix(texture(material.overlayMap,OLTexCoord),texture(material.diffuseMap,TexCoord), Mix));	

        vec3 greyscaleMap = vec3(texture(material.specularMap,TexCoord));
        float specIntensity = (greyscaleMap.x + greyscaleMap.y + greyscaleMap.z)/3.0f;
        vec3 PointspecularColor = specular * light.specular * specIntensity * 
					vec3(texture(material.diffuseMap,TexCoord));  //* light.intensity;

	float pixIten = (PointspecularColor.x + PointspecularColor.y + PointspecularColor.z) / 3.0f;
	if (pixIten > 1.0f)
		{pixIten = 1.0f;}

	vec3 overlay = pixIten * vec3(texture(material.overlayMap,OLTexCoord));

	PointspecularColor = vec3(mix(overlay,PointspecularColor, Mix));

	output = vec3(PointambientColor + PointdiffuseColor + PointspecularColor);
	//output = vec3(PointambientColor + PointdiffuseColor);

        return output;
};

vec3 GlowTex(vec3 output,Material material)
{
	float L = length(output);
	float ths = 0.15f;

	vec3 opt;
	if (L < ths)
	{
		opt = (ths - L)/ths * vec3(texture(material.glowMap,TexCoord));
		float gL = length(opt);
		opt = 0.75 * gL * gL * opt;
	}

	return opt;
};

out vec4 color;

uniform Material material;
uniform Light light;
uniform DirectionalLightVars Dlight;

void main()
{
        vec3 norm = normalize(Normal);
	
	vec3 output;
	
	//Directional Lighting
	output = DirectionalLight(FragPos,norm,material,light,Dlight,Time,CameraPos);

	//Apply Glow Tex
	output += GlowTex(output,material);

	color = vec4(output,1.0f);
};

