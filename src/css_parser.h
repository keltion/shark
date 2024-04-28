#pragma once

#include <optional>
#include <string>
#include <tuple>
#include <unordered_map>
#include <variant>
#include <vector>


struct Keyword 
{
	std::string value;
};

enum class Unit
{
	Px,
};

struct Length 
{
	Length(float inputValue, Unit inputUnit, bool autoValue = false) : value(inputValue, inputUnit), autoValue(autoValue) {}
	std::pair<float, Unit> value;
	bool autoValue;
};

struct Color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

};


using Value = std::variant<Keyword, Length, Color>;

enum class kValueType
{
	Keyword,
	Length,
	Color,
	Display,
};

enum class kProperty
{
	FontWeight,
	Color,
	Display,
	Width,
	Margin,
	MarginLeft,
	MarginRight,
	BorderWidth,
	BorderLeftWidth,
	BorderRightWidth,
	Padding,
	PaddingLeft,
	PaddingRight,
};

struct Declaration
{
	std::string property_;
	Value value_;
};

class SimpleSelecter
{
public:
	std::tuple<int32_t, int32_t, int32_t> specificity() const
	{
		const int32_t idCount = id_ ? 1 : 0;
		const int32_t classCount = classes_.size();
		const int32_t tagCount = tagName_ ? 1 : 0;

		return std::make_tuple(idCount, classCount, tagCount);
	}
	
	void pushClass(std::string className);
	void setId(std::string id);
	void setTagName(std::string tagName);
	const std::optional<std::string>& tagName() const;
	const std::optional<std::string>& id() const;
	const std::vector<std::string>& classes() const;

private:
	std::optional<std::string> tagName_;
	std::optional<std::string> id_;
	std::vector<std::string> classes_;
};

class Rule
{
public:
	void pushDelaration(Declaration declaration);
	void setSelector(SimpleSelecter selector);
	const SimpleSelecter& selector() const;
	const std::vector<Declaration> &declarations() const;
private:
	SimpleSelecter selector_;
	std::vector<Declaration> declarations_;

};

class Stylesheet
{
public:
	void emplaceRule(Rule rule);
	void sortRule();
	Rule &back();
	const std::vector<Rule>& rules() const;
private:
	std::vector<Rule> rules_;
};

class CSSParser
{
public:
	enum class State {
		kSelectorNameState,
		kDeclarationProperty,
		kDeclarationValue,

	};

	CSSParser(std::string &&input) 
		:position_{ 0 }, input_{ std::move(input) }, state_{ State::kSelectorNameState } {}
	//void sortSelector();
	void parseCss();
	char nextCharacter();
	char peekCharacter() const;
	void consumeCharacter();
	SimpleSelecter parseSelectors();
	Value parseValue(const std::string& property);
	std::string parseProperty();
	void pumpRule();
	const Stylesheet& stylesheet();
	std::string inputForTest()
	{
		return input_;
	}

private:
	int32_t position_;
	std::string input_;
	State state_;
	Stylesheet stylesheet_;
};


