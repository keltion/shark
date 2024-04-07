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
    constexpr char kDisplay[] = "display";
};

namespace CssPropertyValues {
    constexpr char kDisplayBlock[] = "block";
    constexpr char kDisplayInline[] = "inline";
    constexpr char kDisplayNone[] = "none";
};