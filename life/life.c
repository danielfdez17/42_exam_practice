#include "life.h"

int main(int ac, char **av)
{
	if (ac != 4)
		return 1;
	t_life	life;
	if (!init_life(&life, av))
		return printf("init_life"), 1;
	if (!read_input(&life))
		return printf("read_input"), 1;
	play(&life);
	draw_map(&life);
	free_map(&life);
	return 0;
}
