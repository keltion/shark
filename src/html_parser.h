#pragma once

#include <memory>
#include <string>

#include "dom.h"
#include "html_token.h"
#include "html_tokenizer.h"
#include "dom_tree_builder.h"

class HTMLParser 
{
public:
    HTMLParser(std::string&& html)
        : pos_(0), tokenizer_(HTMLTokenizer(std::move(html))) {}
    bool PumpTokenizer();
    const DomNode& DomTree() const;

private:
    std::size_t pos_;
    HTMLTokenizer tokenizer_;
    DomTreeBuilder treeBuilder_;
};