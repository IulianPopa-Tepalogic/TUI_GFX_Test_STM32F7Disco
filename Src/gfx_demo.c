#include "tos_proc.h"

TOS_PROCESS(test_fpu_ops, 10)
{
	int i = 0;
	while (true)
		++i;
}
