#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	// early exit if no arguments
	if (argc == 1) {
		errno = EINVAL;
		fprintf(stderr, "usage: ./pipe CMD...\n");
		exit(errno);
	};


	// loop through arguments
	for (int i = 1; i < argc; i++) {
		// current arg
		char* prog = argv[i];

		// note: create pipe *before* forking
		int pipefd[2];
		pipe(pipefd);

		int pid = fork();
		if (pid < 0) {
			fprintf(stderr, "fatal: fork failed\n");
			exit(errno);
		}
		// in child
		else if (pid == 0) {
			// connect child's stdout to pipe's write-end unless it's the last program
			if (i != argc-1) {
				dup2(pipefd[1], 1);
			}

			// close pipe
			// note: write-end is still open because stdout points to the same open file table entry
			close(pipefd[0]);
			close(pipefd[1]);

			// replace this current program with the program specified on the command line
			execlp(prog, prog);

			// return errno (parent will receive it in its part of its status var)
			// note: this only runs if execlp failed
			return errno;
		}
		// in parent
		else {
			// connect parent's stdin to pipe's read-end
			//   - the next forked process will inherit this
			dup2(pipefd[0], 0);

			// close pipe
			// note: read-end is still open because stdin points to the same open file table entry
			close(pipefd[0]);
			close(pipefd[1]);

			// suspend execution until child process finishes
			int status;
			waitpid(pid, &status, 0);

			// if child failed, immediately terminate
			// note:  to get the actual return value from the child, we need the WEXITSTATUS macro b/c status is encoded with other data
			//   - the return value (exit code) from the child is the second lowest byte of status
			//   - the least significant byte holds information about the reason the child exited, not the exit code
			// aside: exiting with anything >255 wraps around
			//   - e.g. if you call exit(256), the return code will actually be 0 instead of 256
			int exit_code = WEXITSTATUS(status);
			if (WIFEXITED(status) && exit_code) {
				fprintf(stderr, "pipe: could not execute %s\n", prog);
				exit(exit_code);
			}
		}
	}
	return 0;
}
