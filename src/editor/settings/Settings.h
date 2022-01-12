//
// Created by keega on 8/1/2021.
//

#ifndef FORGE_SETTINGS_H
#define FORGE_SETTINGS_H


#include <filesystem>
#include "keymap/Keymap.h"
#include "../utils/SerializableInterface.h"

class Settings : public SerializableInterface {
public:
    Keymap keymap;
    float nearClipping = 0.1f;
    float farClipping = 1000.0f;
    float padding = 16.0f;
    float thumbnailSize = 48.0f;
    float exposure = 1.0f;
    bool hdr = true;
    bool firstMouse = true;
    bool showDebugWindows = true;
    bool showSettingsWindow = false;
    bool openScene = false;
    bool saveScene = false;
    bool clearScenePopup = false;
    bool openSavePopup = false;
    bool openFilePopup = false;
    bool openSettingsPopup = false;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    std::filesystem::path startingDirectory = "C:\\Users\\keega\\Desktop\\school\\grad\\research\\forge\\assets";
    std::filesystem::path currentDirectory = "C:\\Users\\keega\\Desktop\\school\\grad\\research\\forge\\assets";

    Settings();
    Settings(const Keymap &keymap, float nearClipping, float farClipping, float padding, float thumbnailSize,
             bool firstMouse, bool showSettingsWindow, bool openScene, bool saveScene, bool openSavePopup,
             bool openFilePopup, bool openSettingsPopup, float deltaTime, float lastFrame,
             const std::filesystem::path &startingDirectory, const std::filesystem::path &currentDirectory);

    void serialize(const std::string &filepath) override;
    void deserialize() override;
};


#endif //FORGE_SETTINGS_H
