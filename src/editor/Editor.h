//
// Created by keega on 8/1/2021.
//

#ifndef FORGE_EDITOR_H
#define FORGE_EDITOR_H

#include "scene/Scene.h"

//#include "../../external/imgui/imgui.h"
//#include "../../external/imgui/imgui_impl_glfw.h"
//#include "../../external/imgui/imgui_impl_opengl3.h"
//#include "../../external/imgui/imgui_internal.h"
//#include "../../external/imgui/imfilebrowser.h"


#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window/Window.h"
#include "ui/imgui-layer/GuiLayer.h"
#include "ui/ModalManager/ModalManager.h"
#include "ui/textures/UITexture.h"
#include "ui/UIManager.h"
#include "utils/shader-manager/Shader.h"
#include "camera/Camera.h"
#include "mesh/Model.h"
#include "light/Light.h"
#include "settings/Settings.h"
#include "utils/Serializer.h"
#include "skybox/Skybox.h"
#include "renderer/framebuffer/Framebuffer.h"
#include "renderer/screen/ScreenTexture.h"

#include <iostream>

class Editor {
public:
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    UIManager uiManager;
    Shader meshShader = Shader("../shaders/vert.glsl", "../shaders/frag.glsl");
    Shader lightShader = Shader("../shaders/light/vert.glsl", "../shaders/light/frag.glsl");
    Shader screenShader = Shader("../shaders/framebuffer/vert.glsl", "../shaders/framebuffer/frag.glsl");
    Skybox skybox;
    ScreenTexture screenTexture;
    Scene scene;
    Framebuffer framebuffer;





    Editor();

    void calculateFrame();
    void renderScene();
    void Editor::renderSceneDepth(std::shared_ptr<Light> light);
    void drawToQuad();
    void setupUI();
    void updateUI();
    void run();
    void destroy();
};


#endif //FORGE_EDITOR_H
