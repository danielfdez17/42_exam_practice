#include "life.h"

int main(int ac, char **av)
{
	if (ac != 4)
		return 1;
	
	t_life life;
	if (!init_life(&life, av))
		return 2;
	read_info(&life);
	print_map(&life);
	play(&life);
	return 0;
}
