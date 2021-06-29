//
// Created by keega on 6/28/2021.
//

#include "Serializer.h"
Serializer::Serializer(const std::vector<std::shared_ptr<Model>> &c)
        : components(c)
{
}

void Serializer::Serialize(const std::string &filepath) {
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << "Untitled";
    out << YAML::Key << "Test" << YAML::Value << "Test";
    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream fout(filepath);
    fout << out.c_str();
}