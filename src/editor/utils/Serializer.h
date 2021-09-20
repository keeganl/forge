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
#include "../scene/Scene.h"

class Serializer {
public:
    Serializer(const Scene &scene);

    void Serialize(const std::string& filepath);
    Scene Deserialize(const std::string& filepath);
private:
    Scene scene;


};


#endif //FORGE_SERIALIZER_H
