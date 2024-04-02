#include <string.h>

#include "boyer-moore.h"

bool majorant(const char *str, char *res)
{
	size_t candi = 0;
	const size_t len = strlen(str);
	for (size_t i = 0, balance = 0; i < len; ++i)
	{
		if (!balance)
			candi = i;
		if (str[candi] == str[i])
			++balance;
		else
			--balance;
	}

	size_t count = 0;
	for (size_t i = 0; i < len; ++i)
		if (str[i] == str[candi])
			++count;

	if (count > len/2)
	{
		*res = str[candi];
		return true;
	}
	return false;
}
