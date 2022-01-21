//
// Created by keega on 8/1/2021.
//

#include "Keymap.h"

Keymap::Keymap() {
    this->keys.insert(std::pair<std::string, int> ("w", 87));
    this->keys.insert(std::pair<std::string, int> ("a", 65));
    this->keys.insert(std::pair<std::string, int> ("s", 83));
    this->keys.insert(std::pair<std::string, int> ("d", 68));
    this->keys.insert(std::pair<std::string, int> ("r", 82));
    this->keys.insert(std::pair<std::string, int> ("t", 84));
    this->keys.insert(std::pair<std::string, int> ("y", 89));
    this->keys.insert(std::pair<std::string, int> ("o", 79));
    this->keys.insert(std::pair<std::string, int> (",", 44));
    this->keys.insert(std::pair<std::string, int> ("del", 261));
    this->keys.insert(std::pair<std::string, int> ("1", 49));
    this->keys.insert(std::pair<std::string, int> ("2", 50));
    this->keys.insert(std::pair<std::string, int> ("3", 51));
    this->keys.insert(std::pair<std::string, int> ("5", 53));
}

void Keymap::removeKey(std::string &key)
{
    this->keys.erase(key);
}


void Keymap::insertKey(std::string &key, int &val)
{
    this->keys.insert(std::pair<std::string, int> (key, val));
}
