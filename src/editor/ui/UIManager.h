//
// Created by keega on 8/2/2021.
//

#ifndef FORGE_UIMANAGER_H
#define FORGE_UIMANAGER_H


#include "../window/Window.h"
#include "imgui-layer/GuiLayer.h"
#include "textures/UITexture.h"

class UIManager {
public:
    Window window;
    ModalManager modalManager;
    Settings settings;
    std::map<std::string, UITexture> uiTextures;

    UIManager();

private:
    void loadDefaultTextures();
};


#endif //FORGE_UIMANAGER_H
