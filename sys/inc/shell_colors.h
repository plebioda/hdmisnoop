#ifndef _SHELL_COLORS_H
#define _SHELL_COLORS_H

#define COLOR_PREFIX		"\e["
#define COLOR_SEPARATOR		";"
#define COLOR_POSTFIX		"m"
#define COLOR_FOREGROUND	"3"
#define COLOR_BACKGROUND	"4"
#define COLOR_REGULAR		"0"
#define COLOR_BOLD		"1"
#define COLOR_UNDERLINE		"4"
#define COLOR_BLACK		"0"
#define COLOR_RED		"1"
#define COLOR_GREEN		"2"
#define COLOR_YELLOW		"3"
#define COLOR_BLUE		"4"
#define COLOR_PURPLE		"5"
#define COLOR_CYAN		"6"
#define COLOR_WHITE		"7"

#define COLORF(c, a)		COLOR_PREFIX COLOR_##a COLOR_SEPARATOR COLOR_FOREGROUND COLOR_##c COLOR_POSTFIX
#define COLORB(b)		COLOR_PREFIX COLOR_BACKGROUND COLOR_##b COLOR_POSTFIX
#define COLORFB(c, a, b)	COLORF(c, a) COLORB(b)
#define COLOR(c, a)		COLORF(c, a)
#define COLOR_CLEAR		"\e[0m"

#endif //_SHELL_COLORS_H
