#include "Model.h"

#include <iostream>
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer.h"

Model::Model(std::string const& path, bool gamma) :
    m_meshes{}, m_directory{}, m_gamma_correction{ gamma }
{
    LoadModel(path);
}

void Model::Draw(Renderer& renderer, Shader& shader)
{
    for (size_t i{ 0 }; i < m_meshes.size(); ++i)
        renderer.DrawMesh(m_meshes[i], shader);
}

void Model::LoadModel(std::string const& path)
{
    Assimp::Importer importer;
    const aiScene* scene{ importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace) };

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    m_directory = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (size_t i{ 0 }; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh{ scene->mMeshes[node->mMeshes[i]] };
        m_meshes.push_back(ProcessMesh(mesh, scene));
    }

    for (size_t i{ 0 }; i < node->mNumChildren; ++i)
    {
        ProcessNode(node->mChildren[i], scene);
    }

}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices{};
    vertices.reserve(mesh->mNumVertices);
    std::vector<std::uint32_t> indices{};
    std::vector<Texture> textures{};

    for (size_t i{ 0 }; i < mesh->mNumVertices; ++i)
    {
        glm::vec3 position{};
        glm::vec3 normal{};
        glm::vec2 texture_coordinates{};
        glm::vec3 tangent{};
        glm::vec3 bitangent{};

        position.x = mesh->mVertices[i].x;
        position.y = mesh->mVertices[i].y;
        position.z = mesh->mVertices[i].z;

        if (mesh->HasNormals())
        {
            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;
        }

        // texture coordinates
        if (mesh->mTextureCoords[0])
        {
            texture_coordinates.x = mesh->mTextureCoords[0][i].x;
            texture_coordinates.y = mesh->mTextureCoords[0][i].y;

            // tangent
            tangent.x = mesh->mTangents[i].x;
            tangent.y = mesh->mTangents[i].y;
            tangent.z = mesh->mTangents[i].z;

            // bitangent
            bitangent.x = mesh->mBitangents[i].x;
            bitangent.y = mesh->mBitangents[i].y;
            bitangent.z = mesh->mBitangents[i].z;
        }
        else
        {
            texture_coordinates = glm::vec2(0.0f, 0.0f);
        }

        vertices.emplace_back(position, normal, texture_coordinates, tangent, bitangent);
    }

    for (std::size_t i{ 0 }; i < mesh->mNumFaces; ++i)
    {
        aiFace face{ mesh->mFaces[i] };

        for (size_t j{ 0 }; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial* material{ scene->mMaterials[mesh->mMaterialIndex] };

    size_t total_texture_count{ material->GetTextureCount(aiTextureType_DIFFUSE) *
        material->GetTextureCount(aiTextureType_SPECULAR) *
        material->GetTextureCount(aiTextureType_HEIGHT) *
        material->GetTextureCount(aiTextureType_AMBIENT) *
        material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS)
    };

    textures.reserve(total_texture_count);

    std::vector<Texture> diffuse_maps{ LoadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::Type::Diffuse) };
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

    std::vector<Texture> specular_maps{ LoadMaterialTextures(material, aiTextureType_SPECULAR, Texture::Type::Specular) };
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

    std::vector<Texture> normal_maps{ LoadMaterialTextures(material, aiTextureType_HEIGHT, Texture::Type::Normal) };
    textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());

    std::vector<Texture> height_maps{ LoadMaterialTextures(material, aiTextureType_AMBIENT, Texture::Type::Height) };
    textures.insert(textures.end(), height_maps.begin(), height_maps.end());

    std::vector<Texture> roughness_maps{ LoadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS , Texture::Type::Roughness) };
    textures.insert(textures.end(), roughness_maps.begin(), roughness_maps.end());

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, Texture::Type texture_type)
{
    std::vector<Texture> textures{};
    textures.reserve(material->GetTextureCount(type));

    for (size_t i{ 0 }; i < material->GetTextureCount(type); ++i)
    {
        aiString file_name_ai;
        material->GetTexture(type, i, &file_name_ai);
        std::string file_name{ std::string(file_name_ai.C_Str()) };

        bool found{ (m_loaded_textures_file_names.find(file_name) != m_loaded_textures_file_names.end()) };

        if (m_loaded_textures_file_names.empty() || !found)
        {
            std::string file_path{ m_directory + '/' + file_name };
            textures.emplace_back(file_path.c_str(), true, texture_type, file_name);
            m_loaded_textures_file_names.insert(file_name);
        }
    }

    return textures;
}

const std::vector<Mesh>* Model::GetMeshes() const
{
    return &m_meshes;
}
