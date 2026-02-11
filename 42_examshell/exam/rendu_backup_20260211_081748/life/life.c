#include "life.h"

int main(int ac, char **av)
{
	if (ac != 4)
		return 1;

	t_life life;
	// init_life
	if (!init_life(&life, av))
		return 2;
	// read_input
	read_input(&life);
	// play
	play(&life);
	// print_map
	print_map(&life);
	// free_life
	free_life(&life);
	return 0;
}
