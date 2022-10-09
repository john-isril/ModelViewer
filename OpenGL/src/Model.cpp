/*#include "Model.h"
#include "Mesh.h"
#include "Texture.h"
#include "Vertex.h"

Model::Model(const char* path) :
	m_directory{}
{
	LoadModel(path);
}

void Model::Draw(Shader& shader)
{
	for (std::size_t i{ 0 }; i < m_meshes.size(); ++i)
		m_meshes[i].Draw(shader);
}

void Model::LoadModel(const char* path)
{
	Assimp::Importer importer;
	// aiProcess_Triangulate: A postprocessing option that allows assimp to transform its primitive shapes to triangles if it isn't completely made of triangles.
	// 
	const aiScene* scene{ importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs) };

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	m_directory = std::string(path).substr(0, std::string(path).find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);

}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (std::size_t i{ 0 }; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh{ scene->mMeshes[node->mMeshes[i]] }; // retreive the mesh index from the node, then retrieve the mesh from the scene with the mesh index
		m_meshes.push_back(ProcessMesh(mesh, scene));
	}
	
	for (std::size_t i{ 0 }; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices{};
	vertices.reserve(mesh->mNumVertices);

	std::vector<uint32_t> indices{};
	std::vector<Texture> textures{};

	glm::vec3 position{};
	glm::vec3 normal{};
	glm::vec2 texture_coordinates{0.0f, 0.0f};

	for (std::size_t i{ 0 }; i < mesh->mNumVertices; ++i)
	{
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;
		
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			texture_coordinates.x = mesh->mTextureCoords[0][i].x;
			texture_coordinates.y = mesh->mTextureCoords[0][i].y;
		}

		vertices.emplace_back(Vertex{ position, normal, texture_coordinates });
	}

	for (std::size_t i{ 0 }; i < mesh->mNumFaces; ++i)
	{
		const aiFace face{ mesh->mFaces[i] };
		for (std::size_t j{ 0 }; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		const aiMaterial* material{ scene->mMaterials[mesh->mMaterialIndex] };
		
		std::vector<Texture> diffuseMaps{ LoadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::Type::Diffuse) };
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, Texture::Type::Specular);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh{ vertices, indices, textures };
}

std::vector<Texture> Model::LoadMaterialTextures(const aiMaterial* material, aiTextureType type, Texture::Type texture_type)
{
	std::vector<Texture> textures{};
	textures.reserve(material->GetTextureCount(type));

	for (uint32_t i{ 0 }; i < material->GetTextureCount(type); ++i)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		std::string texture_file_path{ m_directory + '/' + std::string(str.C_Str()) };

		textures.emplace_back(texture_file_path.c_str(), true, texture_type);
	}

	return textures;
}
*/