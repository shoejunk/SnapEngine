#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

/**
 * \class Shader
 * \brief OpenGL shader program wrapper.
 * 
 * This class manages OpenGL shader programs including:
 * - Loading and compiling vertex/fragment shaders
 * - Setting uniforms
 * - Program activation
 */
class Shader
{
public:
    /**
     * \brief Constructor.
     */
    Shader();

    /**
     * \brief Constructor.
     * \param vertexPath Path to vertex shader file.
     * \param fragmentPath Path to fragment shader file.
     */
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    /**
     * \brief Destructor.
     */
    ~Shader();

    /**
     * \brief Load shader from files.
     * \param vertexPath Path to vertex shader file.
     * \param fragmentPath Path to fragment shader file.
     * \return True if loading succeeded.
     */
    bool LoadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);

    /**
     * \brief Activate the shader program.
     */
    void Use() const;

    /**
     * \brief Get uniform location.
     * \param name Uniform name.
     * \return Uniform location.
     */
    GLint GetUniformLocation(const std::string& name) const;

    /**
     * \brief Set a boolean uniform.
     * \param name Uniform name.
     * \param value Boolean value.
     */
    void SetBool(const std::string& name, bool value) const;

    /**
     * \brief Set an integer uniform.
     * \param name Uniform name.
     * \param value Integer value.
     */
    void SetInt(const std::string& name, int value) const;

    /**
     * \brief Set a float uniform.
     * \param name Uniform name.
     * \param value Float value.
     */
    void SetFloat(const std::string& name, float value) const;

    /**
     * \brief Set a vec3 uniform.
     * \param name Uniform name.
     * \param value Vector value.
     */
    void SetVec3(const std::string& name, const glm::vec3& value) const;

    /**
     * \brief Set a mat4 uniform.
     * \param name Uniform name.
     * \param value Matrix value.
     */
    void SetMat4(const std::string& name, const glm::mat4& value) const;

    /**
     * \brief Get shader program.
     * \return Shader program ID.
     */
    GLuint GetProgram() const { return m_program; }

    /**
     * \brief Check if shader is valid.
     * \return True if shader is valid.
     */
    bool IsValid() const { return m_program != 0; }

    /**
     * \brief Run unit tests for the Shader class.
     */
    static void test();

    /**
     * \brief Set test mode.
     * \param enabled True to enable test mode.
     */
    static void SetTestMode(bool enabled) { s_testMode = enabled; }

    /**
     * \brief Check if test mode is enabled.
     * \return True if test mode is enabled.
     */
    static bool IsTestMode() { return s_testMode; }

private:
    /**
     * \brief Compile shader.
     * \param source Shader source code.
     * \param type Type of shader.
     * \param shader Shader ID.
     * \return True if compilation succeeded.
     */
    bool CompileShader(const std::string& source, GLenum type, GLuint& shader);

    /**
     * \brief Link program.
     * \param vertexShader Vertex shader ID.
     * \param fragmentShader Fragment shader ID.
     * \return True if linking succeeded.
     */
    bool LinkProgram(GLuint vertexShader, GLuint fragmentShader);

    /**
     * \brief Read file.
     * \param path File path.
     * \return File contents.
     */
    std::string ReadFile(const std::string& path);

    GLuint m_program;  ///< OpenGL shader program ID
    static bool s_testMode;  ///< Test mode flag
};
