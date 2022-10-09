#ifndef SHADER_H
#define SHADER_H

#include <cstdint>
#include <unordered_map>
#include <string>
#include <utility>
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader(const char* file_path);
	~Shader();
	
	void Bind() const;
	void Unbind() const;
	int32_t GetUniformLocation(const char* name);
	inline uint32_t GetID() const { return m_ID; }

	void SetUniform1f(const char* name, float value);
	void SetUniform1i(const char* name, int value);
	void SetUniformMat4f(const char* name, const glm::mat4& value);
	void SetUniformMat3f(const char* name, const glm::mat4& value);
	void SetUniformVec3f(const char* name, const glm::vec3& value);
	// todo add more func overloads

private:
	uint32_t m_ID;
	std::unordered_map<std::string, uint32_t> m_uniform_cache;
	std::pair<std::string, std::string> ParseShader(const char* file_path);

private:
	uint32_t CompileShader(unsigned int shader_type, const char* source);
	uint32_t CreateShader(const char* vertex_shader_source, const char* fragment_shader_source);
};

#endif // !
