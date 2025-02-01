#### **API Documentation**

1. **Window Class**  
   - **Purpose**: Manages a GLFW/OpenGL window.  
   - **Public API**:  
     ```cpp
     Window(const std::string& title, int width, int height);
     bool Create();
     bool ProcessMessages();
     int GetWidth() const;
     int GetHeight() const;
     static void test();
     ```
   - **Notes**:
     - Uses GLFW for window management and OpenGL for rendering
     - Automatically handles window resizing and input events
     - Supports modern OpenGL (4.6) core profile

2. **ManagerBase Class**  
   - **Purpose**: Abstract base class for managing JSON objects.  
   - **Public API**:  
     ```cpp
     void addJsonObject(const nlohmann::json& obj);
     const std::vector<nlohmann::json>& getJsonObjects() const;
     virtual void createObjects() = 0;
     ```

3. **WindowManager Class**  
   - **Purpose**: Manages multiple windows using JSON data.  
   - **Public API**:  
     ```cpp
     bool Initialize();  // Initializes GLFW globally
     void createObjects() override;  // Creates windows from JSON data
     bool ProcessMessages();  // Updates all windows
     const std::vector<std::unique_ptr<Window>>& GetWindows() const;
     static void test();
     ```
   - **Notes**:
     - Handles GLFW initialization and shutdown
     - Creates windows based on JSON configuration
     - Supports multiple windows

4. **DataManager Class**  
   - **Purpose**: Manages multiple managers and loads data from JSON files.  
   - **Public API**:  
     ```cpp
     DataManager(const std::string& filename = "snapengine_data.json");
     bool Initialize();
     bool LoadData();
     void CreateManagedObjects();
     WindowManager& GetWindowManager();
     static void test();
     ```
   - **Notes**:
     - Loads window configuration from JSON file
     - Routes JSON objects to appropriate managers
     - Manages the lifecycle of all engine objects

5. **Model Class**  
   - **Purpose**: Handles 3D models using Assimp.  
   - **Public API**:  
     ```cpp
     Model();
     Model(const std::string& filePath);
     bool LoadFromFile(const std::string& filePath);
     void Draw() const;
     size_t GetMeshCount() const;
     const std::vector<Mesh>& GetMeshes() const;
     static void test();
     ```
   - **Notes**:
     - Uses Assimp for model loading
     - Supports various 3D file formats
     - Will use OpenGL for rendering (not yet implemented)

6. **Mesh Class**  
   - **Purpose**: Encapsulates GPU buffers (vertex, index) for a 3D mesh.  
   - **Public API**:  
     ```cpp
     Mesh();
     ~Mesh();
     bool CreateFromModelPart(ID3D11Device* device, const Model::Mesh& srcMeshData);
     void Draw(ID3D11DeviceContext* context) const;
     static void test();
     ```
   - **Usage Example**:  
     ```cpp
     Mesh mesh;
     if (mesh.CreateFromModelPart(device, model.GetMeshes().front())) {
         mesh.Draw(context);
     }
     ```

7. **D3D11Renderer Class**  
   - **Purpose**: Implements a basic Direct3D 11 renderer for SnapEngine.  
   - **Public API**:  
     ```cpp
     D3D11Renderer();
     ~D3D11Renderer();
     bool Initialize(void* windowHandle) override;
     void BeginFrame() override;
     void DrawModel(const Model& model) override;
     void EndFrame() override;
     void UpdateConstantBuffer(const DirectX::XMMATRIX& worldViewProj);
     static void test();
     ```
   - **Usage Example**:  
     ```cpp
     D3D11Renderer renderer;
     if (renderer.Initialize(windowHandle)) {
         renderer.BeginFrame();
         renderer.DrawModel(model);
         renderer.EndFrame();
     }
     ```

#### **JSON Configuration**
The engine uses JSON files for configuration. Here's an example window configuration:
```json
[
    {
        "class": "window",
        "title": "Main Game Window",
        "width": 1280,
        "height": 720
    }
]
