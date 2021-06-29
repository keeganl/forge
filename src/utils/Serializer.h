//
// Created by keega on 6/28/2021.
//

#ifndef FORGE_SERIALIZER_H
#define FORGE_SERIALIZER_H

#include <vector>
#include <yaml-cpp/yaml.h>
#include "../mesh/Model.h"

class Serializer {
public:
    Serializer(const std::vector<std::shared_ptr<Model>> &c);

    void Serialize(const std::string& filepath);
    bool Deserialize(const std::string& filepath);
private:
    std::vector<std::shared_ptr<Model>> components;


};


#endif //FORGE_SERIALIZER_H
