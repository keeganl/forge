#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_glfw.h"
#include "../external/imgui/imgui_impl_opengl3.h"
#include "../external/imgui/imgui_internal.h"
#include "../external/imgui/imfilebrowser.h"

#include "../external/stb/stb_image.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils/imgui-layer/GuiLayer.h"
#include "utils/shader-manager/Shader.h"
#include "camera/Camera.h"
#include "mesh/Model.h"
#include "light/Light.h"
#include "utils/Serializer.h"

#include <iostream>
#include <windows.h>
#include <tchar.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;


static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void showSettings(bool* p_open)
{
    if (!ImGui::Begin("About Dear ImGui", p_open))
    {
        ImGui::End();
        return;
    }
    ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
    ImGui::Separator();
    ImGui::Text("By Omar Cornut and all Dear ImGui contributors.");
    ImGui::Text("Dear ImGui is licensed under the MIT License, see LICENSE for more information.");

    ImGui::End();
}

static void Strtrim(char* s) {
    char* str_end = s + strlen(s);
    while (str_end > s && str_end[-1] == ' ')
        str_end--;
    *str_end = 0;
}

bool checkLights(std::vector<std::shared_ptr<Model>> const &scenes) {
    bool res = false;
    for (std::shared_ptr<Model> m : scenes) {
        if (m->objectType == "light") {
            res = true;
        }
    }
    return res;
}

bool isSelected(std::shared_ptr<Model> const &model) {
    return model->selected;
}

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

Mesh saveTexture(ImGui::FileBrowser &textureDialog, Mesh &m) {
    textureDialog.Open();

    if (textureDialog.HasSelected())
    {
        std::string textureFilePath = textureDialog.GetSelected().string();

        Texture texture;

        int new_texture_width = 0;
        int new_texture_height = 0;
        GLuint new_texture_texture = 0;
        bool new_texture_ret = LoadTextureFromFile(textureFilePath.c_str(), &new_texture_texture, &new_texture_width, &new_texture_height);
        IM_ASSERT(new_texture_ret);

        texture.id = new_texture_texture;
        texture.path = textureFilePath;

        m.textures.push_back(texture);
        textureDialog.ClearSelected();
    }

    return m;

}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

int main()
{
    bool useMultiSampling = false;
    int sampleCount = 8;
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Forge", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    if (useMultiSampling) {
        glEnable(GL_MULTISAMPLE);
    }

    // build and compile our shader program
    // ------------------------------------
    Shader meshShader("../shaders/vert.glsl", "../shaders/frag.glsl");
    Shader lightShader("../shaders/light/vert.glsl", "../shaders/light/frag.glsl");
    Shader screenShader("../shaders/framebuffer/vert.glsl", "../shaders/framebuffer/frag.glsl");
    Shader skyboxShader("../shaders/skybox/vert.glsl", "../shaders/skybox/frag.glsl");

    float cubeVertices[] = {
            // positions          // normals
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    float skyboxVertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };


    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
    };
    ImGui::FileBrowser fileDialog;
    ImGui::FileBrowser saveDialog(ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_CreateNewDir);
    ImGui::FileBrowser sceneDialog;
    ImGui::FileBrowser textureDialog;
    // (optional) set browser properties
    fileDialog.SetTitle("Select Mesh");
    fileDialog.SetTypeFilters({ ".obj", ".fbx" });

    sceneDialog.SetTitle("Select Scene File");
    sceneDialog.SetTypeFilters({ ".yml" });

    saveDialog.SetTitle("Save Scene File");
    saveDialog.SetTypeFilters({ ".yml" });

    textureDialog.SetTitle("Select Texture File");
    textureDialog.SetTypeFilters({ ".png", ".jpg" });


    // screen quad VAO
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures
    // -------------
    std::vector<std::string> faces
            {
                    "../assets/skybox/right.jpg",
                    "../assets/skybox/left.jpg",
                    "../assets/skybox/top.jpg",
                    "../assets/skybox/bottom.jpg",
                    "../assets/skybox/front.jpg",
                    "../assets/skybox/back.jpg",
            };
    unsigned int cubemapTexture = loadCubemap(faces);

    // tell opengl for each sampler t\o which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------

    meshShader.use();
    lightShader.use();

    // shader configuration
    // --------------------
    meshShader.use();
    meshShader.setInt("skybox", 0);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);
    screenShader.setInt("screenTexture", 0);

    GuiLayer::createContext(window);

    // put this into a class
    int my_image_width = 0;
    int my_image_height = 0;
    GLuint my_image_texture = 0;
    bool ret = LoadTextureFromFile("../assets/editor/folder.png", &my_image_texture, &my_image_width, &my_image_height);
    IM_ASSERT(ret);

    int my_image_width2 = 0;
    int my_image_height2 = 0;
    GLuint my_image_texture2 = 0;
    bool ret2 = LoadTextureFromFile("../assets/editor/file.png", &my_image_texture2, &my_image_width2, &my_image_height2);
    IM_ASSERT(ret2);

    int back_arrow_width = 0;
    int back_arrow_height = 0;
    GLuint back_arrow_texture = 0;
    bool back_arrow_ret = LoadTextureFromFile("../assets/editor/back_arrow.png", &back_arrow_texture, &back_arrow_width, &back_arrow_height);
    IM_ASSERT(back_arrow_ret);

    int yml_width = 0;
    int yml_height = 0;
    GLuint yml_texture = 0;
    bool yml_ret = LoadTextureFromFile("../assets/editor/yml.png", &yml_texture, &yml_width, &yml_height);
    IM_ASSERT(yml_ret);

    int default_width = 0;
    int default_height = 0;
    GLuint default_texture = 0;
    bool default_ret = LoadTextureFromFile("../assets/editor/no_texture.jpg", &default_texture, &default_width, &default_height);
    IM_ASSERT(default_ret);


    // Our state
    [[maybe_unused]] bool show_demo_window = true;

    bool firstMouse = true;
    float sensitivity = 0.5f; // change this value to your liking
    float lastX =  800.0f / 2.0;
    float lastY =  600.0 / 2.0;
    float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
    float pitch =  0.0f;

    static float scale = 1.0f;
    static float nearClipping = 0.1f;
    static float farClipping = 1000.0f;
    static float padding = 16.0f;
    static float thumbnailSize = 48.0f;
    bool orthographic = false;
    bool showSettingsWindow = false;
    bool openScene = false;
    bool saveScene = false;
    bool openSavePopup = false;
    bool openFilePopup = false;
    bool openSettingsPopup = false;
    int w = 87;
    int a = 65;
    int s = 83;
    int d = 68;
    int o = 79;
    int comma = 44;
    int del = 261;
    int one = 49;
    int two = 50;
    int three = 51;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    std::filesystem::path startingDirectory = "C:\\Users\\keega\\Desktop\\school\\grad\\research\\forge\\assets";
    std::filesystem::path currentDirectory = "C:\\Users\\keega\\Desktop\\school\\grad\\research\\forge\\assets";


    std::vector<std::shared_ptr<Model>> scenes;

    glm::vec3 front;
    Camera camera = Camera();

    glm::vec3 initialCamPos = camera.pos;

    meshShader.use();
    lightShader.use();


    // framebuffer configuration
    // -------------------------
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // create a color attachment texture
    unsigned  int id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, SCR_WIDTH, SCR_HEIGHT, 0, GL_RED_INTEGER, GL_INT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.


    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, id, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, attachments);

    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        if (useMultiSampling) {
            glfwWindowHint(GLFW_SAMPLES, sampleCount);
        }

        // render
        // ------
        // bind to framebuffer and draw scene as we normally would to color texture
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
        if (useMultiSampling) {
            glEnable(GL_MULTISAMPLE);
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        if (orthographic) {
            projection = glm::ortho(0.0f, (float) SCR_WIDTH, 0.0f, (float) SCR_HEIGHT, nearClipping, farClipping);
        } else {
            projection = glm::perspective(glm::radians(camera.fov), (float) SCR_WIDTH / (float) SCR_HEIGHT, nearClipping, farClipping);
        }
        view = glm::lookAt(camera.pos, camera.pos + camera.front, camera.up);

        for (int i = 0; i < scenes.size(); i++) {
            // calculate the model matrix for each object and pass it to shader before drawing
            scenes[i]->modelMatrix = glm::translate(glm::mat4(1.0f), scenes[i]->pos);

            scenes[i]->modelMatrix =  scenes[i]->modelMatrix * (
                    glm::rotate(glm::mat4(1.0f), glm::radians(scenes[i]->rotateFloats.x), glm::vec3(1.0,0.0f,0.0f)) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(scenes[i]->rotateFloats.y), glm::vec3(0.0,1.0f,0.0f)) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(scenes[i]->rotateFloats.z), glm::vec3(1.0,0.0f,1.0f))
                    );



            if (scenes[i]->meshes[0].textures.empty()) {
                scenes[i]->mixVal = 0.0f;
                meshShader.set1DFloat("mixVal", scenes[i]->mixVal);
            } else {
                scenes[i]->mixVal = 1.0f;
                meshShader.set1DFloat("mixVal", scenes[i]->mixVal);
            }

            // update the scene if there are no lights
            if (!checkLights(scenes)) {
                float reset = 0;
                meshShader.set3DFloat("lightPos",reset, reset, reset);
                meshShader.set3DFloat("lightColor", reset, reset, reset);
                meshShader.set3DFloat("viewPos", camera.pos.x, camera.pos.y, camera.pos.z);
            } else {
                if (scenes[i]->objectType == "light") {
                    meshShader.use();
                    meshShader.set3DFloat("lightPos", scenes[i]->pos.x, scenes[i]->pos.y, scenes[i]->pos.z);
                    meshShader.set3DFloat("lightColor", scenes[i]->color.x, scenes[i]->color.y, scenes[i]->color.z);
                    meshShader.set3DFloat("viewPos", camera.pos.x, camera.pos.y, camera.pos.z);

                    lightShader.use();
                    lightShader.setMat4("model", scenes[i]->modelMatrix);
                    lightShader.setMat4("view", view);
                    lightShader.setMat4("projection", projection);
                    lightShader.set3DFloat("lightColor", scenes[i]->color.x, scenes[i]->color.y, scenes[i]->color.z);

                    scenes[i]->Draw(lightShader);
                }
                else if (scenes[i]->objectType == "model"){
                    meshShader.use();
                    scenes[i]->modelMatrix = glm::scale(scenes[i]->modelMatrix, scenes[i]->scaleAxes);

                    meshShader.setMat4("model", scenes[i]->modelMatrix);
                    meshShader.setMat4("projection", projection);
                    meshShader.setMat4("view", view);
                    meshShader.set3DFloat("scaleAxes", scenes[i]->scaleAxes.x, scenes[i]->scaleAxes.y, scenes[i]->scaleAxes.z);
                    meshShader.set3DFloat("objectColor", scenes[i]->color.x, scenes[i]->color.y, scenes[i]->color.z);
                    meshShader.set1DFloat("scale", scenes[i]->uniformScale);
                    meshShader.setInt("objectId", i);
                    scenes[i]->Draw(meshShader);
                }
            }
        }

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        glm::mat4 viewNoTranslations = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
        skyboxShader.setMat4("view", viewNoTranslations);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
        // clear all relevant buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader.use();
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);


        ImGuiIO& io = ImGui::GetIO(); (void)io;

        GuiLayer::startFrame();
        GuiLayer::createDockspace();
        GuiLayer::createPerformanceWindow();
        ImGui::ShowMetricsWindow(&show_demo_window);
        ImGui::ShowDemoWindow(&show_demo_window);

        if (showSettingsWindow)         { showSettings(&showSettingsWindow); }

        if(io.KeyCtrl && ImGui::IsKeyPressed(o) && !openFilePopup) {
            openFilePopup = true;
        }
        else if(io.KeyCtrl && ImGui::IsKeyPressed(s) && !openSavePopup) {
            openSavePopup = true;
        }
        else if(io.KeyCtrl && ImGui::IsKeyPressed(comma) && !openSettingsPopup) {
            openSettingsPopup = true;
        }

        // menu bar
        {
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if(ImGui::MenuItem("Open Scene", "CTRL+O")) { openFilePopup = true; }
                    if(ImGui::MenuItem("Save Scene", "CTRL+S")) { openSavePopup = true; }
                    if(ImGui::MenuItem("Settings", NULL)) { openSettingsPopup = true; }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Edit"))
                {
                    if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                    if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }

        }

        // this is a workaround for a known bug
        if (openSavePopup) {
            saveDialog.Open();
            openSavePopup = false;
        }

        if (openFilePopup) {
            ImGui::OpenPopup("Open File");
            openFilePopup = false;
        }

        if(openSettingsPopup) {
            ImGui::OpenPopup("Settings");
            openSettingsPopup = false;
        }


        if (ImGui::BeginPopupModal("Open File", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            sceneDialog.Open();
            ImGui::EndPopup();
        }

        if (ImGui::BeginPopupModal("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Settings");
            ImGui::Separator();

            ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
            ImGui::SliderFloat("Padding", &padding, 0, 32);
            ImGui::Checkbox("Set MSAA", &useMultiSampling);


            if (ImGui::Button("Revert", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Apply", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }

        ImGui::Begin("Scene Objects");
        {
            for (int i = 0; i < scenes.size(); i++) {
                ImGui::AlignTextToFramePadding();
                ImGui::Selectable(scenes[i]->modelName.c_str(), scenes[i]->selected);
                if (ImGui::IsItemClicked()) {
                    std::cout << scenes[i]->modelName.c_str() << std::endl;
                    scenes[i]->selected = !scenes[i]->selected;
                }
            }
            if(ImGui::Button("Add object")) {

                std::cout << "adding mesh" << std::endl;
                ImGui::OpenPopup("Add object");
            }

            if (ImGui::BeginPopupModal("Add object", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Select a model to generate");
                ImGui::Separator();


                if (ImGui::Button("Cube")) {
                    scenes.push_back(std::make_shared<Model>("../assets/models/primitives/cube/cube.obj"));
                }
                if (ImGui::Button("Cylinder")) {
                    scenes.push_back(std::make_shared<Model>("../assets/models/primitives/cylinder.obj"));
                }
                if (ImGui::Button("Plane")) {
                    scenes.push_back(std::make_shared<Model>("../assets/models/primitives/plane.obj"));
                }
                if (ImGui::Button("Light")) {
                    scenes.push_back(std::make_shared<Light>("../assets/models/primitives/cube/cube.obj"));
                }
                if (ImGui::Button("Import")) {
                    fileDialog.Open();
                }

                if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::SameLine();
                if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
            }
        }
        ImGui::End();

        if (ImGui::IsKeyPressed(del)) {
            std::cout << "delete mesh" << std::endl;
            scenes.erase(
                    std::remove_if(scenes.begin(), scenes.end(), isSelected),
                    scenes.end());
        }

        {

            fileDialog.Display();

            if (fileDialog.HasSelected())
            {
                scenes.push_back(std::make_shared<Model>(fileDialog.GetSelected().string()));
                fileDialog.ClearSelected();
            }


        }

        {
            saveDialog.Display();

            if (saveDialog.HasSelected())
            {
                Serializer serializer = Serializer(scenes, camera);
                serializer.Serialize( saveDialog.GetSelected().string() + ".yml");
                saveDialog.ClearSelected();
            }
        }

        {
            sceneDialog.Display();

            if (sceneDialog.HasSelected())
            {
                Serializer serializer = Serializer(scenes, camera);
                scenes = serializer.Deserialize(sceneDialog.GetSelected().string());
                sceneDialog.ClearSelected();
            }
        }

        ImGui::Begin("Asset Browser");
        {

            if (currentDirectory != startingDirectory) {
                if (ImGui::ImageButton((void *) (intptr_t) back_arrow_texture, ImVec2(20, 20))) {
                    currentDirectory = currentDirectory.parent_path();
                }
            }

            float cellSize = thumbnailSize + padding;

            float panelWidth = ImGui::GetContentRegionAvail().x;
            int columnCount = (int)(panelWidth / cellSize);
            if (columnCount < 1)
                columnCount = 1;


            ImGui::Columns(columnCount, 0, false);


            for(auto& p: std::filesystem::directory_iterator(currentDirectory))
            {
                auto relativePath = std::filesystem::relative(p.path(), startingDirectory).filename();
                ImGuiStyle& style = ImGui::GetStyle();
                if (p.is_directory())
                {
                    ImGui::BeginGroup();
                    {
                        ImGui::PushID(p.path().string().c_str());
                        if (ImGui::ImageButton((void *) (intptr_t) my_image_texture, { thumbnailSize, thumbnailSize }))
                        {
                            currentDirectory /= p.path();
                        }
                        ImGui::TextWrapped(relativePath.string().c_str());
                        ImGui::PopID();
                    }
                    ImGui::EndGroup();
                } else {
                    ImGui::BeginGroup();
                    {
                        ImGui::PushID(p.path().string().c_str());
                        if (p.path().string().find(".yml") != std::string::npos) {
                            ImGui::ImageButton((void *) (intptr_t) yml_texture, { thumbnailSize, thumbnailSize });
                        } else {
                            ImGui::ImageButton((void *) (intptr_t) my_image_texture2, { thumbnailSize, thumbnailSize });
                        }
                        ImGui::Text(relativePath.string().c_str());
                        ImGui::PopID();
                    }
                    ImGui::EndGroup();
                }
                ImGui::NextColumn();
            }

            ImGui::Columns(1);

        }
        ImGui::End();

        if (scenes.size() > 0)
        {
            ImGui::Begin("Mesh Properties");
            {
                int i = 0;
                for (auto &m: scenes) {
                    if (m->selected) {
                       bool tNode = ImGui::TreeNodeEx((void*)"###", ImGuiTreeNodeFlags_DefaultOpen, "%s", m->modelName.c_str());
                       if (tNode) {
                           ImGui::PushID(i);
                           ImGui::AlignTextToFramePadding();
                           ImGui::Text("Name");
                           ImGui::SameLine();
                           ImGui::InputText("", m->modelName.data(), m->modelName.size()+1);
                           ImGui::PopID();


                           if (ImGui::CollapsingHeader("Mesh Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                               ImGui::PushID(&m->uniformScale);
                               ImGui::AlignTextToFramePadding();
                               ImGui::Text("Uniform Scale");
                               ImGui::SameLine();
                               ImGui::SliderFloat("\t", &m->uniformScale, 1.0f, 20.0f);
                               ImGui::PopID();

                               ImGui::PushID(&m->scaleAxes[0]);
                               ImGui::AlignTextToFramePadding();
                               ImGui::Text("Axis Scale");
                               ImGui::SameLine();
                               ImGui::SliderFloat3("\t", &m->scaleAxes[0], 0.0, 100.0);
                               ImGui::PopID();

                               ImGui::PushID(&m->modelMatrix);
                               ImGui::AlignTextToFramePadding();
                               ImGui::Text("Model Position");
                               ImGui::SameLine();
                               ImGui::SliderFloat3("\t", &m->pos[0], -nearClipping, farClipping);
                               ImGui::PopID();

                               ImGui::PushID(&m->rotateFloats[0]);
                               ImGui::AlignTextToFramePadding();
                               ImGui::Text("Model Rotation");
                               ImGui::SameLine();
                               ImGui::SliderFloat3("\t", &m->rotateFloats[0], -1000.0, 1000.0);
                               ImGui::PopID();
                           }

                           if (ImGui::CollapsingHeader("Mesh Material", ImGuiTreeNodeFlags_DefaultOpen)) {
                               ImGui::PushID(&m->color);
                               ImGui::Text("Color:");
                               ImGui::SameLine(); HelpMarker(
                                       "Click on the color square to open a color picker.\n"
                                       "CTRL+click on individual component to input value.\n");
                               ImGui::ColorPicker4("Mesh color", (float*)&m->color);
                               ImGui::PopID();

                               ImGui::PushID(&m->meshes[0]);
                               for (int i = 0; i < m->meshes.size(); i++) {
                                   if (m->meshes[i].textures.empty()) {
                                       ImGui::PushID("texture_");
                                       ImGui::Text("Add Texture");
                                       if (ImGui::ImageButton((void *) (intptr_t) default_texture, ImVec2(50, 50))) {
                                          m->meshes[i] = saveTexture(textureDialog, m->meshes[i]);
                                       }
                                       ImGui::PopID();
                                   }
                                   else {
                                       float cellSize = 50 + padding;

                                       float panelWidth = ImGui::GetContentRegionAvail().x;
                                       int columnCount = (int)(panelWidth / cellSize);
                                       if (columnCount < 1)
                                           columnCount = 1;


                                       ImGui::Columns(columnCount, 0, false);

                                       for (Texture &t : m->meshes[i].textures) {
                                           if (ImGui::ImageButton((void *) (intptr_t) t.id, ImVec2(50, 50)))
                                           ImGui::NextColumn();
                                       }
                                       ImGui::Columns(1);
                                   }



                               }
                               ImGui::PopID();
                           }

                           ImGui::TreePop();
                       }
                    }
                    ++i;
                }
            }
            ImGui::End();
        }

        ImGui::Begin("Camera Properties");
        {
            ImGui::Text("Select projection mode:");
            ImGui::Text("Swap between orthographic and projection:");
            ImGui::SameLine(); HelpMarker(
                    "Perspective is default.\n");
            ImGui::Checkbox("Click here", &orthographic);
            ImGui::SliderFloat("FOV", &camera.fov, 45.0f, 120.0f);
            ImGui::DragFloat3("Camera Pos", &camera.pos[0]);
            ImGui::SliderFloat("Camera Speed", &camera.speed, 1.0f, 25.0f);
            ImGui::SliderFloat("Camera Sensitivity", &sensitivity, 0.1f, 1.0f);
            if(ImGui::Button("Reset camera")) {
                camera.pos = initialCamPos;
            }

        }
        ImGui::End();

        ImGui::Begin("Scene");
        {
            // Using a Child allow to fill all the space of the window.
            // It also alows customization
            ImGui::BeginChild("ViewportRender");
            // Get the size of the child (i.e. the whole draw size of the windows).
            ImVec2 wsize = ImGui::GetWindowSize();
            // Because I use the texture from OpenGL, I need to invert the V from the UV.
            ImGui::Image((ImTextureID)textureColorbuffer, wsize, ImVec2(0, 1), ImVec2(1, 0));
            if (ImGui::IsWindowHovered()) {

                {
//                    std::cout <<  io.MousePos.x << "  " << io.MousePos.y << std::endl;

                    if ( ImGui::IsMouseDown(0) && io.KeyShift) {

                        if((io.KeyShift && ImGui::IsKeyPressed(a))) {
                            std::cout << "adding mesh" << std::endl;
                            ImGui::OpenPopup("Add object");
                        }

                        if (firstMouse)
                        {
                            lastX = io.MousePos.x;
                            lastY = io.MousePos.y;
                            firstMouse = false;
                        }

                        float xoffset = io.MousePos.x - lastX;
                        float yoffset = lastY - io.MousePos.y; // reversed since y-coordinates go from bottom to top
                        lastX = io.MousePos.x;
                        lastY = io.MousePos.y;


                        xoffset *= sensitivity;
                        yoffset *= sensitivity;

                        yaw += xoffset;
                        pitch += yoffset;

                        // make sure that when pitch is out of bounds, screen doesn't get flipped
                        if (pitch > 89.0f)
                            pitch = 89.0f;
                        if (pitch < -89.0f)
                            pitch = -89.0f;

                        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                        front.y = sin(glm::radians(pitch));
                        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
                        camera.front = glm::normalize(front);
                    }

                    // subtract minimum bound
                    auto windowSize = ImGui::GetWindowSize();
                    auto minBound = ImGui::GetWindowPos();
//                    minBound.x = 0;
//                    minBound.y = 0;

                    ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
                    ImVec2 min = {minBound.x, minBound.y};
                    ImVec2 max = {maxBound.x, maxBound.y};
                    glm::vec2 bounds[2];
                    bounds[0] = {minBound.x, minBound.y};
                    bounds[1] = {maxBound.x, maxBound.y};

                    float mx = io.MousePos.x;
                    float my = io.MousePos.y;
                    mx -= bounds[0].x;
                    my -= bounds[0].y;

                    glm::vec2 viewportSize = bounds[1] - bounds[0];

                    // flip y to match texture
                    my = viewportSize.y - my;

                    int mouseX = (int)mx;
                    int mouseY = (int)my;


//                    std::cout << mouseX << " " << mouseY << std::endl;

                    glReadBuffer(GL_COLOR_ATTACHMENT1);
                    int pixelData;
                    glReadPixels(mouseX, mouseY, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

//                    std::cout << "pixelData: " << pixelData << std::endl;

                    if (ImGui::IsMouseDown(0) && pixelData <= scenes.size()) {
                        scenes[pixelData]->selected = !scenes[pixelData]->selected;
                    }

                }

                if (io.MouseWheel < 0 && camera.fov < 120.0f) {
                    camera.fov = camera.fov + 5.0f;
                }
                if (io.MouseWheel > 0 && camera.fov > 45.0f) {
                    camera.fov = camera.fov - 5.0f;
                }
                float frameCamSpeed = camera.speed * deltaTime;
                if (ImGui::IsKeyPressed(w)) {

                    camera.pos += frameCamSpeed * camera.front;
                }
                if (ImGui::IsKeyPressed(a)) {
                    camera.pos -= frameCamSpeed * glm::normalize(glm::cross(camera.front, camera.up));
                }
                if (ImGui::IsKeyPressed(s)) {
                    camera.pos -= frameCamSpeed * camera.front;

                }
                if (ImGui::IsKeyPressed(d)) {
                    camera.pos += frameCamSpeed * glm::normalize(glm::cross(camera.front, camera.up));
                }
                //project out of normalized coords
                //std::cout << "Hovering scene tab" << std::endl;
            }
            ImGui::EndChild();
        }

        ImGui::End();

        ImGui::Begin("Events");
        {
            ImGui::Text("Keys down:");          for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyDown(i))        { ImGui::SameLine(); ImGui::Text("%d (0x%X) (%.02f secs)", i, i, io.KeysDownDuration[i]); }
            ImGui::Text("Keys pressed:");       for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyPressed(i))     { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
            ImGui::Text("Keys release:");       for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyReleased(i))    { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
            ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
            ImGui::Text("Chars queue:");        for (int i = 0; i < io.InputQueueCharacters.Size; i++) { ImWchar c = io.InputQueueCharacters[i]; ImGui::SameLine();  ImGui::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c); } // FIXME: We should convert 'c' to UTF-8 here but the functions are not public.

            ImGui::Text("NavInputs down:");     for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) if (io.NavInputs[i] > 0.0f)              { ImGui::SameLine(); ImGui::Text("[%d] %.2f (%.02f secs)", i, io.NavInputs[i], io.NavInputsDownDuration[i]); }
            ImGui::Text("NavInputs pressed:");  for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) if (io.NavInputsDownDuration[i] == 0.0f) { ImGui::SameLine(); ImGui::Text("[%d]", i); }

            if (ImGui::IsMousePosValid())
                ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
            else
                ImGui::Text("Mouse pos: <INVALID>");
            ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
            ImGui::Text("Mouse down:");     for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDown(i))         { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
            ImGui::Text("Mouse clicked:");  for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseClicked(i))      { ImGui::SameLine(); ImGui::Text("b%d", i); }
            ImGui::Text("Mouse dblclick:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDoubleClicked(i)){ ImGui::SameLine(); ImGui::Text("b%d", i); }
            ImGui::Text("Mouse released:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseReleased(i))     { ImGui::SameLine(); ImGui::Text("b%d", i); }
            ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);
        }
        ImGui::End();

        lightShader.use();
        meshShader.use();


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }



        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &skyboxVAO);
    glDeleteBuffers(1, &quadVBO);

    meshShader.destroy();
    lightShader.destroy();
    screenShader.destroy();
    scenes.clear();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}