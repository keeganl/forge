//
// Created by keega on 8/1/2021.
//

#include <glm/vec4.hpp>
#include <iostream>
#include <fstream>
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


//YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
//{
//    out << YAML::Flow;
//    out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
//    return out;
//}
//
//YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
//    out << YAML::Flow;
//    out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
//    return out;
//}

void Settings::serialize(const std::string &filepath) {
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Settings" << YAML::Value << YAML::BeginMap;

    out << YAML::Key << "keymap" << YAML::Value << YAML::BeginSeq;
    for (auto const& [key, val] : keymap.keys) {
        out << YAML::BeginMap;
        out << YAML::Key << key << YAML::Value << val;
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;
    out << YAML::Key << "nearClipping" << YAML::Value << nearClipping;
    out << YAML::Key << "farClipping" << YAML::Value << farClipping;
    out << YAML::Key << "padding" << YAML::Value << padding;
    out << YAML::Key << "thumbnailSize" << YAML::Value << thumbnailSize;
    out << YAML::Key << "showDebugWindows" << YAML::Value << showDebugWindows;
    out << YAML::Key << "showSettingsWindow" << YAML::Value << showSettingsWindow;
    out << YAML::Key << "startingDirectory" << YAML::Value << startingDirectory.string();
    out << YAML::Key << "currentDirectory" << YAML::Value << currentDirectory.string();
    out << YAML::Key << "hdr" << YAML::Value << hdr;
    out << YAML::Key << "exposure" << YAML::Value << currentDirectory.string();

    out << YAML::EndMap;

    std::ofstream fout(filepath);
    fout << out.c_str();
}

void Settings::deserialize() {

}
