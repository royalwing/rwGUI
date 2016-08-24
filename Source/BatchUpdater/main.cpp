#include "rwupd.h"
#include <iostream>

int main(int argc, char* argv[])
{
	std::cout << "Arguments : " << std::endl;
	for (int i = 0; i < argc; i++)
	{
		std::cout << argv[i] << std::endl;
	}
	std::cout << "-----------------------" << std::endl;
	if (argc == 2)
	{
		Updater::UpdateApp(argv[1]);
		Updater::LaunchApp(argv[1]);
	}
	else {
		Updater::UpdateApp("GUI");
		Updater::LaunchApp("GUI");
	}
	return 0;
}