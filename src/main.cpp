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

#include <iostream>
#include <windows.h>
#include <tchar.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

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

    // build and compile our shader program
    // ------------------------------------
    Shader meshShader("../shaders/vert.glsl", "../shaders/frag.glsl");
    Shader lightShader("../shaders/light/vert.glsl", "../shaders/light/frag.glsl");
    Shader screenShader("../shaders/framebuffer/vert.glsl", "../shaders/framebuffer/frag.glsl");


    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
    };
    Model scene;
    ImGui::FileBrowser fileDialog;
    // (optional) set browser properties
    fileDialog.SetTitle("Select Mesh");
    fileDialog.SetTypeFilters({ ".obj", ".fbx" });


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


    // tell opengl for each sampler t\o which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------

    meshShader.use();
    lightShader.use();
    screenShader.setInt("screenTexture", 0);

    GuiLayer::createContext(window);


    // Our state
    [[maybe_unused]] bool show_demo_window = true;

    float scale = 1.0f;
    float nearClipping = 0.1f;
    float farClipping = 1000.0f;
    bool orthographic = false;
    int w = 87;
    int a = 65;
    int s = 83;
    int d = 68;
    int one = 49;
    int two = 50;
    int three = 51;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    std::vector<Model*> scenes;

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

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.


    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

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

        // input
        // -----
        processInput(window);


        // render
        // ------
        // bind to framebuffer and draw scene as we normally would to color texture
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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

            meshShader.setMat4("model", scenes[i]->modelMatrix);
            meshShader.setMat4("projection", projection);
            meshShader.setMat4("view", view);

            meshShader.set3DFloat("scaleAxes", scenes[i]->scaleAxes.x, scenes[i]->scaleAxes.y, scenes[i]->scaleAxes.z);
            meshShader.set3DFloat("objectColor", scenes[i]->color.x, scenes[i]->color.y, scenes[i]->color.z);
            meshShader.set1DFloat("scale", scenes[i]->uniformScale);


            if (scenes[i]->meshes[0].textures.empty()) {
                scenes[i]->mixVal = 0.0f;
                meshShader.set1DFloat("mixVal", scenes[i]->mixVal);
            } else {
                scenes[i]->mixVal = 1.0f;
                meshShader.set1DFloat("mixVal", scenes[i]->mixVal);
            }


            if (scenes[i]->objectType == "light") {
                float val = 1.0f;
                meshShader.set3DFloat("lightPos", scenes[i]->pos.x, scenes[i]->pos.y, scenes[i]->pos.z);
                meshShader.set3DFloat("lightColor", scenes[i]->color.x, scenes[i]->color.y, scenes[i]->color.z);
                meshShader.set3DFloat("viewPos", camera.pos.x, camera.pos.y, camera.pos.z);

                lightShader.setMat4("model", scenes[i]->modelMatrix);
                lightShader.setMat4("view", view);
                lightShader.setMat4("projection", projection);
                lightShader.set3DFloat("objectColor",val,val,val);
                lightShader.set3DFloat("lightColor", val,val,val);

                scenes[i]->Draw(lightShader);
            } else {

                scenes[i]->Draw(meshShader);
            }
        }


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

        // menu bar
        {
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if(ImGui::Button("Settings")) {

                        std::cout << "opening settings modal" << std::endl;
                        ImGui::OpenPopup("s");
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Edit"))
                {
                    if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                    if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                    ImGui::Separator();
                    if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                    if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                    if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }

        }

        if (ImGui::BeginPopupModal("s", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Settings");
            ImGui::Separator();


            if (ImGui::Button("Apply", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Revert", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }

        ImGui::Begin("Model Objects");
        {
            for (int i = 0; i < scenes.size(); i++) {
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_None;
                ImGui::TreeNodeEx("Field", flags,  scenes[i]->modelName.c_str());
                if (ImGui::IsItemClicked()) {
                    std::cout << scenes[i]->modelName.c_str() << std::endl;
                    scenes[i]->selected = !scenes[i]->selected;
                }
                ImGui::SameLine();
                if (ImGui::Button("Delete")) {
                    std::cout << "Delete mesh" << std::endl;
                    scenes.erase(scenes.begin() + i);
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
                    scenes.push_back(new Model("../assets/models/primitives/cube/cube.obj"));
                }
                if (ImGui::Button("Cylinder")) {
                    scenes.push_back(new Model("../assets/models/primitives/cylinder.obj"));
                }
                if (ImGui::Button("Plane")) {
                    scenes.push_back(new Model("../assets/models/primitives/plane.obj"));
                }
                if (ImGui::Button("Light")) {
                    scenes.push_back(new Light("../assets/models/primitives/cube/cube.obj", false));
                }
                if (ImGui::Button("Import")) {
                    fileDialog.Open();
                }

                if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }
        }
        ImGui::End();

        {

            fileDialog.Display();

            if (fileDialog.HasSelected()) {
                std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;
                scenes.push_back(new Model(fileDialog.GetSelected().string()));
                fileDialog.ClearSelected();
            }


        }

        ImGui::Begin("Mesh Properties");
        {
           for (auto &m: scenes) {
               if (m->selected) {
                   ImGui::Text("Mesh Name: ");
                   ImGui::SameLine();
                   ImGui::Text(m->modelName.c_str());

                   ImGui::PushID(&m->uniformScale);
                   ImGui::SliderFloat("Uniform Scale", &m->uniformScale, 1.0f, 20.0f);
                   ImGui::PopID();

                   ImGui::PushID(&m->scaleAxes[0]);
                   ImGui::SliderFloat3("Axis Scale", &m->scaleAxes[0], 0.0f, 10.0f);
                   ImGui::PopID();

                   ImGui::PushID(&m->modelMatrix);
                   ImGui::SliderFloat3("Model Position", &m->pos[0], -100.0f, 100.0f);
                   ImGui::PopID();

                   ImGui::PushID(&m->rotateFloats[0]);
                   ImGui::SliderFloat3("Model Rotation", &m->rotateFloats[0], 0.0f, 360.0f);
                   ImGui::PopID();

                   if (m->mixVal == 0.0f) {
                       ImGui::Text("Color:");
                       ImGui::PushID(&m->color);
                       ImGui::SameLine(); HelpMarker(
                               "Click on the color square to open a color picker.\n"
                               "CTRL+click on individual component to input value.\n");
                       ImGui::ColorEdit3("Mesh color", (float*)&m->color);
                       ImGui::PopID();
                   }

               }
           }
        }
        ImGui::End();

        ImGui::Begin("Camera Properties");
        {
            ImGui::Text("Select projection mode:");
            ImGui::Text("Swap between orthographic and projection:");
            ImGui::SameLine(); HelpMarker(
                    "Perspective is default.\n");
            ImGui::Checkbox("Click here", &orthographic);
            ImGui::SliderFloat("FOV", &camera.fov, 45.0f, 250.0f);
            ImGui::DragFloat3("Camera Pos", &camera.pos[0]);
            if(ImGui::Button("Reset camera")) {
                camera.pos = initialCamPos;
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


            if (io.MouseWheel < 0 && camera.fov < 250.0f) {
                camera.fov = camera.fov + 5.0f;
            }
            if (io.MouseWheel > 0 && camera.fov > 45.0f) {
                camera.fov = camera.fov - 5.0f;
            }
        }
        ImGui::End();

        ImGui::Begin("Model");
        {
            // Using a Child allow to fill all the space of the window.
            // It also alows customization
            ImGui::BeginChild("ViewportRender");
            // Get the size of the child (i.e. the whole draw size of the windows).
            ImVec2 wsize = ImGui::GetWindowSize();
            // Because I use the texture from OpenGL, I need to invert the V from the UV.
            ImGui::Image((ImTextureID)textureColorbuffer, wsize, ImVec2(0, 1), ImVec2(1, 0));
            if (ImGui::IsWindowHovered()) {
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
    glDeleteBuffers(1, &quadVBO);

    meshShader.destroy();
    lightShader.destroy();
    screenShader.destroy();
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