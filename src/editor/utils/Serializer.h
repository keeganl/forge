//
// Created by keega on 6/28/2021.
//

#ifndef FORGE_SERIALIZER_H
#define FORGE_SERIALIZER_H

#include <vector>
#include <yaml-cpp/yaml.h>
#include "../entity/mesh/Model.h"
#include "../entity/light/Light.h"
#include "../entity/camera/EditorCamera.h"

class Serializer {
public:
    Serializer(const std::vector<std::shared_ptr<Model>> &c, EditorCamera camera);

    void Serialize(const std::string& filepath);
    std::vector<std::shared_ptr<Model>> Deserialize(const std::string& filepath);
private:
    std::vector<std::shared_ptr<Model>> components;
    EditorCamera cam;


};


#endif //FORGE_SERIALIZER_H
