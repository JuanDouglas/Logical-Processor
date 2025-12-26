#include "viewer.h"
#include "clock.h"
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

static void sleep_ms(int ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

void viewer_clear(void)
{
    printf("\033[2J\033[H"); /* ANSI clear */
}

void viewer_init(void)
{
    viewer_clear();
    printf("LOGICAL PROCESSOR :: REAL-TIME VIEW\n");
}

void viewer_render(Circuit *circuits, int count)
{
    viewer_clear();

    printf("Cycle: %d | CLK: %s\n",
           clock_cycle(),
           clock_rising_edge() ? "↑" : "↓");

    printf("──────────────────────────────────\n");

    for (int i = 0; i < count; i++)
    {
        circuit_dump(&circuits[i]);
    }

    printf("\n[s]tep [r]un [p]ause [q]uit\n");
}

char viewer_input(void)
{
    char c = 0;
    if (scanf(" %c", &c) == 1)
        return c;
    return 0;
}
