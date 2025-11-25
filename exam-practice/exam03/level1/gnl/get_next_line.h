#ifndef GET_NEXT_LINE_H
#define GET_NEXT_LINE_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 42
#endif // BUFFER_SIZE

#ifndef MAX_FD
#define MAX_FD 1024
#endif // MAX_FD


char	*get_next_line(int fd);
int		ft_strlen(char *str);
char	*ft_find_endl(char *str);
char	*ft_substr(char *str, int start, int len);
char	*ft_strjoin(char *s1, char *s2); // ! free s1 & s2

#endif // GET_NEXT_LINE_H