#include "clock.h"

static int cycle = 0;
static bool clk = 0;
static bool paused = false;

void clock_init(void)
{
    cycle = 0;
    clk = 0;
    paused = false;
}

void clock_tick(void)
{
    if (paused) return;

    clk = !clk;
    if (clk) cycle++;
}

bool clock_rising_edge(void)
{
    return clk;
}

int clock_cycle(void)
{
    return cycle;
}

void clock_pause(bool state)
{
    paused = state;
}
