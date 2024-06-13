#define BLUE "\x1b[34;1m"
#define DEFAULT "\x1b[0m"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/wait.h>
#include <errno.h>
#define MAXLINE 1024
#define MAXARGS 128

volatile int interrupted = 0;

// strings for error handling
const char *errmsg_pwd = "Error: Cannot get current working directory. %s\n";
const char *errmsg_cd = "Error: Cannot change directory to %s. %s\n";
const char *errmsg_malloc = "Error: malloc() failed. %s\n";
const char *errmsg_realloc = "Error: realloc() failed. %s\n";
const char *errmsg_opendir = "Error: Cannot open current working directory. %s\n";
const char *errmsg_proc = "Error: Cannot open /proc. %s\n";
const char *errmsg_fork = "Error: fork() failed. %s\n";
const char *errmsg_exec = "Error: exec() failed. %s\n";
const char *errmsg_wait = "Error: wait() failed. %s\n";
const char *errmsg_signal = "Error: Cannot register signal handler. %s\n";
const char *errmsg_stdin = "Error: Failed to read from stdin. %s\n";

void handle_signal(int sig)
{
	interrupted = 1;
}

char **parse_cmd(char *cmdline)
{
	char **args = malloc(sizeof(char *));
	if (args == NULL)
	{
		fprintf(stderr, errmsg_malloc, strerror(errno));
		exit(EXIT_FAILURE);
	}

	int i = 0;
	char *arg = strtok(cmdline, " \n");

	while (arg != NULL)
	{
		args[i++] = arg;
		char **temp = realloc(args, (i + 1) * sizeof(char *));
		if (temp == NULL)
		{
			fprintf(stderr, errmsg_realloc, strerror(errno));
			free(args);
			exit(EXIT_FAILURE);
		}
		args = temp;
		arg = strtok(NULL, " \n");
	}

	args[i] = NULL;

	return args;
}

void pwd() // pwd
{
	char cwd[MAXLINE];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		fprintf(stderr, errmsg_pwd, strerror(errno));
	}
	else
	{
		printf("%s\n", cwd);
	}
}

void lf() // lf
{
	DIR *dir;
	struct dirent *entry;
	if ((dir = opendir(".")) == NULL)
	{
		fprintf(stderr, errmsg_opendir, strerror(errno));
	}
	else
	{
		while ((entry = readdir(dir)) != NULL)
		{
			if (entry->d_name[0] != '.')
			{
				printf("%s\n", entry->d_name);
			}
		}
		closedir(dir);
	}
}

int compare_ids(const void *x, const void *y) // compares ids
{
	pid_t id1 = *(const pid_t *)x;
	pid_t id2 = *(const pid_t *)y;
	return (id1 > id2) - (id1 < id2);
}

void lp() // lp
{
	const char *proc_dir = "/proc";
	DIR *dir = opendir(proc_dir);
	struct dirent *entry;
	pid_t pids[1024];
	int pid_count = 0;

	if (dir == NULL)
	{
		fprintf(stderr, errmsg_proc, strerror(errno));
		return;
	}

	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_type == DT_DIR)
		{
			pid_t id = atoi(entry->d_name);
			if (id > 0)
			{
				pids[pid_count++] = id;
			}
		}
	}
	closedir(dir);

	qsort(pids, pid_count, sizeof(pid_t), compare_ids);

	for (int i = 0; i < pid_count; i++)
	{
		printf("%d\n", pids[i]);
	}
}

int main(int argc, char const *argv[]) // main function
{
	char commandLine[MAXLINE];
	struct sigaction sigAction;
	memset(&sigAction, 0, sizeof(sigAction));
	sigAction.sa_handler = handle_signal;

	if (sigaction(SIGINT, &sigAction, NULL) < 0) // signal handling
	{
		fprintf(stderr, errmsg_signal, strerror(errno));
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		if (interrupted)
		{
			printf("\n");
			interrupted = 0;
			continue;
		}

		// fetches and displays current directory
		char *currentDir = getcwd(NULL, 0);

		if (!currentDir)
		{
			fprintf(stderr, errmsg_pwd, strerror(errno));
			continue;
		}

		else
		{
			printf("%s[%s]%s> ", BLUE, currentDir, DEFAULT);
			free(currentDir);
		}

		if (!fgets(commandLine, MAXLINE, stdin)) // reads stdin
		{
			printf("\n");
			if (!interrupted)
			{
				fprintf(stderr, errmsg_stdin, strerror(errno));
			}
			continue;
		}

		char **arguments = parse_cmd(commandLine); // parse's through the command line
		if (arguments[0] == NULL)
		{
			free(arguments);
			continue;
		}

		if (!strcmp(arguments[0], "exit"))
		{
			free(arguments);
			break;
		}
		else if (!strcmp(arguments[0], "cd"))
		{
			char *dir = (arguments[1] == NULL || !strcmp(arguments[1], "~")) ? getpwuid(getuid())->pw_dir : arguments[1];
			if (chdir(dir) != 0)
			{
				fprintf(stderr, errmsg_cd, dir, strerror(errno));
			}
		}
		else if (!strcmp(arguments[0], "pwd"))
		{
			pwd();
		}
		else if (!strcmp(arguments[0], "lf"))
		{
			lf();
		}
		else if (!strcmp(arguments[0], "lp"))
		{
			lp();
		}
		else
		{
			pid_t pid = fork();
			if (pid == 0)
			{
				char execPath[MAXLINE] = "/bin/";
				strcat(execPath, arguments[0]);
				if (execv(execPath, arguments) == -1)
				{
					fprintf(stderr, errmsg_exec, strerror(errno));
					exit(EXIT_FAILURE);
				}
			}
			else if (pid < 0) // checking fork
			{
				fprintf(stderr, errmsg_fork, strerror(errno));
			}
			else
			{
				int status;
				if (wait(&status) == -1 && !interrupted)
				{
					fprintf(stderr, errmsg_wait, strerror(errno));
				}
			}
		}

		free(arguments);
	}

	return 0;
}
