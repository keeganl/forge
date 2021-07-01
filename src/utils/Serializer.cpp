//
// Created by keega on 6/28/2021.
//

#include "Serializer.h"
#include <glm/gtx/string_cast.hpp>

Serializer::Serializer(const std::vector<std::shared_ptr<Model>> &c)
        : components(c)
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
        out << YAML::Key << "Name" << YAML::Value << m->modelName;
        out << YAML::Key << "ObjectType" << YAML::Value << m->objectType;
        out << YAML::Key << "Directory" << YAML::Value << m->directory;
        out << YAML::Key << "Position" << YAML::Value << m->pos;
        out << YAML::Key << "Color" << YAML::Value << m->color;
        out << YAML::Key << "ModelMatrix" << YAML::Value << glm::to_string(m->modelMatrix);
        out << YAML::Key << "Axis Scale" << YAML::Value << m->scaleAxes;
        out << YAML::Key << "Rotation Axis Values" << YAML::Value << m->scaleAxes;
        out << YAML::EndMap;
        out << YAML::EndMap;
        i++;

    }
    out << YAML::EndSeq;
    out << YAML::EndMap;
    std::cout << "in serializer: " << filepath << std::endl;
    std::ofstream fout( filepath);
    fout << out.c_str();
}