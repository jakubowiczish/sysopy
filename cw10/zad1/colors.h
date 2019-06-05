#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_BRIGHT_RED "\x1b[91m"
#define ANSI_COLOR_BRIGHT_GREEN "\x1b[92m"
#define ANSI_COLOR_BRIGHT_YELLOW "\x1b[93m"
#define ANSI_COLOR_BRIGHT_BLUE "\x1b[94m"
#define ANSI_COLOR_BRIGHT_MAGENTA "\x1b[95m"
#define ANSI_COLOR_BRIGHT_CYAN "\x1b[96m"
#define ANSI_COLOR_RESET "\x1b[0m"

void set_color(char* color);
void reset_color();
void colorprintf(char* color, char* format, ...);