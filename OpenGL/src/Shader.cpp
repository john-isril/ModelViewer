#include "Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

typedef struct ShaderProgramSource
{
    std::string vertex_shader_source{};
    std::string fragment_shader_source{};

} ShaderProgramSource;

Shader::Shader(const char* file_path)
    : m_ID{}, m_uniform_cache {}
{
    auto [vertex_shader_source, fragment_shader_source] = ParseShader(file_path);
    m_ID = CreateShader(vertex_shader_source.c_str(), fragment_shader_source.c_str());
}

Shader::~Shader()
{
    glDeleteProgram(m_ID);
}

void Shader::Bind() const
{
    glUseProgram(m_ID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

int32_t Shader::GetUniformLocation(const char* name)
{
    if (const auto it{ m_uniform_cache.find(name) }; it != m_uniform_cache.end())
    {
        return it->second;
    }

    int32_t found_uniform{ glGetUniformLocation(m_ID, name)};

    if (found_uniform == -1)
    {
        std::cerr << "ERROR: " << name << " UNIFORM NOT FOUND!\n";
        return -1;
    }

    m_uniform_cache[name] = found_uniform;

    return found_uniform;
}

void Shader::SetUniform1f(const char* name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform1i(const char* name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniformMat4f(const char* name, const glm::mat4& value)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetUniformMat3f(const char* name, const glm::mat4& value)
{
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetUniformVec3f(const char* name, const glm::vec3& value)
{
    glUniform3fv(GetUniformLocation(name), 1, &value[0]);
}

void Shader::SetUniformVec4f(const char* name, const glm::vec4& value)
{
    glUniform4fv(GetUniformLocation(name), 1, &value[0]);
}

void Unbind()
{
    glUseProgram(0);
}

std::pair<std::string, std::string> Shader::ParseShader(const char* file_path)
{
    std::ifstream file_stream{ file_path };
    std::stringstream ss[2]{};
    std::string line{};

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderType type{ ShaderType::NONE };

    while (std::getline(file_stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else if (type != ShaderType::NONE)
        {
            ss[(int)type] << line << '\n';
        }
    }

    file_stream.close();

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int shader_type, const char* source)
{
    unsigned int shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int compile_success {};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_success);

    char info_log[212];

    if (!compile_success)
    {
        glGetShaderInfoLog(shader, 212, NULL, info_log);
        std::cerr << "ERROR::SHADER::" << ((shader_type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << info_log << "\n";
        glDeleteShader(shader);

        return 0;
    }

    return shader;
}

uint32_t Shader::CreateShader(const char* vertex_shader_source, const char* fragment_shader_source)
{
    uint32_t shader_program = glCreateProgram();
    uint32_t vertex_shader = CompileShader(GL_VERTEX_SHADER, vertex_shader_source);
    uint32_t fragment_shader = CompileShader(GL_FRAGMENT_SHADER, fragment_shader_source);

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glValidateProgram(shader_program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}