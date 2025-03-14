#pragma once

#include <string>
#include <vector>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Texture.h"

/**
 * \class Model
 * \brief A class to load and render 3D models.
 */
class Model
{
public:
    /**
     * \brief Constructor.
     */
    Model() = default;

    /**
     * \brief Destructor.
     */
    ~Model() = default;

    /**
     * \brief Load model from file.
     * \param filePath Path to model file.
     * \return True if loading was successful.
     */
    bool LoadFromFile(const std::string& filePath);

    /**
     * \brief Draw the model.
     */
    void Draw() const;

    /**
     * \brief Run unit tests for Model class.
     */
    static void test();

    /**
     * \brief Enable or disable test mode.
     * \param enabled Whether to enable test mode.
     */
    static void SetTestMode(bool enabled) { s_testMode = enabled; }

    /**
     * \brief Check if test mode is enabled.
     * \return Whether test mode is enabled.
     */
    static bool IsTestMode() { return s_testMode; }

private:
    /**
     * \brief Process an Assimp node.
     * \param node The node to process.
     * \param scene The Assimp scene.
     */
    void processNode(aiNode* node, const aiScene* scene);

    /**
     * \brief Process an Assimp mesh.
     * \param mesh The mesh to process.
     * \param scene The Assimp scene.
     * \return The processed mesh.
     */
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    /**
     * \brief Load textures from material.
     * \param material The material to load textures from.
     * \param type The type of textures to load.
     * \param typeName The name of the texture type.
     * \return Vector of loaded textures.
     */
    std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName);

    /**
     * \brief Load texture from file.
     * \param path Path to texture file.
     * \param directory Directory containing texture file.
     * \return OpenGL texture ID.
     */
    unsigned int TextureFromFile(const char* path, const std::string& directory);

    std::string m_directory;                  ///< Directory containing model files
    std::vector<Mesh> m_meshes;              ///< Model meshes
    std::vector<Texture> m_loadedTextures;    ///< Loaded textures

    static bool s_testMode;                   ///< Test mode flag
};
