//
// Created by keega on 6/28/2021.
//

#include "Serializer.h"
#include <glm/gtx/string_cast.hpp>
#include "../entity/EntityType.h"

Serializer::Serializer(const std::vector<std::shared_ptr<Model>> &c, EditorCamera camera)
        : components(c), cam(camera)
{
}

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
    out << YAML::Key << "Components" << YAML::Value << YAML::BeginSeq;
    int i = 0;
    for (std::shared_ptr<Model> &m : components) {
        out << YAML::BeginMap;
        out << YAML::Key << "Model" << YAML::Value;
        out << YAML::BeginMap;
        out << YAML::Key << "Index" << YAML::Value << i;
        out << YAML::Key << "Name" << YAML::Value << m->getName();
        out << YAML::Key << "ObjectType" << YAML::Value << m->getType();
        out << YAML::Key << "Directory" << YAML::Value << m->getDirectory();
        out << YAML::Key << "Path" << YAML::Value << m->getPath();
        out << YAML::Key << "Position" << YAML::Value << m->getPosition();
        out << YAML::Key << "Color" << YAML::Value << m->getColor();
        out << YAML::Key << "ModelMatrix" << YAML::Value << glm::to_string(m->getModelMatrix());
        out << YAML::Key << "Axis Scale" << YAML::Value << m->getScale();
        out << YAML::Key << "Rotation Axis Values" << YAML::Value << m->getRotation();
        out << YAML::EndMap;
        out << YAML::EndMap;
        i++;
    }
    out << YAML::EndSeq;
    /*
     * float fov{45.0f};
        float speed{2.5f};
        glm::vec3 pos{glm::vec3(1.0f, 2.0f, 3.0f)};
        glm::vec3 target{glm::vec3(0.0f, 1.0f, 0.0f)};
        glm::vec3 direction{glm::normalize(glm::vec3(0.0f, 0.0f, 3.0f) - glm::vec3(0.0f, 0.0f, 0.0f))};
        glm::vec3 front{glm::vec3(0.0f, 0.0f, -1.0f)};
        glm::vec3 up{glm::vec3(0.0f, 1.0f,  0.0f)};
     */
    out << YAML::Key << "Camera" << YAML::Value;
    out << YAML::BeginMap;
    out << YAML::Key << "FOV" << YAML::Value << cam.fov;
    out << YAML::Key << "Speed" << YAML::Value << cam.speed;
    out << YAML::Key << "Position" << YAML::Value << cam.pos;
    out << YAML::Key << "Target" << YAML::Value << cam.target;
    out << YAML::Key << "Direction" << YAML::Value << cam.direction;
    out << YAML::Key << "Front" << YAML::Value << cam.front;
    out << YAML::Key << "Up" << YAML::Value << cam.up;
    out << YAML::EndMap;
    out << YAML::EndMap;
    std::cout << "in serializer: " << filepath << std::endl;
    std::ofstream fout(filepath);
    fout << out.c_str();
}

std::vector<std::shared_ptr<Model>> Serializer::Deserialize(const std::string &filepath) {
//    std::cout << "!!!" << filepath << std::endl;
    YAML::Node data = YAML::LoadFile(filepath);
//    if (!data["Scene"])
//        return false;

    std::string sceneName = data["Scene"].as<std::string>();

    auto entities = data["Components"];
    if (entities) {
        for (auto m : entities) {
            if (m["Model"]["ObjectType"].as<std::string>() == "MODEL") {
                components.push_back(std::make_shared<Model>(
                        m["Model"]["Path"].as<std::string>(),
                        false,
                        MODEL,
                        glm::vec4(m["Model"]["Color"][0].as<float>(), m["Model"]["Color"][1].as<float>(), m["Model"]["Color"][2].as<float>(), m["Model"]["Color"][3].as<float>()),
                        glm::vec3(m["Model"]["Position"][0].as<float>(), m["Model"]["Position"][1].as<float>(), m["Model"]["Position"][2].as<float>()),
                        glm::mat4(1.0f),
                        glm::vec3(m["Model"]["Rotation Axis Values"][0].as<float>(), m["Model"]["Rotation Axis Values"][1].as<float>(), m["Model"]["Rotation Axis Values"][2].as<float>()),
                        glm::vec3(m["Model"]["Axis Scale"][0].as<float>(), m["Model"]["Axis Scale"][1].as<float>(), m["Model"]["Axis Scale"][2].as<float>()),
                        1.0f));

            } else if (m["Model"]["ObjectType"].as<std::string>() == "LIGHT") {
                components.push_back(std::make_shared<Light>(
                        m["Model"]["Path"].as<std::string>(),
                        false,
                        LIGHT,
                        glm::vec4(m["Model"]["Color"][0].as<float>(), m["Model"]["Color"][1].as<float>(), m["Model"]["Color"][2].as<float>(), m["Model"]["Color"][3].as<float>()),
                        glm::vec3(m["Model"]["Position"][0].as<float>(), m["Model"]["Position"][1].as<float>(), m["Model"]["Position"][2].as<float>()),
                        glm::mat4(1.0f),
                        glm::vec3(m["Model"]["Rotation Axis Values"][0].as<float>(), m["Model"]["Rotation Axis Values"][1].as<float>(), m["Model"]["Rotation Axis Values"][2].as<float>()),
                        glm::vec3(m["Model"]["Axis Scale"][0].as<float>(), m["Model"]["Axis Scale"][1].as<float>(), m["Model"]["Axis Scale"][2].as<float>()),
                        1.0f));
            }
        }
        return components;
    } else {
        return std::vector<std::shared_ptr<Model>>();
    }
}