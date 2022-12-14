#pragma once

#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include "Mesh.h"
#include "Texture.h"
#include <unordered_set>
#include "Transform.h"

class Model
{
public:
    Model(const std::string& path = "", const std::string& name = "3D Model");
    Model(const Transform &transform, const std::string& path);
    const std::vector<Mesh>* GetMeshes() const;
    Transform& GetTransform();
    void LoadNewModel(std::string const& path);
    inline const std::string& GetFilePath() const { return m_path; };
    inline const std::string& GetName() const { return m_name; };
    inline void SetFilePath(const std::string& path) { m_path = path; }
    inline void SetName(const std::string& name) { m_name = name; }

private:
    std::unordered_set<std::string> m_loaded_textures_file_names;
    std::string m_path;
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    std::string m_name;
    Transform m_transform;

private:
    void LoadModel(std::string const& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    //Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    //std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, Texture::Type texture_type);
};