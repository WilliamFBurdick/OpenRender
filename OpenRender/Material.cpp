#include "Material.h"
#include <fstream>
#include <cstdio>
#include "Utility.h"

const std::string TEXTURES_PATH = "resources/textures/";

Material::Material()
{
    mTextured = false;
    mShininess = 0.0f;
    mAmbient = glm::vec3(0.0f);
    mDiffuse = glm::vec3(0.0f);
    mSpecular = glm::vec3(0.0f);
}

void Material::SetShininess(GLfloat shininess)
{
    mShininess = shininess;
}

void Material::SetAmbient(const glm::vec3& ambient)
{
    mAmbient.x = ambient.x;
    mAmbient.y = ambient.y;
    mAmbient.z = ambient.z;
}

void Material::SetDiffuse(const glm::vec3& diffuse)
{
    mDiffuse.x = diffuse.x;
    mDiffuse.y = diffuse.y;
    mDiffuse.z = diffuse.z;
}

void Material::SetSpecular(const glm::vec3& specular)
{
    mSpecular.x = specular.x;
    mSpecular.y = specular.y;
    mSpecular.z = specular.z;
}

void Material::SendToShader(const Shader& shader) const
{
    shader.SetFloat("material.shininess", mShininess);
    shader.SetVec3("material.ambient", mAmbient);
    shader.SetVec3("material.diffuse", mDiffuse);
    shader.SetVec3("material.specular", mSpecular);
}

bool Material::Load(const std::string& path, std::vector<Material>& materials)
{
    std::ifstream iFile("resources/materials/" + path, std::ios_base::in);
    if (!iFile)
    {
        printf("Couldn't find .mat file at path: %s\n", path.c_str());
        return false;
    }

    std::string line;
    std::vector<std::string> substrings;

    while (!iFile.eof())
    {
        std::getline(iFile, line);
        substrings.clear();

        if (!line.empty() && line[0] != '#')
        {
            Utility::ParseString(line, substrings, ' ');

            // New material group
            if (substrings[0] == "newmtl")
            {
                materials.push_back(Material());
                materials.back().mName = substrings[1];
                continue;
            }

            // Read ambient data
            if (substrings[0] == "Ka")
            {
                materials.back().SetAmbient(glm::vec3(std::stof(substrings[1]), std::stof(substrings[2]), std::stof(substrings[3])));
                continue;
            }

            // Read diffuse data
            if (substrings[0] == "Kd")
            {
                materials.back().SetDiffuse(glm::vec3(std::stof(substrings[1]), std::stof(substrings[2]), std::stof(substrings[3])));
                continue;
            }

            // Read specular data
            if (substrings[0] == "Ks")
            {
                materials.back().SetSpecular(glm::vec3(std::stof(substrings[1]), std::stof(substrings[2]), std::stof(substrings[3])));
                continue;
            }

            // Read specular exponent (shininess)
            if (substrings[0] == "Ns")
            {
                materials.back().SetShininess(std::stof(substrings[1]));
                continue;
            }

            // Read ambient texture file
            if (substrings[0] == "map_Ka")
            {
                materials.back().mAmbientMap.Load(TEXTURES_PATH + substrings[1]);
                materials.back().mTextured = true;
                continue;
            }

            // Read diffuse texture file
            if (substrings[0] == "map_Kd")
            {
                materials.back().mDiffuseMap.Load(TEXTURES_PATH + substrings[1]);
                materials.back().mTextured = true;
                continue;
            }

            // Read specular texture file
            if (substrings[0] == "map_Ks")
            {
                materials.back().mSpecularMap.Load(TEXTURES_PATH + substrings[1]);
                materials.back().mTextured = true;
                continue;
            }

            // Read normal texture file
            if (substrings[0] == "map_Ns")
            {
                materials.back().mNormalMap.Load(TEXTURES_PATH + substrings[1]);
                materials.back().mTextured = true;
                continue;
            }
        }
    }
    iFile.close();
    return true;
}