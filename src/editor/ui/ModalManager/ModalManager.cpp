//
// Created by keega on 8/1/2021.
//

#include "ModalManager.h"

ModalManager::ModalManager() {
    this->fileDialog = ImGui::FileBrowser();
    this->saveDialog = ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_CreateNewDir);
    this->sceneDialog = ImGui::FileBrowser();
    this->textureDialog = ImGui::FileBrowser();
    // (optional) set browser properties
    fileDialog.SetTitle("Select Mesh");
    fileDialog.SetTypeFilters({ ".obj", ".fbx" });

    sceneDialog.SetTitle("Select Scene File");
    sceneDialog.SetTypeFilters({ ".yml" });

    saveDialog.SetTitle("Save Scene File");
    saveDialog.SetTypeFilters({ ".yml" });

    textureDialog.SetTitle("Select Texture File");
    textureDialog.SetTypeFilters({ ".png", ".jpg" });
}
