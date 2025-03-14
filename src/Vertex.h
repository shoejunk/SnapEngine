#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <cassert>

/**
 * \struct Vertex
 * \brief Defines the vertex layout for 3D meshes.
 * 
 * This structure defines the vertex format used throughout the engine.
 * It includes position, normal, and texture coordinates for basic 3D rendering.
 */
struct Vertex
{
    glm::vec3 position;  ///< Position in 3D space (x, y, z)
    glm::vec3 normal;    ///< Surface normal vector
    glm::vec2 texCoord;  ///< Texture coordinates (u, v)

    /**
     * \brief Returns the size of the vertex structure in bytes.
     * \return Size in bytes.
     */
    static size_t GetStride()
    {
        return sizeof(Vertex);
    }

    /**
     * \brief Returns the offset of the position attribute.
     * \return Offset in bytes.
     */
    static size_t GetPositionOffset()
    {
        return offsetof(Vertex, position);
    }

    /**
     * \brief Returns the offset of the normal attribute.
     * \return Offset in bytes.
     */
    static size_t GetNormalOffset()
    {
        return offsetof(Vertex, normal);
    }

    /**
     * \brief Returns the offset of the texture coordinate attribute.
     * \return Offset in bytes.
     */
    static size_t GetTexCoordOffset()
    {
        return offsetof(Vertex, texCoord);
    }

    /**
     * \brief Runs unit tests for the Vertex structure.
     */
    static void test()
    {
        std::cout << "[Vertex] Running tests...\n";

        // Test vertex size and offsets
        assert(sizeof(Vertex) == sizeof(glm::vec3) * 2 + sizeof(glm::vec2) && "Vertex size incorrect");
        assert(GetStride() == sizeof(Vertex) && "Vertex stride incorrect");
        
        // Test attribute offsets
        assert(GetPositionOffset() == 0 && "Position offset incorrect");
        assert(GetNormalOffset() == sizeof(glm::vec3) && "Normal offset incorrect");
        assert(GetTexCoordOffset() == sizeof(glm::vec3) * 2 && "TexCoord offset incorrect");

        // Test vertex data alignment
        Vertex v;
        v.position = glm::vec3(1.0f, 2.0f, 3.0f);
        v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        v.texCoord = glm::vec2(0.5f, 0.5f);

        assert(v.position.x == 1.0f && v.position.y == 2.0f && v.position.z == 3.0f && "Position data incorrect");
        assert(v.normal.x == 0.0f && v.normal.y == 1.0f && v.normal.z == 0.0f && "Normal data incorrect");
        assert(v.texCoord.x == 0.5f && v.texCoord.y == 0.5f && "TexCoord data incorrect");

        std::cout << "[Vertex] Tests passed!\n";
    }
};
