//
// Created by keega on 8/1/2021.
//

#include "Settings.h"

Settings::Settings() {
    this->keymap = Keymap();
}

Settings::Settings(const Keymap &keymap, float nearClipping, float farClipping, float padding, float thumbnailSize,
                   bool firstMouse, bool showSettingsWindow, bool openScene, bool saveScene, bool openSavePopup,
                   bool openFilePopup, bool openSettingsPopup, float deltaTime, float lastFrame,
                   const std::filesystem::path &startingDirectory, const std::filesystem::path &currentDirectory)
        : keymap(keymap), nearClipping(nearClipping), farClipping(farClipping), padding(padding),
          thumbnailSize(thumbnailSize), firstMouse(firstMouse), showSettingsWindow(showSettingsWindow),
          openScene(openScene), saveScene(saveScene), openSavePopup(openSavePopup), openFilePopup(openFilePopup),
          openSettingsPopup(openSettingsPopup), deltaTime(deltaTime), lastFrame(lastFrame),
          startingDirectory(startingDirectory), currentDirectory(currentDirectory) {}
