#include <fstream>
#include <sstream>
#include <string>

#include "css_parser.h"
#include "html_parser.h"
#include "layout_builder.h"
#include "style_tree_maker.h"
#include "utils.h"

constexpr const char* kTestHtmlFileName = "src/test.html";
constexpr const char* kTestCssFileName = "src/test.css";



std::string readFileIntoString(const std::string& path) 
{
    std::ifstream fin(path);
    std::stringstream buffer;
    buffer << fin.rdbuf();
    fin.close();
    return buffer.str();
}

HTMLParser parseHtmlFile(const std::string& filePath) 
{
    std::string html = readFileIntoString(filePath);
    HTMLParser htmlParser(std::move(html));

    htmlParser.PumpTokenizer();
    return htmlParser;
}

CSSParser parseCssFile(const std::string& filePath) 
{
    std::string css = readFileIntoString(filePath);
    CSSParser cssParser(std::move(css));
    cssParser.parseCss();
    return cssParser;
}

int main() 
{
    HTMLParser htmlParser = parseHtmlFile(kTestHtmlFileName);
    CSSParser cssParser = parseCssFile(kTestCssFileName);

    StyleTreeMaker styleTreeMaker;
    StyleNode styleRootNode = styleTreeMaker.createStyleTree(htmlParser.DomTree(), cssParser.stylesheet());

    LayoutBox * layoutRootBox = buildLayoutTree(&styleRootNode);


    Dimensions containingBlock;

    containingBlock.content = { 0, 0, 1823, 1186 };
    layoutRootBox->layout(containingBlock);

    return 0;
}