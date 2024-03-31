#include <fstream>
#include <sstream>
#include <string>

#include "html_parser.h"

constexpr const char* kTestHtmlFileName = "src/test.html";

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


int main() 
{
    HTMLParser htmlParser = parseHtmlFile(kTestHtmlFileName);


    return 0;
}