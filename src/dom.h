#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

class Node {
public:
    Node() = default;
    Node(std::string type) : Type(type) {}
    std::vector<Node*> childern;
    std::string Type;
};

class TextNode : public Node {
public:
    TextNode() = default;
    TextNode(std::string text) : text_(text), Node("TextNode") {}
    std::string GetText() {
        return text_;
    }
private:
    std::string text_;
};

class Element : public Node {
public:
    Element() = default;
    Element(std::string tag_name, 
            std::unordered_map<std::string, std::string> AttrMap) : tag_name_(tag_name), AttrMap_(AttrMap), Node("Element") {}
    std::string GetTagName() {
        return tag_name_;
    }
    void PrintAttrMap() {
        for(auto Attr : AttrMap_) {
            std::cout << Attr.first << " " << Attr.second << "\n";
        }
    }
private: 
    std::string tag_name_;
    std::unordered_map<std::string, std::string> AttrMap_;
};