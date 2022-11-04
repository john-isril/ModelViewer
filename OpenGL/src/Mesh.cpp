#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include <string>

void Mesh::Setup()
{
	m_VAO.AddBufferLayoutMeshVertex(m_VBO);
	m_VAO.Unbind();
}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::vector<Texture>& textures) :
    m_vertices{ vertices }, m_indices{ indices }, m_textures{ textures },
    m_VAO{}, m_VBO{ &vertices[0], vertices.size() * sizeof(Vertex) }, m_IBO{ &indices[0], m_indices.size() }
{

    this->Setup();
}

Mesh::Mesh(const Mesh& mesh) :
    m_VAO{}
{
    m_vertices.reserve(mesh.m_vertices.size());
    m_indices.reserve(mesh.m_indices.size());
    m_textures.reserve(mesh.m_textures.size());

    for (size_t i{ 0 }; i < mesh.m_vertices.size(); ++i)
    {
        m_vertices.push_back(mesh.m_vertices[i]);
    }
    
    for (size_t i{ 0 }; i < mesh.m_indices.size(); ++i)
    {
        m_indices.push_back(mesh.m_indices[i]);
    }
    
    for (size_t i{ 0 }; i < mesh.m_textures.size(); ++i)
    {
        m_textures.push_back(mesh.m_textures[i]);
    }
    m_VBO.BindBufferData((void*)(&m_vertices[0]), m_vertices.size() * sizeof(Vertex));
    m_IBO.BindBufferData(&m_indices[0], m_indices.size());

    this->Setup();
}

Mesh::Mesh(aiMesh* mesh, const aiScene* scene, std::unordered_set<std::string>& loaded_textures_file_names, const std::string& directory) :
    m_vertices{}, m_indices{}, m_textures{}, m_VAO{}, m_VBO{}, m_IBO{}
{
    m_vertices.reserve(mesh->mNumVertices);

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

        m_vertices.emplace_back(position, normal, texture_coordinates, tangent, bitangent);
    }

    for (std::size_t i{ 0 }; i < mesh->mNumFaces; ++i)
    {
        aiFace face{ mesh->mFaces[i] };

        for (size_t j{ 0 }; j < face.mNumIndices; ++j)
            m_indices.push_back(face.mIndices[j]);
    }

    aiMaterial* material{ scene->mMaterials[mesh->mMaterialIndex] };

    size_t total_texture_count{ material->GetTextureCount(aiTextureType_DIFFUSE) +
        material->GetTextureCount(aiTextureType_SPECULAR) +
        material->GetTextureCount(aiTextureType_HEIGHT) +
        material->GetTextureCount(aiTextureType_AMBIENT) +
        material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS)
    };

    m_textures.reserve(total_texture_count);

    LoadMaterialTextures(material, aiTextureType_DIFFUSE, loaded_textures_file_names, Texture::Type::Diffuse, directory);
    LoadMaterialTextures(material, aiTextureType_SPECULAR, loaded_textures_file_names, Texture::Type::Specular, directory);
    LoadMaterialTextures(material, aiTextureType_HEIGHT, loaded_textures_file_names, Texture::Type::Normal, directory);
    LoadMaterialTextures(material, aiTextureType_AMBIENT, loaded_textures_file_names, Texture::Type::Height, directory);
    LoadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, loaded_textures_file_names, Texture::Type::Roughness, directory);
    LoadMaterialTextures(material, aiTextureType_OPACITY, loaded_textures_file_names, Texture::Type::Alpha, directory);

    m_VBO.BindBufferData(&m_vertices[0], m_vertices.size() * sizeof(Vertex));
    m_IBO.BindBufferData(&m_indices[0], m_indices.size());

    this->Setup();
}

void Mesh::Bind() const
{
    m_VAO.Bind();
}

void Mesh::Unbind() const
{
    m_VAO.Unbind();
}

void Mesh::LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::unordered_set<std::string>& loaded_textures_file_names, Texture::Type texture_type, const std::string& directory)
{
    for (size_t i{ 0 }; i < material->GetTextureCount(type); ++i)
    {
        aiString file_name_ai;
        material->GetTexture(type, i, &file_name_ai);
        std::string file_name{ std::string(file_name_ai.C_Str()) };

        bool found{ (loaded_textures_file_names.find(file_name) != loaded_textures_file_names.end()) };

        if (loaded_textures_file_names.empty() || !found)
        {
            std::string file_path{ directory + '/' + file_name };
            m_textures.emplace_back(file_path, file_name, texture_type, true);
            loaded_textures_file_names.insert(file_name);
        }
    }
}

const std::vector<Vertex>* Mesh::GetVertices() const
{
    return &m_vertices;
}

const std::vector<uint32_t>* Mesh::GetIndices() const
{
    return &m_indices;
}

const std::vector<Texture>* Mesh::GetTextures() const
{
    return &m_textures;
}
