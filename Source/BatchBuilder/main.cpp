#include <rwupd.h>
#include <iostream>


int main(int argc, char *argv[])
{
	if (argc == 3)
	{
		Updater::SetCurrentFolder(argv[2]);
	}
	if (argc > 1)
	{
		Updater::BuildAppManifest(argv[1]);
		Updater::ReleaseApp(argv[1]);
	} else {
		return 1;
	}
    return 0;
}