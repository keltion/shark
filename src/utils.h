#pragma once

#include <string>

#include "css_parser.h"
#include "dom.h"

namespace Symbols {
    constexpr char kLeftAngleBracket = '<';
    constexpr char kRightAngleBracket = '>';
    constexpr char kLeftSquareBracket = '{';
    constexpr char kRightSquareBracket = '}';
    constexpr char kSemicolon = ';';
    constexpr char kColon = ':';
    constexpr char kSpace = ' ';
    constexpr char kSlash = '/';
    constexpr char kQuote = '"';
    constexpr char kEqual = '=';
    constexpr char kSharp = '#';
    constexpr char kDot = '.';
    constexpr char kEndOfFile = '\0';
}

//void inorder(Node* node, const Stylesheet &stylesheet);
//void inorder(Node* node, std::string deep);
namespace CssProperties {
    constexpr char kDisplay[]               = "display";
    constexpr char kWidth[]                 = "width";
    constexpr char kMargin[]                = "margin";
    constexpr char kMarginTop[]             = "margin-top";
    constexpr char kMarginBottom[]          = "margin-bottom";
    constexpr char kMarginLeft[]            = "margin-left";
    constexpr char kMarginRight[]           = "margin-right";
    constexpr char kBorderWidth[]           = "border-width";
    constexpr char kBorderTopWidth[]        = "border-top-width";
    constexpr char kBorderBottomWidth[]     = "border-bottom-width";
    constexpr char kBorderLeftWidth[]       = "border-left-width";
    constexpr char kBorderRightWidth[]      = "border-right-width";
    constexpr char kPadding[]               = "padding";
    constexpr char kPaddingTop[]            = "padding-top";
    constexpr char kPaddingBottom[]         = "padding-bottom";
    constexpr char kPaddingLeft[]           = "padding-left";
    constexpr char kPaddingRight[]          = "padding-right";
    constexpr char kHeight[]                = "height";
};

namespace CssPropertyValues {
    constexpr char kDisplayBlock[]  = "block";
    constexpr char kDisplayInline[] = "inline";
    constexpr char kDisplayNone[]   = "none";
};