#include "tos_procs.h"

TOS_PROCESS(gfx_demo, 10)
{
	int i = 0;
	while (true)
		++i;
}
