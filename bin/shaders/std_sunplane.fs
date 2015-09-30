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

vec4 SourceLighting(vec3 Position,Material material,Light light)
{
        //Properties
	vec4 output;

        //Combining
	vec4 pixColor = vec4(texture(material.diffuseMap,TexCoord));
        vec3 SourceambientColor = light.ambient * vec3(pixColor);
 
        //Combining
	output = vec4(SourceambientColor,pixColor[3]);

        return output;
};

out vec4 color;

uniform Material material;
uniform Light light;

void main()
{
	vec4 output;
	
	//Ambient Lighting
	output = SourceLighting(Position,material,light);

	color = output;
};

