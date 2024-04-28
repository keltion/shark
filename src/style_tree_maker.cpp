#include <algorithm>
#include <unordered_set>
#include <variant>
#include <vector>

#include "css_parser.h"
#include "dom.h"
#include "style_tree_maker.h"

bool StyleTreeMaker::matchSimpleSelector(const Element& element, const SimpleSelecter& selector) const
{
	if (selector.tagName() && selector.tagName() != element.GetTagName())
	{
		return false;
	}

	if (selector.id() && selector.id() != element.Id())
	{
		return false;
	} 

	for (const auto& className: selector.classes())
	{
		if (!element.classes().contains(className))
		{
			return false;
		}
	}

	return true;
}

std::vector<Rule> StyleTreeMaker::findRules(const Element &element, const Stylesheet& stylesheet) const
{
	std::vector<Rule> rules;
	constexpr int32_t defaultSize = 20;
	rules.reserve(defaultSize);

	for (const auto& rule : stylesheet.rules())
	{
		if (!matchSimpleSelector(element, rule.selector()))
		{
			continue;
		}
		rules.emplace_back(rule);
	}

	std::sort(rules.begin(), rules.end(), [](const auto& lhs, const auto& rhs) {
		return lhs.selector().specificity() < rhs.selector().specificity();
	});

	return rules;
}


std::unordered_map<std::string, Value> StyleTreeMaker::matchSelector(const DomNode& node, const Stylesheet &stylesheet) const
{
	std::unordered_map<std::string, Value> values;

	if (node.type() != ElementType::kElement)
	{
		return values;
	}

	std::vector<Rule> rules = findRules(*dynamic_cast<const Element*>(&node), stylesheet);
	for (const auto& rule : rules)
	{
		for (const auto& declaration : rule.declarations())
		{
			values[declaration.property_] = declaration.value_;
		}
	}

	return values;
}

StyleNode StyleTreeMaker::createStyleTree(const DomNode& node, const Stylesheet& stylesheet)
{
	StyleNode styleNode{ node };
	
	for (const auto& childNode : node.childern())
	{
		styleNode.addChild(std::move(createStyleTree(*childNode, stylesheet)));
	}

	styleNode.setSpecifiedValues(std::move(matchSelector(node, stylesheet)));
	return styleNode;
}

void StyleNode::addChild(StyleNode&& styleNode)
{
	children_.emplace_back(std::move(styleNode));
}

const std::vector<StyleNode>& StyleNode::children() const
{
	return children_;
}

void StyleNode::setSpecifiedValues(std::unordered_map<std::string, Value>&& specifiedValues)
{
	specifiedValues_ = std::move(specifiedValues);
}

std::optional<Value> StyleNode::value(std::string key) const
{
	if (!specifiedValues_.contains(key)) {
		return std::nullopt;
	}
	return specifiedValues_.at(key);
}

Display StyleNode::defaultDisplay() const {
	static std::unordered_set<std::string> blockTag = { "html", "div", "p", "form", "body"};
	static std::unordered_set<std::string> inlineTag = { "span", "a", "em" };

	if (domNode_.isTextNode() || inlineTag.contains(domNode_.GetElementData())) {
		return Display::kInline;
	}

	if (blockTag.contains(domNode_.GetElementData())) {
		return Display::kBlock;
	}

	return Display::kNone;
}

Display StyleNode::display() const {
	const auto displayTypeValue = value(CssProperties::kDisplay);

	if (!displayTypeValue) {
		return defaultDisplay();
	}

	const auto* displayTypePtr = std::get_if<Keyword>(&displayTypeValue.value());
	if (!displayTypePtr) {
		return Display::kNone;
	}

	if (displayTypePtr->value == CssPropertyValues::kDisplayBlock) {
		return Display::kBlock;
	}
	else if (displayTypePtr->value == CssPropertyValues::kDisplayInline) {
		return Display::kInline;
	}

	return Display::kNone;
}

Value StyleNode::lookup(std::string&& propertyName, std::string&& fallbackName, const Value& defaultValue) const
{
	auto propertyValue = value(propertyName);
	if (propertyValue) {
		return *propertyValue;
	}

	propertyValue = value(fallbackName);
	if (propertyValue) {
		return *propertyValue;
	}

	return defaultValue;
}

