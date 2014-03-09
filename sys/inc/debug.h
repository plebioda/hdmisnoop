#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>
#include <clock.h>

#ifndef DEBUG_CONFIG_TIMESTAMPS
#define DEBUG_CONFIG_TIMESTAMPS	1
#endif

#if DEBUG_CONFIG_TIMESTAMPS
#define dbg(fmt, args...)	{						\
					up_time_t time = get_up_time();		\
					printf("[%02d:%02d:%02d.%03d] debug: " fmt, time.hours, time.minutes, time.seconds, time.ms, ## args);		\
				}
#else
#define dbg(fmt, args...)	printf(fmt, ## args)
#endif

#define _dbg(fmt, args...)	printf(fmt, ## args)

#define TODO(fmt, args...)	dbg("** TODO at [%s:%d]\n", __FILE__, __LINE__);	\
				dbg("** " fmt, ## args)

#define STR(name)	#name
#define PRINT_FIELD(n, f)	dbg("%s.%s	: %d\n", STR(n), STR(f), n.b.f)

typedef volatile unsigned int io;

#define PRINT_REG_FIELD(_pre, _reg, _field)	{\
							io * ptr = (io *) &_pre##_reg->_field; \
							dbg("0x%08x : %-20s = 0x%08x\n", ptr, STR(_reg)"."STR(_field), *ptr); \
						}

#define PRINT_REG(_pre, _reg)			{\
							io * ptr = (io *) ( _pre##_reg ); \
							dbg("0x%08x : %-20s = 0x%08x\n", ptr, STR(_reg),  *ptr); \
						}

#define PRINT_SIZE(t)		dbg("sizeof(%s) = %d\n", STR( t ), sizeof( t ) )

#endif //_DEBUG_H
