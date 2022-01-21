//
// Created by keega on 8/1/2021.
//

#ifndef FORGE_MODALMANAGER_H
#define FORGE_MODALMANAGER_H

#include "../../../../external/imgui/imgui.h"
#include "../../external/imgui/imfilebrowser.h"


class ModalManager {
public:
    ImGui::FileBrowser fileDialog;
    ImGui::FileBrowser saveDialog;
    ImGui::FileBrowser screenshotDialog;
    ImGui::FileBrowser sceneDialog;
    ImGui::FileBrowser textureDialog;
    ModalManager();

};


#endif //FORGE_MODALMANAGER_H
