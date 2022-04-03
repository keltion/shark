#pragma once

#include <iostream>
#include <string>
#include <vector>

class HTMLToken {
public:  
    enum TokenType {
        kUinitialized,
        kStartTag,
        kEndTag,
        kCharacter,
        kEndOfFile,
    };

    HTMLToken() : type_(kUinitialized) {}
    HTMLToken(TokenType T) : type_(T) {}
    TokenType GetType() { return type_; }
    std::string TagName() { return data_; }
    void AppendToCharater(char c) { data_ += c; }
    void AppendToAttributeName(char c) { attribute_name_ += c; }
    void AppendToAttributeValue(char c) { attribute_value_ += c; }
    void InputAttributeData() {
        attributes_.emplace_back(std::make_pair(attribute_name_,
                                attribute_value_));
    }
    const std::vector<std::pair<std::string, std::string>>& Attributes() const {
        return attributes_;
    }

private:
    TokenType type_;
    std::string data_;
    std::string attribute_name_;
    std::string attribute_value_;
    std::vector<std::pair<std::string, std::string>> attributes_;
};
