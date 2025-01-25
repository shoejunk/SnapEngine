#### **API Documentation**

1. **Window Class**  
   - **Purpose**: Manages a Win32 window.  
   - **Public API**:  
     ```cpp
     Window(const std::string& title, int width, int height);
     bool Create();
     bool ProcessMessages();
     HWND GetHandle(); // Optional
     static void test();
     ```

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
     void createObjects() override;
     ```

4. **DataManager Class**  
   - **Purpose**: Manages multiple managers and loads data from JSON files.  
   - **Public API**:  
     ```cpp
     DataManager(const std::string& filename);
     bool LoadData();
     void CreateManagedObjects();
     ```

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
     - **Usage Example**:  
       ```cpp
       Model model("path/to/model.obj");
       if (model.LoadFromFile("path/to/another_model.obj")) {
           model.Draw();
       }
       ```

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

