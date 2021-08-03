//
// Created by keega on 8/1/2021.
//

#ifndef FORGE_KEYMAP_H
#define FORGE_KEYMAP_H
#include <map>
#include <string>


class Keymap {

public:
    std::map<std::string, int> keys;

    Keymap();
    void insertKey(std::string &key, int &val);
    void removeKey(std::string &key);
};


#endif //FORGE_KEYMAP_H
