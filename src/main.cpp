#include "main.h"
#include "config.h"

using namespace std;

int main()
{
	map<ConfigEnum, string> config_map = parseConfig();
	return 0;
}