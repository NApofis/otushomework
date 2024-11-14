#include <iostream>
#include <string>


double arithmetic(const std::string& line)
{
	static unsigned int counter = 0;
	static double sum = 0;
    counter++;
    sum += std::stoi(line);
	return sum / counter;
}

double variance(const std::string& line)
{
	static unsigned int counter = 0;
	static double sum = 0;

	counter++;
	const auto value = std::stoi(line);
	sum += value * value;
	const double var = sum / counter;

	const auto ar = arithmetic(line);
	return var - (ar * ar);
}


int main(const int argc, char* argv[])
{
	const bool check = (argc == 2 && std::string(argv[1]) == "--variance");

    double (*func)(const std::string& line);
    if (check)
    {
		func = &variance;
    }
	else
	{
    	func = &arithmetic;
	}

    std::string line;
	double result = 0;
    while (std::getline(std::cin, line))
    {
		result = func(line);
    }
    std::cout << result << std::endl;
    return 0;
}