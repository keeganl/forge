//
// Created by keega on 8/1/2021.
//

#include "Editor.h"


// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// TODO: move vector of meshes to it's own object
bool checkLights(std::vector<std::shared_ptr<Model>> const &scenes) {
    bool res = false;
    for (std::shared_ptr<Model> m : scenes) {
        if (m->objectType == "light") {
            res = true;
        }
    }
    return res;
}

Mesh saveTexture(ImGui::FileBrowser &textureDialog, Mesh &m) {
    textureDialog.Open();

    if (textureDialog.HasSelected())
    {
        std::string textureFilePath = textureDialog.GetSelected().string();

        Texture texture;

        UITexture new_texture_texture(textureFilePath.c_str());

        texture.id = new_texture_texture.textureID;
        texture.path = textureFilePath;

        m.textures.push_back(texture);
        textureDialog.ClearSelected();
    }

    return m;

}


Editor::Editor() {}

void Editor::run() {
    bool useMultiSampling = false;
    int sampleCount = 8;

    Window window;

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

    Skybox skybox;

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
    };
   ModalManager modalManager;

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

    // shader configuration
    // --------------------
    meshShader.use();
    meshShader.setInt("skybox", 0);

    skybox.shader.use();
    skybox.shader.setInt("skybox", 0);
    screenShader.setInt("screenTexture", 0);

    GuiLayer::createContext(window.windowInstance);

    UITexture ret("../assets/editor/folder.png");
    UITexture ret2("../assets/editor/file.png");
    UITexture back_arrow_ret("../assets/editor/back_arrow.png");
    UITexture yml_ret("../assets/editor/yml.png");
    UITexture default_ret("../assets/editor/no_texture.png");


    // Our state
    [[maybe_unused]] bool show_demo_window = true;

    bool firstMouse = true;

    static float scale = 1.0f;
    static float nearClipping = 0.1f;
    static float farClipping = 1000.0f;
    static float padding = 16.0f;
    static float thumbnailSize = 48.0f;
    bool showSettingsWindow = false;
    bool openScene = false;
    bool saveScene = false;
    bool openSavePopup = false;
    bool openFilePopup = false;
    bool openSettingsPopup = false;

    Settings settings = Settings();

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    std::filesystem::path startingDirectory = "C:\\Users\\keega\\Desktop\\school\\grad\\research\\forge\\assets";
    std::filesystem::path currentDirectory = "C:\\Users\\keega\\Desktop\\school\\grad\\research\\forge\\assets";


    std::vector<std::shared_ptr<Model>> scenes;

    Camera camera;

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
    while (!glfwWindowShouldClose(window.windowInstance))
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
        if (camera.orthographic) {
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
        skybox.draw(view, projection);

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

        GuiLayer::drawMenubar(settings.keymap, modalManager, showSettingsWindow,
                              openFilePopup, openSavePopup, openSettingsPopup, scenes, camera);

        ImGui::Begin("Asset Browser");
        {

            if (currentDirectory != startingDirectory) {
                if (ImGui::ImageButton((void *) (intptr_t) back_arrow_ret.textureID, ImVec2(20, 20))) {
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
                        if (ImGui::ImageButton((void *) (intptr_t) ret.textureID, { thumbnailSize, thumbnailSize }))
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
                            ImGui::ImageButton((void *) (intptr_t) yml_ret.textureID, { thumbnailSize, thumbnailSize });
                        } else {
                            ImGui::ImageButton((void *) (intptr_t) ret2.textureID, { thumbnailSize, thumbnailSize });
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

        GuiLayer::drawModelPropertiesPanel(scenes);

        GuiLayer::drawCameraPropertiesPanel(camera);

        GuiLayer::drawScenePanel(textureColorbuffer, firstMouse, deltaTime, camera, settings.keymap, scenes);

        GuiLayer::drawDebugEventsPanel();

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
        glfwSwapBuffers(window.windowInstance);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &skybox.skyboxVAO);
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

    glfwDestroyWindow(window.windowInstance);

    glfwTerminate();
//    return 0;
}
