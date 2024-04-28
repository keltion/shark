#pragma once

#include "css_parser.h"
#include "dom.h"

enum class Display {
	kInline,
	kBlock,
	kNone,
};

class StyleNode
{
public:
	StyleNode() = delete;
	explicit StyleNode(const DomNode &domNode) : domNode_(domNode) {}

	void addChild(StyleNode &&styleNode);
	const std::vector<StyleNode>& children() const;
	void setSpecifiedValues(std::unordered_map<std::string, Value>&& specifiedValues);
	std::optional<Value> value(std::string key) const;
	Display defaultDisplay() const;
	Display display() const;
	Value lookup(std::string&& propertyName, std::string&& fallbackName, const Value& defaultValue) const;

private:
	const DomNode& domNode_;
	std::unordered_map<std::string, Value> specifiedValues_;
	std::vector<StyleNode> children_;
};

class StyleTreeMaker
{
public:
	bool matchSimpleSelector(const Element& element, const SimpleSelecter& selector) const;
	std::vector<Rule> findRules(const Element& element, const Stylesheet& stylesheet) const;
	std::unordered_map<std::string, Value> matchSelector(const DomNode& node, const Stylesheet& stylesheet) const;
	StyleNode createStyleTree(const DomNode &node, const Stylesheet& stylesheet);
private:
};