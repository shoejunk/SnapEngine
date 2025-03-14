#pragma once

#include <string>

/**
 * \struct Texture
 * \brief A structure to hold texture data.
 */
struct Texture
{
    unsigned int id;     ///< OpenGL texture ID
    std::string type;    ///< Texture type (e.g., "texture_diffuse")
    std::string path;    ///< Path to texture file
};
