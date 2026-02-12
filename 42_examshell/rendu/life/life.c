#include "life.h"

int main(int ac, char **av)
{
	if (ac != 4)
		return 1;
	// read_info
	t_life life;
	if (!read_info(&life, av))
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
