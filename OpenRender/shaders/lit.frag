#version 460 core
out vec4 FragColor;

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material
{
	float shininess;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec4 MyColor;
in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

uniform Light light;
uniform Material material;
uniform vec3 viewPos;

uniform sampler2D texture1;
uniform bool bTextured;

void main()
{
	// Properties
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	vec3 viewDir = normalize(viewPos - FragPos);

	// diffuse
	float diff = max(dot(norm, lightDir), 0.0);

	// specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * material.ambient;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;

	vec4 textureColor = vec4(1.0);
	if (bTextured)
	{
		textureColor = texture(texture1, TexCoords);
	}

	FragColor = vec4(ambient + diffuse + specular, 1.0) * MyColor * textureColor;
}