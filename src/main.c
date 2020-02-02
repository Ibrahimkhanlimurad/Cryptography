#include "opt.h"
#include "helper.h"
 
int main(int argc, char *argv[])
{
	srand(time(NULL));
 	Options* options = getOptions(argc, argv);
 	if (options->method) encrypt(options);
 	else decrypt(options);

	return 0;
}