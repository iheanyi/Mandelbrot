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
#include <math.h>
#include <ctype.h>


//  ffmpeg -f image2 -pattern_type sequence -start_number 1 -i mandel%01d.bmp test.mpg


int counter = 0;
float scale = 2.0000000;
//float step 
//float scale = 2.0000000;
//float step = 0.01999995;
int forkCounter = 0;
int main(int argc, char* argv[]) {
	//time_t begin = time(NULL);

	if(argc != 2) {
		printf("usage: mandelmovie <number of forks>\n");
		exit(1);
	}

	/*if(isdigit(argv[1])) {
		printf("Please enter a number as the argument.\n");
		exit(1);
	}*/


	int forkCount = atoi(argv[1]);

	pid_t pid; // Parent process id
	int status;


	counter = 0;

	float step = expf(logf(0.000005/scale)/50);


	//char* command[100];



	// Array of strings
	char* commands[100];

	//char* commands[] = {"./mandel", "-x 0.135", "-y 0.60", "-W 900", "-H 900", "-m 1000", "-s 0.000005", "CHANGINGCHANGINGCHANGINGCHANGING", ""};


	// Command is ./mandel -x 0.135, -y 0.60 -W 900 -H 900 -m 1000 -s 0.000005 
	time_t end;
	double time_spent;


	// Making the initial commands array for execvp
	commands[0] = "./mandel";
	commands[1] = "-x 0.135";
	commands[2] = "-y 0.60";
	commands[3] = "-W 1024";
	commands[4] = "-H 1024";
	commands[5] = "-m 1000";
	commands[6] = malloc(100 * sizeof(char));	// Malloc space for the scale
	commands[7] = "-o";
	commands[8] = malloc(100 * sizeof(char));	// Malloc space for the filename
	commands[9] = NULL;


	// Was segfaulting earlier, but I fixed it.
	//printf("Segging here?\n");
	sprintf(commands[6], "-s %f", scale);		// Update the scale with the original value
	sprintf(commands[8], "mandel%d.bmp", counter);	// Update the filename with counter value appended

	//printf("Before new Forking business?\n");

	// Looping for executing

	while(counter < 50) {

		// If the fork counter is less than defined amount, fork new process 
		if(forkCounter < forkCount) {
			// Before forking, we can increment/decrement variables here because if there is an error in forking,
			// we are going to exit the program anyways, is this good practice?

			// Note: In Fork, the child process is a copy of the parent's memory, therefore global variables 
			// cannot be modified/changed after fork is called. Food for the thought.

			forkCounter++;
			counter++;
			scale = scale*step;
			sprintf(commands[6], "-s %f", scale);
			sprintf(commands[8], "mandel%d.bmp", counter);
			//printf("Fork Count: %d\n", forkCounter);

			pid = fork();

			if(pid >= 0) {
				if(pid == 0) {
					//printf("mandelmovie2: process %d started\n", getpid());

					int execResult = execvp(commands[0], commands);
					if(execResult == -1) {
						printf("mandel: command error: %s\n", strerror(errno));
						exit(EXIT_FAILURE);
					}

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
				forkCounter--;

				// Decrement number of forks . . . 
				if(status == 0) {
					//printf("mandelmovie: process %d exited normally with status %d\n", result, WEXITSTATUS(status));
				}

				else {
					//printf("mandelmovie: process %d exited abnormally with status %d\n", result, status);
				}
			}
		}


	}

	end = time(NULL);
	//time_spent = end - begin;

	//printf("Time spent executing MandelMovie: %lf seconds. \n", time_spent);
	return 0;
}