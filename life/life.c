#include "life.h"

int main(int ac, char **av)
{
	if (ac != 4)
		return 1;
	t_life	life;
	init_life(&life, av);
	play(&life);
	draw_map(&life);
	return 0;
}
