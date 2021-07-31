//
// Created by keega on 7/25/2021.
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

#include "imgui-layer/GuiLayer.h"
#include "utils/shader-manager/Shader.h"
#include "entity/camera/EditorCamera.h"
#include "entity/mesh/Model.h"
#include "entity/light/Light.h"
#include "utils/Serializer.h"
#include "renderer/window/Window.h"

#include <iostream>
#include <windows.h>
#include <tchar.h>


class Editor {
public:
    Editor();

    void run();
};


#endif //FORGE_EDITOR_H
