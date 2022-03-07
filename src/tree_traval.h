#pragma once

#include <iostream>
#include <string>

#include "dom.h"

void inorder(Node* node, std::string deep) {
    if(node->Type == "TextNode") {
        TextNode* TNode =static_cast<TextNode*>(node);
        std::cout << deep + TNode->GetText() << std::endl;
        deep += "  ";
        for(auto& child : TNode->childern) {
            inorder(child, deep);
        }
    }
    else {
        Element* ENode = static_cast<Element*>(node);
        std::cout << deep + ENode->GetTagName() << std::endl;
        deep += "  ";
        for(auto& child : ENode->childern) {
            inorder(child, deep);
        }
    }


}
