#pragma once

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "dom.h"
#include "tree_traval.h"

class HTMLParser {
public:
    HTMLParser(std::string html) : input_(html), pos(0) {} 
    Node* parse_node();
    std::vector<Node*> parse_nodes();
    char next_char();
    Element* parse_element();
    TextNode* parse_text();
    char consume_char();
private:
    std::size_t pos;
    std::string input_;
};

Node* HTMLParser::parse_node() {
    Node* node = nullptr;
    if('<' == next_char()) {
        node = parse_element();
    }
    else {
        node = parse_text();
    }
    return node;
}

std::vector<Node*> HTMLParser::parse_nodes() {
    std::vector<Node*> chlidren;
    while(input_[pos+1] != '/') {
        chlidren.emplace_back(parse_node());
    }
    return chlidren;
}
char HTMLParser::next_char() {
    return input_[pos];
}

char HTMLParser::consume_char() {
    return input_[pos++];
}

TextNode* HTMLParser::parse_text() {
    std::string text;
    while(next_char() != '<')
        text += consume_char();
    TextNode* text_node = new TextNode(text);
    return text_node;
}

Element* HTMLParser::parse_element() {
    std::string tag_name;
    std::string confirm_tag_name;
    std::unordered_map<std::string, std::string> attr_map;
    std::string attr;
    std::string value;

    char temp;

    // opening tag.
    consume_char(); // '<'
    while(next_char() != ' ' && next_char() != '>') {
        tag_name += consume_char();
    }
    if(next_char() == ' ') {
        consume_char();
    }

        // attribute
    while(next_char() != '>') {
        attr = "";
        value = "";
        while(next_char() != '=') {
            attr += consume_char();
        }
        consume_char(); // =
        consume_char(); // "
        while(next_char() != '"') {
            value += consume_char();
        }
        consume_char(); // "
        if(next_char() == ' ') {
            consume_char();
        }
        attr_map.emplace(attr, value);
    }
    consume_char(); // '>'

    // contents - 중첩가능성
    // contents == text이더라도 children이 되니까

    std::vector<Node*> childern = parse_nodes();

    // closing tag. </ >
    consume_char(); // '<'
    consume_char(); // '/'
    while(next_char() != '>') {
        confirm_tag_name += consume_char();
    }
    consume_char(); // '>'
    if(tag_name != confirm_tag_name) {
        exit(0);
    }
    
    Element* element = new Element(tag_name, attr_map);
    element->childern = childern;
    return element;   
}