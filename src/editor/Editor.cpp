//
// Created by keega on 8/1/2021.
//

#include "Editor.h"


// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;


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

    Scene scene;

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
        if (scene.camera.orthographic) {
            projection = glm::ortho(0.0f, (float) SCR_WIDTH, 0.0f, (float) SCR_HEIGHT, uiManager.settings.nearClipping, uiManager.settings.farClipping);
        } else {
            projection = glm::perspective(glm::radians(scene.camera.fov), (float) SCR_WIDTH / (float) SCR_HEIGHT, uiManager.settings.nearClipping, uiManager.settings.farClipping);
        }
        view = glm::lookAt(scene.camera.pos, scene.camera.pos + scene.camera.front, scene.camera.up);

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
                    for (int i = 0; i < scene.lights.size(); i++) {
                        scene.lights[i]->modelMatrix = glm::translate(glm::mat4(1.0f), scene.lights[i]->pos);

                        if (scene.lights[i]->objectType == "directional_light") {
                            meshShader.set3DFloat("dirLight.direction", scene.lights[i]->direction.x, scene.lights[i]->direction.y, scene.lights[i]->direction.z);
                            meshShader.set3DFloat("dirLight.ambient",  scene.lights[i]->ambient.x,  scene.lights[i]->diffuse.y,  scene.lights[i]->diffuse.z);
                            meshShader.set3DFloat("dirLight.diffuse",  scene.lights[i]->diffuse.x,  scene.lights[i]->diffuse.y,  scene.lights[i]->diffuse.z);
                            meshShader.set3DFloat("dirLight.specular",  scene.lights[i]->specular.x,  scene.lights[i]->specular.y,  scene.lights[i]->specular.z);
                        }
                        if ( scene.lights[i]->objectType == "spot_light") {
                            meshShader.set3DFloat("spotLight.position", scene.camera.pos.x, scene.camera.pos.y, scene.camera.pos.z);
                            meshShader.set3DFloat("spotLight.direction", scene.camera.front.x, scene.camera.front.y, scene.camera.front.z);
                            meshShader.set3DFloat("spotLight.ambient",  scene.lights[i]->ambient.x,  scene.lights[i]->diffuse.y,  scene.lights[i]->diffuse.z);
                            meshShader.set3DFloat("spotLight.diffuse",  scene.lights[i]->diffuse.x,  scene.lights[i]->diffuse.y,  scene.lights[i]->diffuse.z);
                            meshShader.set3DFloat("spotLight.specular",  scene.lights[i]->specular.x,  scene.lights[i]->specular.y,  scene.lights[i]->specular.z);
                            meshShader.set1DFloat("spotLight.constant", scene.lights[i]->constant);
                            meshShader.set1DFloat("spotLight.linear", scene.lights[i]->linear);
                            meshShader.set1DFloat("spotLight.quadratic", scene.lights[i]->quadratic);
                            meshShader.set1DFloat("spotLight.cutOff", scene.lights[i]->innerCutoff);
                            meshShader.set1DFloat("spotLight.outerCutOff", scene.lights[i]->outerCutoff);
                        }
                        if ( scene.lights[i]->objectType == "point_light") {
                            meshShader.set3DFloat("pointLight.position",  scene.lights[i]->pos.x,  scene.lights[i]->pos.y,  scene.lights[i]->pos.z);
                            meshShader.set3DFloat("pointLight.ambient",  scene.lights[i]->ambient.x,  scene.lights[i]->diffuse.y,  scene.lights[i]->diffuse.z);
                            meshShader.set3DFloat("pointLight.diffuse",  scene.lights[i]->diffuse.x,  scene.lights[i]->diffuse.y,  scene.lights[i]->diffuse.z);
                            meshShader.set3DFloat("pointLight.specular",  scene.lights[i]->specular.x,  scene.lights[i]->specular.y,  scene.lights[i]->specular.z);
                            meshShader.set1DFloat("pointLight.constant", scene.lights[i]->constant);
                            meshShader.set1DFloat("pointLight.linear", scene.lights[i]->linear);
                            meshShader.set1DFloat("pointLight.quadratic", scene.lights[i]->quadratic);
                        }


                        scene.lights[i]->modelMatrix =  scene.lights[i]->modelMatrix * (
                                glm::rotate(glm::mat4(1.0f), glm::radians(scene.lights[i]->rotateFloats.x), glm::vec3(1.0,0.0f,0.0f)) *
                                glm::rotate(glm::mat4(1.0f), glm::radians(scene.lights[i]->rotateFloats.y), glm::vec3(0.0,1.0f,0.0f)) *
                                glm::rotate(glm::mat4(1.0f), glm::radians(scene.lights[i]->rotateFloats.z), glm::vec3(1.0,0.0f,1.0f))
                                );
                        glm::vec3 lightColor( scene.lights[i]->color.x,  scene.lights[i]->color.y,  scene.lights[i]->color.z);
                        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
                        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
                        meshShader.use();
                        meshShader.set3DFloat("light.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
                        meshShader.set3DFloat("light.diffuse", diffuseColor.x, diffuseColor.y, diffuseColor.z);
                        meshShader.set3DFloat("light.specular", 1.0f, 1.0f, 1.0f);
                        meshShader.set3DFloat("light.pos",  scene.lights[i]->pos.x,  scene.lights[i]->pos.y,  scene.lights[i]->pos.z);
                        meshShader.set3DFloat("viewPos", scene.camera.pos.x, scene.camera.pos.y, scene.camera.pos.z);

                        lightShader.use();
                        lightShader.setMat4("model",  scene.lights[i]->modelMatrix);
                        lightShader.setMat4("view", view);
                        lightShader.setMat4("projection", projection);
                        lightShader.set3DFloat("lightColor",  scene.lights[i]->color.x,  scene.lights[i]->color.y,  scene.lights[i]->color.z);

                         scene.lights[i]->Draw(lightShader);
                    }

                    meshShader.use();
                    scene.models[i]->modelMatrix = glm::scale(scene.models[i]->modelMatrix, scene.models[i]->scaleAxes);

                    meshShader.setMat4("model", scene.models[i]->modelMatrix);
                    meshShader.setMat4("projection", projection);
                    meshShader.setMat4("view", view);
                    meshShader.set3DFloat("scaleAxes", scene.models[i]->scaleAxes.x, scene.models[i]->scaleAxes.y, scene.models[i]->scaleAxes.z);
                    meshShader.set4DFloat("objectColor", scene.models[i]->color.x, scene.models[i]->color.y, scene.models[i]->color.z, scene.models[i]->color.w);
                    meshShader.set1DFloat("scale", scene.models[i]->uniformScale);
                    meshShader.setInt("objectId", i);
                    meshShader.setInt("material.diffuse", 0);
                    //FIXME (materials): these need to be pulled from the materials on the object, probably loaded into an easier to manipulate structure
                    meshShader.set3DFloat("material.ambient", 1.0f, 0.5f, 0.31f);
                    meshShader.set3DFloat("material.diffuse", 1.0f, 0.5f, 0.31f);
                    meshShader.set3DFloat("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
                    meshShader.set1DFloat("material.shininess", 32.0f);

                    scene.models[i]->Draw(meshShader);
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
        GuiLayer::drawMenubar(uiManager.settings, uiManager.modalManager, scene);
        GuiLayer::drawAssetBrowser(uiManager.settings, uiManager.uiTextures);
        GuiLayer::drawModelPropertiesPanel(scene, uiManager.uiTextures, uiManager.modalManager);
        GuiLayer::drawCameraPropertiesPanel(scene.camera);
        GuiLayer::drawScenePanel(framebuffer.textureColorbuffer, uiManager.settings.firstMouse, uiManager.settings.deltaTime, scene, uiManager.settings.keymap);
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
    scene.models.clear();
    scene.lights.clear();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    GuiLayer::cleanup();

    uiManager.window.destroyWindow();

    glfwTerminate();
}
