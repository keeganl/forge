#ifndef FORGE_GUILAYER_H
#define FORGE_GUILAYER_H
#include "../../../../external/imgui/imgui.h"
#include "../../../../external/imgui/imgui_impl_glfw.h"
#include "../../../../external/imgui/imgui_impl_opengl3.h"
#include "../../../../external/imgui/imgui_internal.h"
#include "../../../../external/imgui/ImGuizmo.h"
#include "../../scene/Scene.h"
#include "../../camera/Camera.h"
#include "../../settings/Settings.h"
#include "../ModalManager/ModalManager.h"
#include "../../utils/Serializer.h"
#include "../textures/UITexture.h"
#include "../../light/directional/DirectionalLight.h"
#include "../../light/point/PointLight.h"
#include "../../light/spot/SpotLight.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <vector>
#include <memory>


namespace GuiLayer {
    void createContext(GLFWwindow *window);
    void cleanup();
    void startFrame();
    void createDockspace();
    void createPerformanceWindow();
    void drawModelPropertiesPanel(Scene &scene, std::map<std::string, UITexture> &uiTextures, ModalManager &modalManager);
    void drawCameraPropertiesPanel(Camera &camera);
    void drawDebugEventsPanel();
    void drawScenePanel(unsigned int &textureColorbuffer, bool &firstMouse, float &deltaTime, Scene &scene, Settings &settings,  ModalManager &modalManager);
    void drawMenubar(Settings &settings, ModalManager &modalManager, Scene &scene);
    void drawAssetBrowser(Settings &settings, std::map<std::string, UITexture> &uiTextures);
    // TODO: unify this
    void manipulateMesh(Settings &settings, std::shared_ptr<Model> &model, glm::mat4 &camView,
                        glm::mat4 &camProj,  glm::mat4 &entityModelMatrix);
    void manipulateLight(Settings &settings, std::shared_ptr<Light> &light, glm::mat4 &camView,
                        glm::mat4 &camProj,  glm::mat4 &entityModelMatrix);

    void takeScreenshot(std::string filename, int &windowWidth, int &windowHeight);
};


#endif //FORGE_GUILAYER_H
