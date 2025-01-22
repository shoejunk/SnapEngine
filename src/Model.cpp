#include "Model.h"
#include <iostream>
#include <cassert>

// If you installed Assimp via CPM, you can include it directly:
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model()
    : m_isLoaded(false)
{
}

Model::Model(const std::string& filePath)
    : m_isLoaded(false)
{
    LoadFromFile(filePath);
}

bool Model::LoadFromFile(const std::string& filePath)
{
    Assimp::Importer importer;

    // Read the file with some post-processing flags:
    const aiScene* scene = importer.ReadFile(
        filePath,
        aiProcess_Triangulate          |
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenNormals           |
        aiProcess_CalcTangentSpace
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "[Model] Error loading file '" << filePath 
                  << "': " << importer.GetErrorString() << std::endl;
        m_isLoaded = false;
        return false;
    }

    // Clear old data (if any)
    m_meshes.clear();

    // Process the scene (recursive if needed)
    processScene(scene, filePath);

    m_isLoaded = true;
    return true;
}

void Model::processScene(const aiScene* scene, const std::string& /*scenePath*/)
{
    // Typically, you'd walk through the entire node hierarchy.
    // For simplicity, let's just parse every mesh in the scene directly.

    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        processMesh(mesh, scene);
    }
}

void Model::processMesh(const aiMesh* mesh, const aiScene* /*scene*/)
{
    Mesh outMesh;

    // Extract vertex data
    outMesh.vertices.reserve(mesh->mNumVertices * 8); 
    // 3 floats for position, 3 for normal, 2 for UV? Adjust as needed.

    for (unsigned int v = 0; v < mesh->mNumVertices; v++)
    {
        // Position
        outMesh.vertices.push_back(mesh->mVertices[v].x);
        outMesh.vertices.push_back(mesh->mVertices[v].y);
        outMesh.vertices.push_back(mesh->mVertices[v].z);

        // Normal
        if (mesh->mNormals)
        {
            outMesh.vertices.push_back(mesh->mNormals[v].x);
            outMesh.vertices.push_back(mesh->mNormals[v].y);
            outMesh.vertices.push_back(mesh->mNormals[v].z);
        }
        else
        {
            // Fill with dummy normal if not available
            outMesh.vertices.insert(outMesh.vertices.end(), {0.0f, 1.0f, 0.0f});
        }

        // Texture coordinates (Assume 1 set of UV coords)
        if (mesh->mTextureCoords[0])
        {
            outMesh.vertices.push_back(mesh->mTextureCoords[0][v].x);
            outMesh.vertices.push_back(mesh->mTextureCoords[0][v].y);
        }
        else
        {
            // Fill with dummy UV
            outMesh.vertices.insert(outMesh.vertices.end(), {0.0f, 0.0f});
        }
    }

    // Extract indices
    for (unsigned int f = 0; f < mesh->mNumFaces; f++)
    {
        aiFace face = mesh->mFaces[f];
        for (unsigned int i = 0; i < face.mNumIndices; i++)
        {
            outMesh.indices.push_back(face.mIndices[i]);
        }
    }

    m_meshes.push_back(std::move(outMesh));
}

void Model::Draw() const
{
    if (!m_isLoaded)
    {
        std::cerr << "[Model] Cannot Draw() because the model is not loaded.\n";
        return;
    }
    
    // Placeholder: In a real engine, you would bind your GPU buffers and issue draw calls.
    // For demonstration, we just print the data size:
    std::cout << "[Model] Drawing " << m_meshes.size() << " mesh(es):\n";
    for (size_t i = 0; i < m_meshes.size(); i++)
    {
        const auto& mesh = m_meshes[i];
        std::cout << "  Mesh " << i << ": " << mesh.vertices.size() << " vertices, "
                  << mesh.indices.size() << " indices.\n";
    }
}

void Model::test()
{
    std::cout << "[Model] Running tests...\n";

    // 1. Test constructing an empty model
    Model emptyModel;
    assert(!emptyModel.m_isLoaded && "Empty model should not be loaded");

    // 2. Attempt to load a (possibly) invalid file. Expect failure.
    bool loadResult = emptyModel.LoadFromFile("non_existent_file.obj");
    assert(!loadResult && "Loading a non-existent file should fail");

    // 3. If you have a small test model available, place it in your repository
    // and try loading it here:
    bool loadReal = emptyModel.LoadFromFile("test_assets/box.obj");
    assert(loadReal && "Failed to load test model");
    emptyModel.Draw();  // Should print out mesh details

    std::cout << "[Model] Tests passed!\n";
}
