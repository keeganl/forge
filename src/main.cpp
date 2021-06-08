#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_glfw.h"
#include "../external/imgui/imgui_impl_opengl3.h"
#include "../external/imgui/imgui_internal.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb/stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "utils/imgui-layer/GuiLayer.h"
#include "utils/shader-manager/Shader.h"
#include "camera/Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1367;
const unsigned int SCR_HEIGHT = 667;


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

int main()
{
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

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("../shaders/vert.glsl", "../shaders/frag.glsl");


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //load texture
    // load and create a texture
    // -------------------------
    unsigned int texture1;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters (i.e. mipmapping)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load("../assets/textures/tiles.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);

    GuiLayer::createContext(window);

    // Our state
    [[maybe_unused]] bool show_demo_window = true;
    [[maybe_unused]] bool show_another_window = false;
    [[maybe_unused]] ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool draw = true;

    float scale = 1.0f;
    float mix = 0.2f;
    float nearClipping = 0.1f;
    float farClipping = 100.0f;
    // todo: class to manage
    bool orthographic = false;
    int w = 87;
    int a = 65;
    int s = 83;
    int d = 68;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;



    ImVec4 color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

    Camera camera = Camera();

    glm::vec3 initialCamPos = camera.pos;

    ourShader.use();
    ourShader.set1DFloat("scale", scale);
    ourShader.set1DFloat("mixVal", mix);
//    ourShader.set3DFloat("translate", test[0], test[1], test[2]);
    ourShader.set4DFloat("color",  color.x, color.y, color.z, color.w);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);



        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        if (orthographic) {
            projection = glm::ortho(0.0f, (float) SCR_WIDTH, 0.0f, (float) SCR_HEIGHT, nearClipping, farClipping);
        } else {
            projection = glm::perspective(glm::radians(camera.fov), (float) SCR_WIDTH / (float) SCR_HEIGHT, nearClipping, farClipping);
        }
        view = glm::lookAt(camera.pos, camera.pos + camera.front, camera.up);
        // pass transformation matrices to the shader

        // render boxes
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        GuiLayer::startFrame();
        GuiLayer::createDockspace();
        GuiLayer::createPerformanceWindow();
        ImGui::ShowMetricsWindow(&show_demo_window);

        ImGui::Begin("Geometry Properties");
        ImGui::SliderFloat("Scale", &scale, -10.0f, 10.0f);
        ImGui::Text("Color widget:");
        ImGui::SameLine(); HelpMarker(
                "Click on the color square to open a color picker.\n"
                "CTRL+click on individual component to input value.\n");
        ImGui::ColorEdit3("Mesh color", (float*)&color);
        ImGui::SliderFloat("Mix", &mix, 0.0f, 1.0f);
        ImGui::End();

        ImGui::Begin("Camera Properties");
        ImGui::Text("Select projection mode:");
        ImGui::Text("Swap between orthographic and projection:");
        ImGui::SameLine(); HelpMarker(
                "Perspective is default.\n");
        ImGui::Checkbox("Click here", &orthographic);
        ImGui::SliderFloat("FOV", &camera.fov, 45.0f, 100.0f);
        ImGui::DragFloat3("Camera Pos", &camera.pos[0]);
        ImGui::Button("Reset camera", ImVec2(150, 50));

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


        if (io.MouseWheel < 0 && camera.fov < 100.0f) {
            camera.fov = camera.fov + 5.0f;
        }
        if (io.MouseWheel > 0 && camera.fov > 45.0f) {
            camera.fov = camera.fov - 5.0f;
        }

        ImGui::End();
        ImGui::Begin("Events");
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
        ImGui::End();

        ourShader.use();
        ourShader.set1DFloat("scale", scale);
        ourShader.set1DFloat("mixVal", mix);
        ourShader.set4DFloat("color",  color.x, color.y, color.z, color.w);
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

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
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    ourShader.destroy();
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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}