#pragma once

#include "css_parser.h"
#include "dom.h"

class StyleNode
{
public:
	void addChild(StyleNode &&styleNode);
	void setSpecifiedValues(std::unordered_map<std::string, Value>&& specifiedValues);
private:
	std::unique_ptr<DomNode> domNode_;
	std::unordered_map<std::string, Value> specifiedValues_;
	std::vector<StyleNode> children_;
};

class StyleTreeMaker
{
public:
	bool matchSimpleSelector(const Element& element, const SimpleSelecter& selector) const;
	std::vector<Rule> findRules(const Element& element, const Stylesheet& stylesheet) const;
	std::unordered_map<std::string, Value> matchSelector(DomNode* node, const Stylesheet& stylesheet) const;
	StyleNode createStyleTree(DomNode *node, const Stylesheet& stylesheet);
private:
};