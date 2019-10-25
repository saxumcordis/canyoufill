/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nthrynn <nthrynn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 17:24:40 by nthrynn           #+#    #+#             */
/*   Updated: 2019/10/25 17:26:08 by nthrynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fillit.h"

void	error_message(char *str)
{
	(void)write(1, str, ft_strlen(str));
	write(1, "\n", 1);
	exit(0);
}

/*
** Allocates enough memory for each line of the final print grid
*/

char	**grid_initialize(unsigned int size)
{
	char			**grid;
	unsigned int	i;

	i = 0;
	grid = (char**)malloc(sizeof(char*) * size);
	while (i < size)
	{
		grid[i] = (char*)malloc(size + 1);
		ft_memset(grid[i++], '.', size);
	}
	return (grid);
}

/*
** Read a set of 21 characters from the input file. Each valid pieces consists of 21 characters, including new lines.
** Then the validity of the piece is checked and it is either converted to a struct representation or an error is thrown.
*/

int		read_blocks(int fd, t_tetrim **ar)
{
	char	buf[21];
	int		val;
	int		i;

	i = 0;
	ft_bzero(buf, sizeof(buf));
	while (((val = read(fd, buf, 21)) >= 20) && i < 26)
	{
		if (val == 20)
			buf[20] = '\0';
		if (!(check_count(buf) && check_format(buf) && check_connections(buf)))
			return (0);
		ar[i] = structify(buf);
		ar[i]->id = 'A' + i;
		++i;
	}
	if (val != 0 || buf[20] != '\0' || !ar[0])
		return (0);
	return (1);
}

/*
** This function is responsible for printing the final representation of the solved map.
*/

void	printer(char **grid, unsigned int size)
{
	unsigned int i;

	i = 0;
	while (i < size)
	{
		write(1, grid[i], size);
		write(1, "\n", 1);
		++i;
	}
}

/*
** This is the entry point for the program
*/

int		main(int argc, char **argv)
{
	t_tetrim	*ar[27];
	t_16		map[16];
	int			fd;

	ft_bzero(map, sizeof(map));
	ft_bzero(ar, sizeof(ar));
	if (argc != 2)
		error_message("usage: ./fillit source_file");
	if (!(fd = open(argv[1], O_RDONLY)))
		error_message("error");
	if (!(read_blocks(fd, ar)))
		error_message("error");
	solver(ar, map);
	return (0);
}
