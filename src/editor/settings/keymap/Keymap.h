//
// Created by keega on 8/1/2021.
//

#ifndef FORGE_KEYMAP_H
#define FORGE_KEYMAP_H
#include <map>
#include <string>


class Keymap {

public:
    Keymap();

    std::map<std::string, int> keys;
};


#endif //FORGE_KEYMAP_H
