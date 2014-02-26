#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>
#include <clock.h>

#if 0
#define dbg(fmt, args...)	{						\
					up_time_t time = get_up_time();		\
					printf("[%02d:%02d:%02d.%03d] debug: " fmt, time.hours, time.minutes, time.seconds, time.ms, ## args);		\
				}
#else
#define dbg(fmt, args...)	printf(fmt, ## args)
#endif
#define STR(name)	#name
#define PRINT_FIELD(n, f)	dbg("%s.%s	: %d\n", STR(n), STR(f), n.b.f)


#endif //_DEBUG_H
