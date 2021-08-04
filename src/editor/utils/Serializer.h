//
// Created by keega on 6/28/2021.
//

#ifndef FORGE_SERIALIZER_H
#define FORGE_SERIALIZER_H

#include <vector>
#include <yaml-cpp/yaml.h>
#include "../mesh/Model.h"
#include "../camera/Camera.h"
#include "../light/Light.h"

class Serializer {
public:
    Serializer(const std::vector<std::shared_ptr<Model>> &c, const Camera &camera);

    void Serialize(const std::string& filepath);
    std::vector<std::shared_ptr<Model>> Deserialize(const std::string& filepath);
private:
    std::vector<std::shared_ptr<Model>> components;
    Camera cam;


};


#endif //FORGE_SERIALIZER_H
