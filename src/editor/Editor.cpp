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


Editor::Editor() {}

void Editor::run() {
    UIManager uiManager;

    bool useMultiSampling = false;
    int sampleCount = 8;

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

    ScreenTexture screenTexture;


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

    std::vector<std::shared_ptr<Model>> scenes;

    Camera camera;

    meshShader.use();
    lightShader.use();

    Framebuffer framebuffer;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(uiManager.window.windowInstance))
    {
        float currentFrame = glfwGetTime();
        uiManager.settings.deltaTime = currentFrame - uiManager.settings.lastFrame;
        uiManager.settings.lastFrame = currentFrame;
        if (useMultiSampling) {
            glfwWindowHint(GLFW_SAMPLES, sampleCount);
        }

        // render
        // ------
        // bind to framebuffer and draw scene as we normally would to color texture
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.framebuffer);
        glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
        if (useMultiSampling) {
            glEnable(GL_MULTISAMPLE);
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        if (camera.orthographic) {
            projection = glm::ortho(0.0f, (float) SCR_WIDTH, 0.0f, (float) SCR_HEIGHT, uiManager.settings.nearClipping, uiManager.settings.farClipping);
        } else {
            projection = glm::perspective(glm::radians(camera.fov), (float) SCR_WIDTH / (float) SCR_HEIGHT, uiManager.settings.nearClipping, uiManager.settings.farClipping);
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
        glBindVertexArray(screenTexture.quadVAO);
        glBindTexture(GL_TEXTURE_2D, framebuffer.textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);


        ImGuiIO& io = ImGui::GetIO(); (void)io;

        GuiLayer::startFrame();
        GuiLayer::createDockspace();
        GuiLayer::createPerformanceWindow();
        ImGui::ShowMetricsWindow(&uiManager.settings.showDebugWindows);
        ImGui::ShowDemoWindow(&uiManager.settings.showDebugWindows);
        GuiLayer::drawMenubar(uiManager.settings, uiManager.modalManager, scenes, camera);
        GuiLayer::drawAssetBrowser(uiManager.settings, uiManager.uiTextures);
        GuiLayer::drawModelPropertiesPanel(scenes, uiManager.uiTextures, uiManager.modalManager);
        GuiLayer::drawCameraPropertiesPanel(camera);
        GuiLayer::drawScenePanel(framebuffer.textureColorbuffer, uiManager.settings.firstMouse, uiManager.settings.deltaTime, camera, uiManager.settings.keymap, scenes);
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
        uiManager.window.swapAndPoll();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteFramebuffers(1, &framebuffer.framebuffer);
    glDeleteVertexArrays(1, &screenTexture.quadVAO);
    glDeleteBuffers(1, &skybox.skyboxVAO);
    glDeleteBuffers(1, &screenTexture.quadVBO);

    meshShader.destroy();
    lightShader.destroy();
    screenShader.destroy();

    // clear the scenes vector
    scenes.clear();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    GuiLayer::cleanup();

    uiManager.window.destroyWindow();

    glfwTerminate();
}
