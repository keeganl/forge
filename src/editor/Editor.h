//
// Created by keega on 8/1/2021.
//

#ifndef FORGE_EDITOR_H
#define FORGE_EDITOR_H

#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_glfw.h"
#include "../../external/imgui/imgui_impl_opengl3.h"
#include "../../external/imgui/imgui_internal.h"
#include "../../external/imgui/imfilebrowser.h"

#include "../../external/stb/stb_image.h"

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
#include "utils/shader-manager/Shader.h"
#include "camera/Camera.h"
#include "mesh/Model.h"
#include "light/Light.h"
#include "settings/Settings.h"
#include "utils/Serializer.h"

#include <iostream>
#include <windows.h>
#include <tchar.h>

class Editor {
public:
    Editor();

    void run();
};


#endif //FORGE_EDITOR_H
