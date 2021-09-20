//
// Created by keega on 6/28/2021.
//

#include "Serializer.h"
#include <glm/gtx/string_cast.hpp>

Serializer::Serializer(const Scene &scene)
        : scene(scene)
{}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
{
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
    return out;
}

void Serializer::Serialize(const std::string &filepath) {
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << filepath.substr(filepath.find_last_of('/')+1);
    out << YAML::Key << "Models" << YAML::Value << YAML::BeginSeq;
    int i = 0;
    for (std::shared_ptr<Model> &m : scene.models) {
        out << YAML::BeginMap;
        out << YAML::Key << "Model" << YAML::Value;
        out << YAML::BeginMap;
        out << YAML::Key << "Index" << YAML::Value << i;
        out << YAML::Key << "Name" << YAML::Value << m->modelName;
        out << YAML::Key << "ObjectType" << YAML::Value << m->objectType;
        out << YAML::Key << "Directory" << YAML::Value << m->directory;
        out << YAML::Key << "Path" << YAML::Value << m->modelPath;
        out << YAML::Key << "Position" << YAML::Value << m->pos;
        out << YAML::Key << "Color" << YAML::Value << m->color;
        out << YAML::Key << "ModelMatrix" << YAML::Value << glm::to_string(m->modelMatrix);
        out << YAML::Key << "Axis Scale" << YAML::Value << m->scaleAxes;
        out << YAML::Key << "Rotation Axis Values" << YAML::Value << m->rotateFloats;
        out << YAML::EndMap;
        out << YAML::EndMap;
        i++;
    }
    out << YAML::EndSeq;


    out << YAML::Key << "Lights" << YAML::Value << YAML::BeginSeq;
    int j = 0;
    for (std::shared_ptr<Light> &l : scene.lights) {
        out << YAML::BeginMap;
        out << YAML::Key << "Light" << YAML::Value;
        out << YAML::BeginMap;
        out << YAML::Key << "Index" << YAML::Value << j;
        out << YAML::Key << "Name" << YAML::Value << l->modelName;
        out << YAML::Key << "ObjectType" << YAML::Value << l->objectType;
        out << YAML::Key << "Directory" << YAML::Value << l->directory;
        out << YAML::Key << "Path" << YAML::Value << l->modelPath;
        out << YAML::Key << "Position" << YAML::Value << l->pos;
        out << YAML::Key << "Color" << YAML::Value << l->color;
        out << YAML::Key << "ModelMatrix" << YAML::Value << glm::to_string(l->modelMatrix);
        out << YAML::Key << "Axis Scale" << YAML::Value << l->scaleAxes;
        out << YAML::Key << "Rotation Axis Values" << YAML::Value << l->rotateFloats;
        out << YAML::EndMap;
        out << YAML::EndMap;
        j++;
    }
    out << YAML::EndSeq;

    out << YAML::Key << "Camera" << YAML::Value;
    out << YAML::BeginMap;
    out << YAML::Key << "FOV" << YAML::Value << scene.camera.fov;
    out << YAML::Key << "Speed" << YAML::Value << scene.camera.speed;
    out << YAML::Key << "Position" << YAML::Value << scene.camera.pos;
    out << YAML::Key << "Target" << YAML::Value << scene.camera.target;
    out << YAML::Key << "Direction" << YAML::Value << scene.camera.direction;
    out << YAML::Key << "Front" << YAML::Value << scene.camera.front;
    out << YAML::Key << "Up" << YAML::Value << scene.camera.up;
    out << YAML::EndMap;
    out << YAML::EndMap;
    std::cout << "in serializer: " << filepath << std::endl;
    std::ofstream fout(filepath);
    fout << out.c_str();
}

Scene Serializer::Deserialize(const std::string &filepath) {
//    std::cout << "!!!" << filepath << std::endl;
    YAML::Node data = YAML::LoadFile(filepath);
//    if (!data["Scene"])
//        return false;

    Scene newScene;

    std::string sceneName = data["Scene"].as<std::string>();

    auto models = data["Models"];
    auto lights = data["Lights"];
    auto camera = data["Camera"];
    if (models) {
        for (auto m : models) {
            newScene.models.push_back(std::make_shared<Model>(
                    m["Model"]["Path"].as<std::string>(),
                    false,
                    m["Model"]["ObjectType"].as<std::string>(),
                    glm::vec4(m["Model"]["Color"][0].as<float>(), m["Model"]["Color"][1].as<float>(), m["Model"]["Color"][2].as<float>(), m["Model"]["Color"][3].as<float>()),
                    glm::vec3(m["Model"]["Position"][0].as<float>(), m["Model"]["Position"][1].as<float>(), m["Model"]["Position"][2].as<float>()),
                    glm::mat4(1.0f),
                    glm::vec3(m["Model"]["Rotation Axis Values"][0].as<float>(), m["Model"]["Rotation Axis Values"][1].as<float>(), m["Model"]["Rotation Axis Values"][2].as<float>()),
                    glm::vec3(m["Model"]["Axis Scale"][0].as<float>(), m["Model"]["Axis Scale"][1].as<float>(), m["Model"]["Axis Scale"][2].as<float>()),
                    1.0f));
        }
    }
    if (lights) {
        for (auto l : lights) {
            newScene.lights.push_back(std::make_shared<Light>(
                    l["Light"]["Path"].as<std::string>(),
                    false,
                    l["Light"]["ObjectType"].as<std::string>(),
                    glm::vec4(l["Light"]["Color"][0].as<float>(), l["Light"]["Color"][1].as<float>(), l["Light"]["Color"][2].as<float>(), l["Light"]["Color"][3].as<float>()),
                    glm::vec3(l["Light"]["Position"][0].as<float>(), l["Light"]["Position"][1].as<float>(), l["Light"]["Position"][2].as<float>()),
                    glm::mat4(1.0f),
                    glm::vec3(l["Light"]["Rotation Axis Values"][0].as<float>(), l["Light"]["Rotation Axis Values"][1].as<float>(), l["Light"]["Rotation Axis Values"][2].as<float>()),
                    glm::vec3(l["Light"]["Axis Scale"][0].as<float>(), l["Light"]["Axis Scale"][1].as<float>(), l["Light"]["Axis Scale"][2].as<float>()),
                        1.0f));
        }
    }
    if (camera) {
        newScene.camera = Camera(
                camera["FOV"].as<float>(),
                camera["Speed"].as<float>(),
                glm::vec3(camera["Position"][0].as<float>(), camera["Position"][1].as<float>(), camera["Position"][2].as<float>()),
                glm::vec3(camera["Target"][0].as<float>(), camera["Target"][1].as<float>(), camera["Target"][2].as<float>()),
                glm::vec3(camera["Direction"][0].as<float>(), camera["Direction"][1].as<float>(), camera["Direction"][2].as<float>()),
                glm::vec3(camera["Front"][0].as<float>(), camera["Front"][1].as<float>(), camera["Front"][2].as<float>()),
                glm::vec3(camera["Up"][0].as<float>(), camera["Up"][1].as<float>(), camera["Up"][2].as<float>()));
    }
    return newScene;
}