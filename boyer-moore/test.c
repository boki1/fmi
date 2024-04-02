#include <stdio.h>
#include "boyer-moore.h"

int main(int argc, char* argv[])
{
	char cand;
	for (int i = 1; i < argc; ++i)
		printf("%d: %c\n", i, majorant(argv[i], &cand) ? cand : '-');
	return 0;
}
