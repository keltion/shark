#pragma once

#include <iostream>
#include <string>

#include "dom.h"

void inorder(Node* node, std::string deep) {
    std::cout << deep + node->GetElementData() << std::endl;
    deep += "  ";
    for(auto& child : node->childern) {
        inorder(child, deep);
    }
}