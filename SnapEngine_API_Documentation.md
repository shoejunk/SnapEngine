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
     static void SetTestMode(bool enabled);
     static void test();
     ```
   - **Notes**:
     - Uses GLFW for window management and OpenGL for rendering
     - Automatically handles window resizing and input events
     - Supports modern OpenGL (4.6) core profile
     - Test mode available for unit testing without actual window creation

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
     static void SetTestMode(bool enabled);
     static void test();
     ```
   - **Notes**:
     - Handles GLFW initialization and shutdown
     - Creates windows based on JSON configuration
     - Supports multiple windows
     - Test mode available for unit testing

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
     - Integrates with OpenGL for rendering

6. **Mesh Class**  
   - **Purpose**: Encapsulates OpenGL buffers (VAO, VBO, IBO) for a 3D mesh.  
   - **Public API**:  
     ```cpp
     Mesh();
     ~Mesh();
     bool CreateFromModelPart(const Model::Mesh& srcMeshData);
     void Draw() const;
     size_t GetVertexCount() const;
     size_t GetIndexCount() const;
     static void SetTestMode(bool enabled);
     static void test();
     ```
   - **Usage Example**:  
     ```cpp
     Mesh mesh;
     if (mesh.CreateFromModelPart(model.GetMeshes().front())) {
         mesh.Draw();
     }
     ```

7. **Shader Class**  
   - **Purpose**: Manages OpenGL shader programs.  
   - **Public API**:  
     ```cpp
     Shader();
     bool LoadFromFile(const std::string& vertexPath, const std::string& fragmentPath);
     void Use() const;
     void SetBool(const std::string& name, bool value);
     void SetInt(const std::string& name, int value);
     void SetFloat(const std::string& name, float value);
     void SetVec3(const std::string& name, const glm::vec3& value);
     void SetVec4(const std::string& name, const glm::vec4& value);
     void SetMat4(const std::string& name, const glm::mat4& value);
     static void SetTestMode(bool enabled);
     static void test();
     ```
   - **Usage Example**:  
     ```cpp
     Shader shader;
     if (shader.LoadFromFile("basic.vert", "basic.frag")) {
         shader.Use();
         shader.SetMat4("model", modelMatrix);
         shader.SetVec3("lightPos", lightPosition);
     }
     ```

8. **Camera Class**  
   - **Purpose**: Handles 3D camera movement and view/projection matrices.  
   - **Public API**:  
     ```cpp
     Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 3.0f));
     glm::mat4 GetViewMatrix() const;
     glm::mat4 GetProjectionMatrix(float aspectRatio) const;
     void ProcessKeyboard(Movement direction, float deltaTime);
     void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
     void ProcessMouseScroll(float yoffset);
     const glm::vec3& GetPosition() const;
     const glm::vec3& GetFront() const;
     float GetFov() const;
     static void test();
     ```
   - **Usage Example**:  
     ```cpp
     Camera camera;
     // Handle keyboard input
     if (keyPressed[GLFW_KEY_W])
         camera.ProcessKeyboard(Camera::FORWARD, deltaTime);
     // Update shader
     shader.SetMat4("view", camera.GetViewMatrix());
     shader.SetMat4("projection", camera.GetProjectionMatrix(aspectRatio));
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
