#pragma once

#include <string>
#include <vector>
#include <memory>

/// \class Model
/// \brief Stores and renders a 3D model loaded from file.
///
/// This class loads mesh data (vertices, indices, etc.) from a file using
/// Assimp. The `Draw()` method is a placeholder and should be adapted to your
/// chosen graphics API (e.g. DirectX, OpenGL, Vulkan).
class Model
{
public:
    /// \brief Creates an empty model.
    Model();

    /// \brief Constructs and immediately loads a model from a file.
    /// \param filePath The path to the 3D model file (e.g., .obj, .fbx).
    Model(const std::string& filePath);

    /// \brief Loads a model from the specified file path using Assimp.
    /// \param filePath The path to the 3D model file.
    /// \return True if loading was successful, false otherwise.
    bool LoadFromFile(const std::string& filePath);

    /// \brief Draws the model.
    /// 
    /// This is a placeholder function. In a real engine, you'd integrate this
    /// with DirectX, OpenGL, or another rendering API to submit mesh data to
    /// the GPU.
    void Draw() const;

    /// \brief Runs the test suite for the Model class.
    ///
    /// Loads a small test model (if available), checks for valid data, etc.
    static void test();

private:
    /// \brief Internal struct for holding mesh data.
    struct Mesh
    {
        std::vector<float>   vertices;  ///< Interleaved vertex data (pos, normal, uv, etc.)
        std::vector<unsigned> indices;  ///< Index buffer
        // Add more as needed (normals, tangents, material info, etc.)
    };

    /// \brief Processes an Assimp scene recursively, extracting mesh data.
    /// \param scene The loaded Assimp scene.
    /// \param scenePath The file path (useful for relative resources).
    void processScene(const struct aiScene* scene, const std::string& scenePath);

    /// \brief Processes a single Assimp mesh and stores it internally.
    /// \param mesh The mesh data from Assimp.
    /// \param scene The parent scene (for materials, etc.).
    void processMesh(const struct aiMesh* mesh, const struct aiScene* scene);

private:
    std::vector<Mesh> m_meshes;
    bool              m_isLoaded;
};
