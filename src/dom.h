#pragma once

#include "utils.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum class ElementType{
    kElement,
    kTextNode,
};

class DomNode {
public:
    DomNode() = default;
    DomNode(ElementType type) : type_(type), parent_{ nullptr } {}
    //virtual ~DomNode() { delete parent_; }; TODO : apply smart pointer
    virtual std::string GetElementData() const = 0;
    void addChild(std::unique_ptr<DomNode> node);
    void setParent(DomNode* node);
    DomNode * parent() const;
    const std::vector<std::unique_ptr<DomNode>>& childern() const;
    ElementType type() const;
    virtual bool isTextNode() const;
private:
    std::vector<std::unique_ptr<DomNode>> childern_;
    DomNode* parent_;
    ElementType type_;
};

class TextNode : public DomNode {
public:
    TextNode() = default;
    TextNode(std::string text) : text_(text), DomNode(ElementType::kTextNode) {}
    std::string GetElementData() const override {
        return text_;
    }
    std::string GetText() {
        return text_;
    }
    bool isTextNode() const override {
        return true;
    }

private:
    std::string text_;
};

class Element : public DomNode {
public:
    Element() = default;
    Element(std::string tag_name) : tag_name_(tag_name), DomNode(ElementType::kElement) {}// temp

    //Element(std::string tag_name,
    //        std::unordered_map<std::string, std::string> AttrMap)
    //        : tag_name_(tag_name), AttrMap_(AttrMap), DomNode(ElementType::kElement) {}
    std::string GetElementData() const override {
        return tag_name_;
    }
    const std::string& GetTagName() const {
        return tag_name_;
    }
    void PrintAttrMap() const {
        for(auto Attr : AttrMap_) {
            std::cout << Attr.first << " " << Attr.second << "\n";
        }
    }
    const std::string& Id() const;
    const std::unordered_set<std::string> classes() const;
    void setAttributeMap(std::unordered_map<std::string, std::string> &&AttributeMap);
private:
    std::string tag_name_;
    std::unordered_map<std::string, std::string> AttrMap_;
};


class DomTree
{
public:
private:
};