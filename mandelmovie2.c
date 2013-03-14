#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>


int counter = 0;
float scale = 2.00000;
int main(int argc, char* argv[]) {

	int forkCount = atoi(argv[1]);

	printf("Fork Count: %d\n", forkCount);


	pid_t* processes = malloc(forkCount * sizeof(pid_t));
	pid_t pid; // Parent process id
	int status;

	float step = 0.0399999;

	counter = 0;



	//char* command[100];

	char* command[] = {"./mandel", "-x 0.135", "-y 0.60", "-W 900", "-H 900", "-m 1000", "-s 0.000005", "", ""};


	char* commands[100];

	//char* commands[] = {"./mandel", "-x 0.135", "-y 0.60", "-W 900", "-H 900", "-m 1000", "-s 0.000005", "CHANGINGCHANGINGCHANGINGCHANGING", ""};



	commands[0] = "./mandel";
	commands[1] = "-x 0.135";
	commands[2] = "-y 0.60";
	commands[3] = "-W 900";
	commands[4] = "-H 900";
	commands[5] = "-m 1000";
	commands[6] = "-s 0.000005";
	commands[7] = malloc(100 * sizeof(char));
	//command[0] = "./mandel";

	//commands[7] = "-o mandeltest.bmp";

	printf("Segging here?\n");
	sprintf(commands[7], "-o mandel%d.bmp", counter);


	printf("No? Maybe at the forking shit then?\n");
	pid = fork();

	if(pid >= 0) {
		if(pid == 0) {
			printf("FORK SUCCESSFUL! YEAH!\n");
			execvp(commands[0], commands);
			counter++;
		}

		else {
			pid_t result = wait(&status);

			if(result == -1) { 
				printf("mandelmovie: wait error %s\n", strerror(errno));
				exit(1);
			}

			else {
				if(status == 0) {
					printf("mandelmovie: process exited normally with status %d\n", WEXITSTATUS(status));
				}

				else {
					printf("mandelmovie: process exited abnormally with status %d\n", status);
				}
			}
		}
	}

	else {
		perror("Forking failed");
		exit(1);
	}

	int i = 0;
	int forkCounter = 0;
	int imgCounter = 0;

	printf("Before new Forking business?\n");

	while(counter < 50) {
		// If the fork counter is less than defined amount, fork new process 
		if(forkCounter < forkCount) {
			pid = fork();

			if(pid >= 0) {
				if(pid == 0) {
					//counter++;
					//scale -= step;
					printf("mandelmovie2: process %d started\n", getpid());
					sprintf(commands[6], "-s %f", scale);
					sprintf(commands[7], "-o mandel%d.bmp", counter);
					execvp(commands[0], commands);
					counter++;
					scale -= step;
				}
			}

			else {
				perror("Forking error!");
				exit(1);
			}
		}

		// Else, wait for some junts to exit
		else {
			pid_t result = wait(&status);

			if(result == -1) {
				printf("mandelmovie: wait error %s\n", strerror(errno));
				exit(1);			
			}

			else {
				if(status == 0) {
					printf("mandelmovie: process %d exited normally with status %d\n", result, WEXITSTATUS(status));
				}

				else {
					printf("mandelmovie: process %d exited abnormally with status %d\n", result, status);
				}
			}
		}


	}
	/*for(i = 0; i < forkCount; ++i) {
		pid = fork();

		if(pid >= 0) {
			if(pid == 0) {
				counter++;
				scale -= step;
				sprintf(commands[7], "-o mandel%d.bmp", counter);
				sprintf(commands[6], "-s %f", scale);
				execvp(commands[0], commands);
			}

			else {
				processes[i] = pid;
			}
		}
	}*/

	return 0;
}