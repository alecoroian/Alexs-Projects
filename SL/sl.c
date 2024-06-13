/*******************************************************************************
 * Name        : sl.c
 * Author      : Alex Coroian
 * Pledge      : I pledge my honour that I have abided by the Stevens Honour System
 ******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

void validate_directory(char *directory);
void create_pipes(int ls_to_sort_fds[2], int sort_to_parent_fds[2]);
void close_pipes(int fds[2]);
void read_and_print_sorted_output(int read_end);

void launch_ls_process(const char *path, int pipe_write_end);
void launch_sort_process(int pipe_read_end, int result_pipe_write_end);

int main(int argc, char *argv[]) // checks amount of argeuments
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <DIRECTORY>\n", argv[0]);
		return EXIT_FAILURE;
	}

	validate_directory(argv[1]);

	int ls_to_sort_fds[2], sort_to_parent_fds[2];
	create_pipes(ls_to_sort_fds, sort_to_parent_fds);

	if (fork() == 0)
	{
		close(ls_to_sort_fds[READ_END]);
		launch_ls_process(argv[1], ls_to_sort_fds[WRITE_END]);
	}

	if (fork() == 0)
	{
		close(ls_to_sort_fds[WRITE_END]);
		close(sort_to_parent_fds[READ_END]);
		launch_sort_process(ls_to_sort_fds[READ_END], sort_to_parent_fds[WRITE_END]);
	}

	close_pipes(ls_to_sort_fds);
	close(sort_to_parent_fds[WRITE_END]);

	read_and_print_sorted_output(sort_to_parent_fds[READ_END]);

	while (wait(NULL) > 0)
		;

	return EXIT_SUCCESS;
}

void validate_directory(char *directory)
{
	struct stat directory_info;
	if (stat(directory, &directory_info) != 0 || !S_ISDIR(directory_info.st_mode) || access(directory, R_OK) != 0)
	{
		fprintf(stderr, "Permission denied. nonexist cannot be read.\n");
		exit(EXIT_FAILURE);
	}
}

void create_pipes(int ls_to_sort_fds[2], int sort_to_parent_fds[2])
{
	pipe(ls_to_sort_fds);
	pipe(sort_to_parent_fds);
}

void close_pipes(int fds[2])
{
	close(fds[READ_END]);
	close(fds[WRITE_END]);
}

void read_and_print_sorted_output(int read_end)
{
	char output_buffer[1024];
	int count = 0, read_bytes;

	while ((read_bytes = read(read_end, output_buffer, sizeof(output_buffer) - 1)) > 0)
	{
		output_buffer[read_bytes] = '\0';
		printf("%s", output_buffer);
		for (int i = 0; i < read_bytes; ++i)
		{
			if (output_buffer[i] == '\n')
				count++;
		}
	}
	close(read_end);
	printf("Total files: %d\n", count);
}

void launch_ls_process(const char *path, int pipe_write_end)
{
	dup2(pipe_write_end, STDOUT_FILENO);
	close(pipe_write_end);
	execlp("ls", "ls", "-1ai", path, NULL);
	perror("ls execution failure");
	exit(EXIT_FAILURE);
}

void launch_sort_process(int pipe_read_end, int result_pipe_write_end)
{
	dup2(pipe_read_end, STDIN_FILENO);
	dup2(result_pipe_write_end, STDOUT_FILENO);
	close(pipe_read_end);
	close(result_pipe_write_end);
	execlp("sort", "sort", NULL);
	perror("sort execution failure");
	exit(EXIT_FAILURE);
}