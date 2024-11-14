#include <iostream>

std::string get_by_id(const std::string& str, const unsigned int id)
{
    std::string result;
    bool single = false;
    unsigned int count = 0;
    unsigned int prev = 0;
    for(auto i = 0; i < str.size(); i++)
    {
        if(str[i] == '"')
        {
            single = !single;
            continue;
        }

        if(!single)
        {

            if(str[i] == ',')
            {
                result = str.substr(prev, i-prev);
                prev = i + 1;
                count++;
            }

        }
        if(count == id)
        {
            break;
        }
    }
    return result;
}

int main(int argc, char ** argv)
{
    std::string line;
    int count = 0;
    while (std::getline(std::cin, line))
    {
        count++;
        auto result = get_by_id(line, 10);
        std::cout << result << std::endl;
    }

    return 0;
}