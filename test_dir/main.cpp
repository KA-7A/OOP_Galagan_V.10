#include <string>
#include <iostream>
int main()
{
	std::string msg = "Message";
	int i = 10;
	msg += std::to_string(i);
	std::cout << msg << std::endl;
	return 0;
}
