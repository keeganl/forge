//
// Created by keega on 8/1/2021.
//

#include "Editor.h"


// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;


Editor::Editor() {
}

void Editor::calculateFrame() {
    float currentFrame = glfwGetTime();
    uiManager.settings.deltaTime = currentFrame - uiManager.settings.lastFrame;
    uiManager.settings.lastFrame = currentFrame;
}

void Editor::setupUI() {
        GuiLayer::startFrame();
        GuiLayer::createDockspace();
        GuiLayer::createPerformanceWindow();
        ImGui::ShowMetricsWindow(&uiManager.settings.showDebugWindows);
        ImGui::ShowDemoWindow(&uiManager.settings.showDebugWindows);
        GuiLayer::drawMenubar(uiManager.settings, uiManager.modalManager, scene);
        GuiLayer::drawAssetBrowser(uiManager.settings, uiManager.uiTextures);
        GuiLayer::drawModelPropertiesPanel(scene, uiManager.uiTextures, uiManager.modalManager);
        GuiLayer::drawCameraPropertiesPanel(scene.camera);
        GuiLayer::drawScenePanel(framebuffer.textureColorbuffer, uiManager.settings.firstMouse, uiManager.settings.deltaTime, scene, uiManager.settings);
        GuiLayer::drawDebugEventsPanel();
};

void Editor::updateUI() {
    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void Editor::renderScene() {
    if (scene.camera.orthographic) {
        scene.camera.projection = glm::ortho(0.0f, (float) SCR_WIDTH, 0.0f, (float) SCR_HEIGHT, uiManager.settings.nearClipping, uiManager.settings.farClipping);
    } else {
        scene.camera.projection = glm::perspective(glm::radians(scene.camera.fov), (float) SCR_WIDTH / (float) SCR_HEIGHT, uiManager.settings.nearClipping, uiManager.settings.farClipping);
    }
    scene.camera.view = glm::lookAt(scene.camera.pos, scene.camera.pos + scene.camera.front, scene.camera.up);

    if (!scene.lights.empty() || !scene.models.empty()) {

        for (int i = 0; i < scene.models.size(); i++) {
            // calculate the model matrix for each object and pass it to shader before drawing
            scene.models[i]->modelMatrix = glm::translate(glm::mat4(1.0f), scene.models[i]->pos);

            scene.models[i]->modelMatrix =  scene.models[i]->modelMatrix * (
                    glm::rotate(glm::mat4(1.0f), glm::radians(scene.models[i]->rotateFloats.x), glm::vec3(1.0,0.0f,0.0f)) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(scene.models[i]->rotateFloats.y), glm::vec3(0.0,1.0f,0.0f)) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(scene.models[i]->rotateFloats.z), glm::vec3(1.0,0.0f,1.0f))
            );

            if(scene.lights.empty()) {
                float reset = 0;
                meshShader.set3DFloat("light.position",reset, reset, reset);
                meshShader.set3DFloat("lightColor", reset, reset, reset);
                meshShader.set3DFloat("viewPos", scene.camera.pos.x, scene.camera.pos.y, scene.camera.pos.z);
            } else {
                meshShader.use();
                meshShader.setInt("numLights", scene.lights.size());

                for (int i = 0; i < scene.lights.size(); i++) {
                    scene.lights[i]->modelMatrix = glm::translate(glm::mat4(1.0f), scene.lights[i]->pos);
                    scene.lights[i]->modelMatrix =  scene.lights[i]->modelMatrix * (
                            glm::rotate(glm::mat4(1.0f), glm::radians(scene.lights[i]->rotateFloats.x), glm::vec3(1.0,0.0f,0.0f)) *
                            glm::rotate(glm::mat4(1.0f), glm::radians(scene.lights[i]->rotateFloats.y), glm::vec3(0.0,1.0f,0.0f)) *
                            glm::rotate(glm::mat4(1.0f), glm::radians(scene.lights[i]->rotateFloats.z), glm::vec3(1.0,0.0f,1.0f))
                    );
                    float test = 0.0f;
                    if (scene.lights[i]->objectType != "directional_light") {
                        test = 1.0f;
                    }
                    meshShader.use();
                    meshShader.set3DFloat("viewPos", scene.camera.pos.x, scene.camera.pos.y, scene.camera.pos.z);
                    meshShader.set4DFloat("allLights[" + std::to_string(i) + "].position",  scene.lights[i]->pos.x,  scene.lights[i]->pos.y,  scene.lights[i]->pos.z, test);
                    meshShader.set3DFloat("allLights[" + std::to_string(i) + "].direction",  scene.lights[i]->rotateFloats.x,  scene.lights[i]->rotateFloats.y,  scene.lights[i]->rotateFloats.z);
                    meshShader.set4DFloat("allLights[" + std::to_string(i) + "].color",  scene.lights[i]->color.x,  scene.lights[i]->color.y,  scene.lights[i]->color.z, scene.lights[i]->color.w);
                    meshShader.set1DFloat("allLights[" + std::to_string(i) + "].coneAngle",  scene.lights[i]->coneAngle);
                    meshShader.set1DFloat("allLights[" + std::to_string(i) + "].constant",  scene.lights[i]->constant);
                    meshShader.set1DFloat("allLights[" + std::to_string(i) + "].linear",  scene.lights[i]->linear);
                    meshShader.set1DFloat("allLights[" + std::to_string(i) + "].quadratic",  scene.lights[i]->quadratic);
                    meshShader.set1DFloat("allLights[" + std::to_string(i) + "].innerCutoff",  scene.lights[i]->innerCutoff);
                    meshShader.set1DFloat("allLights[" + std::to_string(i) + "].outerCutoff",  scene.lights[i]->outerCutoff);
                    meshShader.set1DFloat("allLights[" + std::to_string(i) + "].attenuation",  scene.lights[i]->attenuation);
                    meshShader.set1DFloat("allLights[" + std::to_string(i) + "].ambient",  scene.lights[i]->ambient);

                    lightShader.use();
                    lightShader.setMat4("model",  scene.lights[i]->modelMatrix);
                    lightShader.setMat4("view",  scene.camera.view);
                    lightShader.setMat4("projection",  scene.camera.projection);
                    lightShader.set3DFloat("lightColor", scene.lights[i]->color.x,  scene.lights[i]->color.y,  scene.lights[i]->color.z);


                    scene.lights[i]->Draw(lightShader);
                }

                meshShader.use();
                scene.models[i]->modelMatrix = glm::scale(scene.models[i]->modelMatrix, scene.models[i]->scaleAxes);

                meshShader.setMat4("model", scene.models[i]->modelMatrix);
                meshShader.setMat4("projection",  scene.camera.projection);
                meshShader.setMat4("view",  scene.camera.view);
                meshShader.set3DFloat("scaleAxes", scene.models[i]->scaleAxes.x, scene.models[i]->scaleAxes.y, scene.models[i]->scaleAxes.z);
                meshShader.set4DFloat("objectColor", scene.models[i]->color.x, scene.models[i]->color.y, scene.models[i]->color.z, scene.models[i]->color.w);
                meshShader.set1DFloat("scale", scene.models[i]->uniformScale);
                meshShader.setInt("objectId", i);
//                    meshShader.setInt("material.diffuse", 0);
                //FIXME (materials): these need to be pulled from the materials on the object, probably loaded into an easier to manipulate structure
                if (!scene.models[i]->isImported) {
                    meshShader.set3DFloat("material.ambient", scene.models[i]->material.ambient.x, scene.models[i]->material.ambient.y, scene.models[i]->material.ambient.z);
                    meshShader.set3DFloat("material.diffuse", scene.models[i]->material.diffuse.x, scene.models[i]->material.diffuse.y, scene.models[i]->material.diffuse.z);
                    meshShader.set3DFloat("material.specular", scene.models[i]->material.specular.x, scene.models[i]->material.specular.y, scene.models[i]->material.specular.z);
                    meshShader.set1DFloat("material.shininess", scene.models[i]->material.shininess);
                }

                scene.models[i]->Draw(meshShader);
            }
        }
    }
}

void Editor::destroy() {
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
    scene.models.clear();
    scene.lights.clear();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    GuiLayer::cleanup();

    uiManager.window.destroyWindow();

    glfwTerminate();
}

void Editor::run() {
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);


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


    meshShader.use();
    lightShader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(uiManager.window.windowInstance))
    {
       calculateFrame();

        // render
        // ------
        // bind to framebuffer and draw scene as we normally would to color texture
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.framebuffer);
        glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        renderScene();

        // draw skybox as last
        skybox.draw(scene.camera.view, scene.camera.projection);

        // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
        // clear all relevant buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader.use();
        screenShader.setInt("screenTexture", 0);
        screenShader.setBool("hdr", &uiManager.settings.hdr);
        screenShader.set1DFloat("exposure", uiManager.settings.exposure);
        glBindVertexArray(screenTexture.quadVAO);
        glBindTexture(GL_TEXTURE_2D, framebuffer.textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glBindVertexArray(0);

        ImGuiIO& io = ImGui::GetIO(); (void)io;

        GuiLayer::startFrame();
        GuiLayer::createDockspace();
        GuiLayer::createPerformanceWindow();
        ImGui::ShowMetricsWindow(&uiManager.settings.showDebugWindows);
        ImGui::ShowDemoWindow(&uiManager.settings.showDebugWindows);
        GuiLayer::drawMenubar(uiManager.settings, uiManager.modalManager, scene);
        GuiLayer::drawAssetBrowser(uiManager.settings, uiManager.uiTextures);
        GuiLayer::drawModelPropertiesPanel(scene, uiManager.uiTextures, uiManager.modalManager);
        GuiLayer::drawCameraPropertiesPanel(scene.camera);
        GuiLayer::drawScenePanel(framebuffer.textureColorbuffer, uiManager.settings.firstMouse, uiManager.settings.deltaTime, scene, uiManager.settings);
        GuiLayer::drawDebugEventsPanel();
//        setupUI();

        lightShader.use();
        meshShader.use();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        updateUI();
        uiManager.window.swapAndPoll();
    }

    destroy();
}
