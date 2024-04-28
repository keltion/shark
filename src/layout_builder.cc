#include "layout_builder.h"

#include <format>
#include <memory>
#include <optional>
#include <variant>

LayoutBox* buildLayoutTree(const StyleNode* styleNode) {
    LayoutBox* root = nullptr;
    switch (styleNode->display()) {
    case Display::kBlock:
        root = new LayoutBox(BoxType::kBlock, styleNode);
        break;
    case Display::kInline:
        root = new LayoutBox(BoxType::kInline, styleNode);
        break;
    case Display::kNone:
        return nullptr;
        break;
    }

    for (const StyleNode& child : styleNode->children()) {
        switch (child.display()) {
        case Display::kBlock: {
            LayoutBox* childLayoutBox = buildLayoutTree(&child);
            if (childLayoutBox) {
                root->addChild(std::unique_ptr<LayoutBox>(childLayoutBox));
            }
        }
        break;
        case Display::kInline: {
            root->getInlineContainer().addChild(std::unique_ptr<LayoutBox>(buildLayoutTree(&child)));
        }
        break;
        }
    }
    return root;
}

LayoutBox& LayoutBox::getInlineContainer() {
    if (type_ != BoxType::kBlock) {
        return *this;
    }

    if (children_.empty() || children_.back()->type_ != BoxType::kAnonymousBlock) {
        addChild(std::make_unique<LayoutBox>(BoxType::kAnonymousBlock));
    }

    return *children_.back();
}

void LayoutBox::setStyleNode(StyleNode *styleNode) {
    styleNode_ = styleNode;
}

bool LayoutBox::isWidthAuto() const
{
    const auto optionalValue = styleNode_->value(CssProperties::kWidth);
    if (!optionalValue)
    {
        return false;
    }
    return true;
}
void LayoutBox::calculateBlockWidth(const Dimensions& containingBlock) {

    const auto autoLength = Length(0.0, Unit::Px, true);
    Value width = styleNode_->value(CssProperties::kWidth).value_or(autoLength);


    const auto zeroLength = Length(0.0, Unit::Px);
    Value marginLeft = styleNode_->lookup(CssProperties::kMarginLeft, CssProperties::kMargin, zeroLength);
    Value marginRight = styleNode_->lookup(CssProperties::kMarginRight, CssProperties::kMargin, zeroLength);

    Value borderLeft = styleNode_->lookup(CssProperties::kBorderLeftWidth, CssProperties::kBorderWidth, zeroLength);
    Value borderRight = styleNode_->lookup(CssProperties::kBorderRightWidth, CssProperties::kBorderWidth, zeroLength);

    Value paddingLeft = styleNode_->lookup(CssProperties::kPaddingLeft, CssProperties::kPadding, zeroLength);
    Value paddingRight = styleNode_->lookup(CssProperties::kPaddingRight, CssProperties::kPadding, zeroLength);

    if (!std::get_if<Length>(&width) 
        || !std::get_if<Length>(&marginLeft) || !std::get_if<Length>(&marginRight)
        || !std::get_if<Length>(&borderLeft) || !std::get_if<Length>(&borderRight) 
        || !std::get_if<Length>(&paddingLeft) || !std::get_if<Length>(&paddingRight))
    {
        exit(0); // @@@
    }

    std::vector<Value> valuePool{ width, marginLeft, marginRight, borderLeft, borderRight, paddingLeft, paddingRight };
    float totalWidth = 0;
    for (const auto& value : valuePool)
    {
        const Length* length = std::get_if<Length>(&value);
        if (!length)
        {
            continue;
        }
        totalWidth += length->value.first;
    }

    const float underflow = containingBlock.content.width - totalWidth;

    const bool isWidthAuto = (std::get_if<Length>(&width)->autoValue);
    const bool isMarginLeftAuto = (std::get_if<Length>(&marginLeft)->autoValue);
    const bool isMarginRightAuto = (std::get_if<Length>(&marginRight)->autoValue);

    if (!isWidthAuto && !isMarginLeftAuto && !isMarginRightAuto)
    {
        marginRight = Length(std::get<Length>(marginRight).value.first + underflow, Unit::Px);
    }
    else if (!isWidthAuto && !isMarginLeftAuto && isMarginRightAuto)
    {
        marginRight = Length(underflow, Unit::Px);
    }
    else if (!isWidthAuto && isMarginLeftAuto && !isMarginRightAuto)
    {
        marginLeft = Length(underflow, Unit::Px);
    }
    else if (!isWidthAuto && isMarginLeftAuto && isMarginRightAuto)
    {
        if (underflow >= 0.0)
        { 
            marginRight = Length(underflow / 2, Unit::Px);
            marginLeft = Length(underflow / 2, Unit::Px);
        }
        else 
        {
            marginLeft = Length(0.0, Unit::Px);
            marginRight = Length(0.0, Unit::Px);
        }
    }
    else if (isWidthAuto)
    {
        if (isMarginRightAuto)
        {
            marginRight = Length(0.0, Unit::Px);
        }

        if (isMarginLeftAuto)
        {
            marginLeft = Length(0.0, Unit::Px);
        }

        if (underflow >= 0.0)
        {
            width = Length(underflow, Unit::Px);
        }
        else 
        {
            width = Length(0.0, Unit::Px);
            marginRight = Length(std::get<Length>(marginRight).value.first + underflow, Unit::Px);
        }
    }
}

void LayoutBox::calculateBlockPosition(const Dimensions& containingBlock) {

    const auto zeroLength = Length(0.0, Unit::Px);


    auto marginTopValue = styleNode_->lookup(CssProperties::kMarginTop, CssProperties::kMargin, zeroLength);
    auto* marginTopLength = std::get_if<Length>(&marginTopValue);
    if (!marginTopLength) {
        dimensions_.margin.top = marginTopLength->value.first;
    }

    auto marginBottomValue = styleNode_->lookup(CssProperties::kMarginBottom, CssProperties::kMargin, zeroLength);
    auto* marginBottomLength = std::get_if<Length>(&marginBottomValue);
    if (!marginBottomLength) {
        dimensions_.margin.bottom = marginBottomLength->value.first;
    }

    auto borderTopValue = styleNode_->lookup(CssProperties::kBorderTopWidth, CssProperties::kBorderWidth, zeroLength);
    auto* borderTopLength = std::get_if<Length>(&borderTopValue);
    if (!borderTopLength) {
        dimensions_.margin.top = borderTopLength->value.first;
    }

    auto borderBottomValue = styleNode_->lookup(CssProperties::kMarginBottom, CssProperties::kMargin, zeroLength);
    auto* borderBottomLength = std::get_if<Length>(&borderBottomValue);
    if (!borderBottomLength) {
        dimensions_.margin.bottom = borderBottomLength->value.first;
    }

    auto paddingTopValue = styleNode_->lookup(CssProperties::kPaddingTop, CssProperties::kPadding, zeroLength);
    auto* paddingTopLength = std::get_if<Length>(&paddingTopValue);
    if (!paddingTopLength) {
        dimensions_.padding.top = marginTopLength->value.first;
    }

    auto paddingBottomValue = styleNode_->lookup(CssProperties::kPaddingBottom, CssProperties::kPadding, zeroLength);
    auto* paddingBottomLength = std::get_if<Length>(&paddingBottomValue);
    if (!paddingBottomLength) {
        dimensions_.padding.bottom = paddingBottomLength->value.first;
    }

    dimensions_.content.x = containingBlock.content.x +
        dimensions_.margin.left + dimensions_.border.left + dimensions_.padding.left;

    dimensions_.content.y = containingBlock.content.height + containingBlock.content.y +
        dimensions_.margin.top + dimensions_.border.top + dimensions_.padding.top;

}

void LayoutBox::layoutBlockChildern() {
    for (auto &child : children_) {
        child->layout(dimensions_);
        dimensions_.content.height = dimensions_.content.height + child->dimensions().marginBox().height;
    }
}

void LayoutBox::calculateBlockHeight() {
    auto heightValue = styleNode_->value(CssProperties::kHeight);
    if (!heightValue || !std::get_if<Length>(&*heightValue))
    {
        return;
    }

    if (std::get_if<Length>(&*heightValue)->value.second == Unit::Px) {
        dimensions_.content.height = std::get_if<Length>(&*heightValue)->value.first;
    }
}

const Dimensions& LayoutBox::dimensions() const {
    return dimensions_;
}

void LayoutBox::layout(const Dimensions& containingBlock) {
    switch (type_) {
    case BoxType::kBlock:
        layoutBlock(containingBlock);
        break;
    }
}

void LayoutBox::layoutBlock(const Dimensions& containingBlock) {
    calculateBlockWidth(containingBlock);
    calculateBlockPosition(containingBlock);
    layoutBlockChildern();
    calculateBlockHeight();
}
