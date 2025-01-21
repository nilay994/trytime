#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

#include <iostream>

std::vector<std::string> boost_fn(const std::string& topic)
{
    std::vector<std::string> result;

    using TokenizerWithCharSep = boost::tokenizer<boost::char_separator<char>>;

    boost::char_separator<char> separator("/");
    TokenizerWithCharSep        tokenizer(topic, separator);

    for (auto const token : tokenizer)
    {
        result.emplace_back(token);
    }

    return result;
}

std::vector<std::string> std_fn(const std::string& topic)
{
    std::vector<std::string> result;

    std::istringstream iss(topic);

    std::string token;
    while (std::getline(iss, token, '/'))
    {
        result.emplace_back(token);
    }

    return result;
}

int main()
{
    std::cout << "now using boost" << std::endl;
    std::string              test_string = "here//is/a/topic//";
    std::vector<std::string> result      = boost_fn(test_string);
    for (auto ele : result)
    {
        std::cout << ele << "\n";
    }
    std::cout << "\nnow using std" << std::endl;
    std::vector<std::string> result2 = std_fn(test_string);
    for (auto ele : result2)
    {
        std::cout << ele << "\n";
    }

    std::cin.get();
    return 0;
}