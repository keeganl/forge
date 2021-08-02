//
// Created by keega on 8/1/2021.
//

#include "Keymap.h"

Keymap::Keymap() {
    keys.insert(std::pair<std::string, int> ("w", 87));
    keys.insert(std::pair<std::string, int> ("a", 65));
    keys.insert(std::pair<std::string, int> ("s", 83));
    keys.insert(std::pair<std::string, int> ("d", 68));
    keys.insert(std::pair<std::string, int> ("o", 79));
    keys.insert(std::pair<std::string, int> (",", 44));
    keys.insert(std::pair<std::string, int> ("del", 261));
    keys.insert(std::pair<std::string, int> ("1", 49));
    keys.insert(std::pair<std::string, int> ("2", 50));
    keys.insert(std::pair<std::string, int> ("3", 51));
}
