#include "Model.h"

#include <iostream>
#include "Renderer.h"

Model::Model(const std::string& path, const std::string& name) :
    m_name{ name }, m_meshes {}, m_directory{}, m_transform{}
{
    LoadModel(path);
}

Model::Model(const Transform& transform, const std::string& path) :
    m_transform{ transform }, m_meshes{}, m_directory{}
{
    LoadModel(path);
}

void Model::LoadModel(std::string const& path)
{
    if (m_path == path)
    {
        std::cerr << "Model already loaded!" << std::endl;
        return;
    }

    m_path = path;
    Assimp::Importer importer;
    const aiScene* scene{ importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace) };

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    m_directory = path.substr(0, path.find_last_of('/'));
    m_meshes.reserve(scene->mNumMeshes);
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (size_t i{ 0 }; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh{ scene->mMeshes[node->mMeshes[i]] };
        m_meshes.emplace_back(mesh, scene, m_loaded_textures_file_names, m_directory);
    }

    for (size_t i{ 0 }; i < node->mNumChildren; ++i)
    {
        ProcessNode(node->mChildren[i], scene);
    }

}

const std::vector<Mesh>* Model::GetMeshes() const
{
    return &m_meshes;
}

Transform& Model::GetTransform()
{
    return m_transform;
}

void Model::LoadNewModel(std::string const& path)
{
    if (m_path == path)
    {
        std::cerr << "Model already loaded!" << std::endl;
        return;
    }
    m_meshes.clear();
    this->LoadModel(path);
}
