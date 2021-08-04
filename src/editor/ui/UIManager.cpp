//
// Created by keega on 8/2/2021.
//

#include "UIManager.h"


UIManager::UIManager() {
    GuiLayer::createContext(this->window.windowInstance);
    UIManager::loadDefaultTextures();
}


void UIManager::loadDefaultTextures() {
    uiTextures.insert(std::pair<std::string, UITexture>("folder", UITexture("../assets/editor/folder.png")));
    uiTextures.insert(std::pair<std::string, UITexture>("file", UITexture("../assets/editor/file.png")));
    uiTextures.insert(std::pair<std::string, UITexture>("backArrow", UITexture("../assets/editor/back_arrow.png")));
    uiTextures.insert(std::pair<std::string, UITexture>("yml", UITexture("../assets/editor/yml.png")));
    uiTextures.insert(std::pair<std::string, UITexture>("default", UITexture("../assets/editor/no_texture.jpg")));

}

