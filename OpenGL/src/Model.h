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

class Model
{
private:
    std::unordered_set<std::string> m_loaded_textures_file_names;
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    bool m_gamma_correction;

private:
    void LoadModel(std::string const& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, Texture::Type texture_type);

public:

public:
    Model(std::string const& path, bool gamma = false);
    void Draw(class Renderer &renderer, Shader& shader);

    const std::vector<Mesh>* GetMeshes() const;
};