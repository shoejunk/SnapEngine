#pragma once

#include <GL/glew.h>
#include <vector>
#include <string>
#include "Vertex.h"
#include "Texture.h"

/**
 * \struct Mesh
 * \brief A mesh containing vertex and index data.
 */
struct Mesh
{
    std::vector<Vertex> vertices;      ///< Vertex data
    std::vector<unsigned int> indices; ///< Index data
    std::vector<Texture> textures;     ///< Texture data

    // OpenGL buffer handles
    mutable GLuint vao = 0;  ///< Vertex Array Object
    mutable GLuint vbo = 0;  ///< Vertex Buffer Object
    mutable GLuint ebo = 0;  ///< Element Buffer Object

    /**
     * \brief Default constructor.
     */
    Mesh() = default;

    /**
     * \brief Constructor.
     * \param vertices Vector of vertices.
     * \param indices Vector of indices.
     * \param textures Vector of textures.
     */
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
        : vertices(vertices), indices(indices), textures(textures)
    {
        setupMesh();
    }

    /**
     * \brief Move constructor.
     */
    Mesh(Mesh&& other) noexcept
        : vertices(std::move(other.vertices))
        , indices(std::move(other.indices))
        , textures(std::move(other.textures))
        , vao(other.vao)
        , vbo(other.vbo)
        , ebo(other.ebo)
    {
        other.vao = 0;
        other.vbo = 0;
        other.ebo = 0;
    }

    /**
     * \brief Move assignment operator.
     */
    Mesh& operator=(Mesh&& other) noexcept
    {
        if (this != &other)
        {
            // Clean up existing resources
            cleanup();

            // Move resources
            vertices = std::move(other.vertices);
            indices = std::move(other.indices);
            textures = std::move(other.textures);
            vao = other.vao;
            vbo = other.vbo;
            ebo = other.ebo;

            // Clear other's resources
            other.vao = 0;
            other.vbo = 0;
            other.ebo = 0;
        }
        return *this;
    }

    /**
     * \brief Destructor.
     */
    ~Mesh()
    {
        cleanup();
    }

    // Prevent copying (since we have OpenGL resources)
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    /**
     * \brief Draw the mesh.
     */
    void Draw() const;

private:
    /**
     * \brief Clean up OpenGL resources.
     */
    void cleanup()
    {
        if (vao != 0) glDeleteVertexArrays(1, &vao);
        if (vbo != 0) glDeleteBuffers(1, &vbo);
        if (ebo != 0) glDeleteBuffers(1, &ebo);
    }

    /**
     * \brief Set up mesh buffers.
     */
    void setupMesh();
};
