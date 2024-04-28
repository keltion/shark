#pragma once

#include <vector>
#include <memory>

#include "style_tree_maker.h"


enum class BoxType {
    kBlock,
    kInline,
    kAnonymousBlock,
};


struct EdgeSizes {
    float left;
    float right;
    float top;
    float bottom;

    EdgeSizes() : left(0), right(0), top(0), bottom(0) {}
};

struct Rectangle {
    float x;
    float y;
    float width;
    float height;

    Rectangle() : x(0.0), y(0.0), width(0.0), height(0.0) {}
    Rectangle(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}
    Rectangle expandedBy(EdgeSizes edge) const {
        return Rectangle { x - edge.left, y - edge.top, width + edge.left + edge.right, height + edge.top + edge.bottom };
    }
};

struct Dimensions {
    Rectangle content;
    EdgeSizes padding;
    EdgeSizes border;
    EdgeSizes margin;

    Dimensions() : content(), padding(), border(), margin() {}
    Rectangle paddingBox() const {
        return content.expandedBy(padding);
    }
    Rectangle borderBox() const {
        return paddingBox().expandedBy(border);
    }
    Rectangle marginBox() const {
        return borderBox().expandedBy(margin);
    }

};


class LayoutBox {
public:
    LayoutBox() = delete;
    LayoutBox(BoxType type) : type_(type) {}
    LayoutBox(BoxType type, const StyleNode *styleNode) : type_(type), styleNode_(styleNode) {
        children_.reserve(50);
    }
    LayoutBox& getInlineContainer();
    void setStyleNode(StyleNode *styleNode);
    void addChild(std::unique_ptr<LayoutBox> child) {
        children_.emplace_back(std::move(child));
    }
    void layout(const Dimensions& containingBlock);
    void layoutBlock(const Dimensions& containingBlock);
    void calculateBlockWidth(const Dimensions& containingBlock);
    std::vector<Value> getWidthValues() const;
    bool isWidthAuto() const;
    void calculateBlockPosition(const Dimensions& containingBlock);
    void layoutBlockChildern();
    void calculateBlockHeight();
    const Dimensions &dimensions() const;

private:
    const StyleNode* styleNode_;
    Dimensions dimensions_;
    BoxType type_;
    std::vector<std::unique_ptr<LayoutBox>> children_;
};

LayoutBox* buildLayoutTree(const StyleNode* styleNode);


class LayoutBuilder {
public:
private:

};
