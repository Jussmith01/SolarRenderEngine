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

vec3 SourceLighting(vec3 Position,Material material,Light light)
{
        //Properties
	vec3 output;

        //Combining
        vec3 SourceambientColor = light.ambient * vec3(texture(material.diffuseMap,TexCoord));
 
        //Combining
	output = vec3(SourceambientColor);

        return output;
};

out vec4 color;

uniform Material material;
uniform Light light;

void main()
{
	vec3 output;
	
	//Ambient Lighting
	output = SourceLighting(Position,material,light);

	color = vec4(output,1.0f);
};

