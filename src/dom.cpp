#include <sstream>
#include <string>
#include <unordered_set>

#include "dom.h"

void DomNode::addChild(std::unique_ptr<DomNode> node)
{
	childern_.emplace_back(std::move(node));
}

void DomNode::setParent(DomNode* node)
{
	parent_ = node;
}

DomNode * DomNode::parent() const
{
	return parent_;
}

const std::vector<std::unique_ptr<DomNode>>& DomNode::childern() const
{
	return childern_;
}

ElementType DomNode::type() const
{
	return type_;
}

bool DomNode::isTextNode() const
{
	return false;
}

const std::string& Element::Id() const
{
	static std::string emptyString = "";
	if (!AttrMap_.contains("id")) // TODO: change enum class 
	{
		return emptyString;
	}
	
	return AttrMap_.at("id");
}

const std::unordered_set<std::string> Element::classes() const
{
	std::unordered_set<std::string> emptySet = {};
	if (!AttrMap_.contains("class")) 
	{
		return emptySet;
	}

	std::istringstream iss(AttrMap_.at("class"));

	std::unordered_set<std::string> classes;
	std::string token;
	while (iss >> token) 
	{
		classes.insert(token);
	}

	return classes;
}

void Element::setAttributeMap(std::unordered_map<std::string, std::string>&& AttributeMap)
{
	AttrMap_ = std::move(AttributeMap);
}

