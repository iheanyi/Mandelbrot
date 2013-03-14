/* Iheanyi Ekechukwu and Justin Bartlett

	Project 3: Mandelbrot Sets, awww yiss */


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


int numPics = 0;
int forkCounter = 0;
//int counter = 0;

float scale;
float step;



typedef struct {

	int iteration;
} mandel;

int main(int argc, char* argv[]) {

	int numProcesses = atoi(argv[1]);

	//pthread_t* threads = malloc(numProcesses * sizeof(pthread_t));

	pid_t* processes = malloc(numProcesses * sizeof(pid_t));
	int* statuses = malloc(numProcesses * sizeof(int));
	pid_t p_id; // Parent ID
    int status;

    scale = 2.0000;
    step = 0.0399999;

	// ./mandel -x 0.135 -y 0.60 -W 900 -H 900 -s 0.000005 -m 1000

    char* command[] = {"./mandel", "-x 0.135", "-y 0.60", "-W 900", "-H 900", "-m 1000", "-s 0.000005", ""};

    char* words[100];

    words[0] = "mandel";
    words[1] = "-x 0.135";
    words[2] = "-y 0.60";
    words[3] = "-W 900";
    words[4] = "-H 900";
    words[5] = "-m 1000";
    words[6] = "-s 0.000005";
    words[7] = "-o mandeltest.bmp";

    /*     words[0] = "mandel";
    words[1] = "-x";
    words[2] =  "0.135";
    words[3] = "-y";
    words[4] = "0.60";
    words[5] = "-W";
    words[6] = "900";
    words[7] = "H";
    words[8] = "900";
    words[9] = "-m";
    words[10] = "1000";
    words[11] = "-s";
    words[12] = "2.0"; */

	int i;
	int counter = 0;

	// Initially fork the process multiple times, as designated from the command line
	for(i = 0; i < numProcesses; ++i) {
		p_id = fork();

		printf("Initially forking\n");
		//printf("Counting forks %d", counter);


		if(p_id >= 0) {

			if(p_id == 0) {
				// Execvp junts here
				printf("mandelmovie: process %d started with %d\n", getpid(), counter);
				printf("Current Scale: %f\n", scale);
		 		sprintf(words[6], "-s %f", scale);
				sprintf(words[7], "mandel%d.bmp", counter);

				//printf("Comman")
				execvp(words[0], words);
				counter++;




			}

			else {

				printf("%d\n", p_id);
				counter++;
				processes[i] = p_id;
			}

		}

		else {
			perror("forking failed"); // Throw error
            exit(1);
		}


		 }

		 int waiting;

		 do {

		 	printf("In Loop, Counter = %d\n", counter);
		 	waiting = 0;
		 	
		 	for(i = 0; i < numProcesses; ++i) {
		 		if(processes[i] > 0) {
		 			// Should fork a new process if there aren't enough processes done

		 			printf("Checking Processes\n");

		 			pid_t exited = waitpid(processes[i], &status, 0);

		 			printf("%d", exited);
		 			if(exited == -1) {
		 				printf("mandelmovie: wait error: %s", strerror(errno));
		 			}

		 			//else {
	   		 		if(exited >= 0 && counter < 50) {
		 				// Process is done, so fork a new one?

			 				printf("Forking new process\n");
			 				p_id = fork();

							if(p_id >= 0) {
								if(p_id == 0) {
									// Execvp junts here

									printf("Process Successfully Ran and Reforked!");
		    						scale = scale - step;
		   		 					sprintf(words[6], "-s %f", scale);
		    						sprintf(words[7], "mandel%d.bmp", counter);
		    						execvp(words[0], words);

		    						counter++;
		    					}

		    					else {
		    						processes[i] = 0;
		    					}

							}

							else {
								perror("new forking failed");
								exit(1);
							}
		 			}



		 			else {
		 				//processes[i];
		 				waiting = 1;
		 			}

		 			//}
		 			sleep(1);
		 		}
		 	}

		 } while(waiting && counter < 50);


	return 0;
}