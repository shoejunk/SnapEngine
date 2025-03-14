#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

// Initialize static members
bool Shader::s_testMode = false;

Shader::Shader()
    : m_program(0)
{
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
    : m_program(0)
{
    LoadFromFiles(vertexPath, fragmentPath);
}

Shader::~Shader()
{
    if (m_program != 0)
    {
        glDeleteProgram(m_program);
    }
}

bool Shader::LoadFromFiles(const std::string& vertexPath, const std::string& fragmentPath)
{
    // Read shader source files
    std::string vertexCode = ReadFile(vertexPath);
    std::string fragmentCode = ReadFile(fragmentPath);
    if (vertexCode.empty() || fragmentCode.empty())
    {
        return false;
    }

    GLuint vertexShader = 0, fragmentShader = 0;

    // Compile shaders
    if (!CompileShader(vertexCode, GL_VERTEX_SHADER, vertexShader) ||
        !CompileShader(fragmentCode, GL_FRAGMENT_SHADER, fragmentShader))
    {
        return false;
    }

    // Link program
    if (!LinkProgram(vertexShader, fragmentShader))
    {
        return false;
    }

    // Delete shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

void Shader::Use() const
{
    glUseProgram(m_program);
}

GLint Shader::GetUniformLocation(const std::string& name) const
{
    return glGetUniformLocation(m_program, name.c_str());
}

void Shader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(GetUniformLocation(name), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

bool Shader::CompileShader(const std::string& source, GLenum type, GLuint& shader)
{
    shader = glCreateShader(type);
    const char* sourcePtr = source.c_str();
    glShaderSource(shader, 1, &sourcePtr, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        std::cerr << "Shader compilation error (" 
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") 
                  << "): " << infoLog << std::endl;
        glDeleteShader(shader);
        return false;
    }
    return true;
}

bool Shader::LinkProgram(GLuint vertexShader, GLuint fragmentShader)
{
    // Delete old program if it exists
    if (m_program != 0)
    {
        glDeleteProgram(m_program);
    }

    // Create and link new program
    m_program = glCreateProgram();
    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);
    glLinkProgram(m_program);

    GLint success;
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1024];
        glGetProgramInfoLog(m_program, sizeof(infoLog), NULL, infoLog);
        std::cerr << "Shader program linking error: " << infoLog << std::endl;
        glDeleteProgram(m_program);
        m_program = 0;
        return false;
    }
    return true;
}

std::string Shader::ReadFile(const std::string& path)
{
    std::string code;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        file.open(path);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        code = stream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "Error reading shader file '" << path << "': " << e.what() << std::endl;
        return "";
    }
    return code;
}

void Shader::test()
{
    std::cout << "\nRunning Shader tests...\n";

    // Skip shader compilation in test mode
    if (s_testMode)
    {
        std::cout << "Shader tests skipped in test mode\n";
        return;
    }

    // Test shader compilation
    Shader shader;
    bool success = shader.LoadFromFiles("shaders/basic.vert", "shaders/basic.frag");
    assert(success && "Failed to load shader files");

    // Test uniform operations
    shader.Use();
    GLint location = shader.GetUniformLocation("model");
    assert(location != -1 && "Failed to get uniform location");

    std::cout << "Shader tests passed!\n";
}
