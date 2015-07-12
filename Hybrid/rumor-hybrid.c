/* Parallelization: Infectious Disease
 * By Aaron Weeden, Shodor Education Foundation, Inc.
 * November 2011
 * Modified by Phil List, Shodor Education Foundation, Inc.
 * May 2015
 *
 * Parallel code -- MPI for distributed memory (processes), OpenMP for shared
 *  memory (threads). "Hybrid" uses both (in which case each MPI process can 
 *  spawn OpenMP threads).
 *
 * Parts corresponding to the module's algorithm are indicated by comments that
 *  begin with ALG 1:, ALG 1.A:, ALG 1.A.1:, etc.
 *
 * Note on naming scheme:  Variables that begin with "our" are private to
 *  processes and shared by threads ("our" is from the perspective of the
 *  threads).  Variables that begin with "my" are private to threads (again,
 *  "my" from the perspective of threads). */

//include <assert.h> /* for assert */
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc, free, and various others */
#include <time.h> /* time is used to seed the random number generator */
#include <unistd.h> /* random, getopt, some others */

#include <mpi.h> /* MPI_Allgather, MPI_Init, MPI_Comm_rank, MPI_Comm_size */
#include <omp.h>
/* States of people -- all people are one of these 4 states */
/* These are const char because they are displayed as ASCII if TEXT_DISPLAY 
 *  is enabled */
const char INFORMED = '!';
const char APATHETIC = 'Z';
const char UNINFORMED = '?';
const char DEAD = 'X';


/* PROGRAM EXECUTION BEGINS HERE */
int main(int argc, char** argv) {
    /** Declare variables **/
    /* People */
    int total_number_of_people = 60;
    int total_num_initially_informed = 1;
    int total_num_informed = 1;
    int our_number_of_people = 60;
    int our_person1 = 0;
    int our_current_informed_person = 0;
    int our_num_initially_informed = 1;
    int our_num_informed = 0;
    int our_current_location_x = 0;
    int our_current_location_y = 0;
    int our_num_uninformed = 0;
    int our_num_apathetic = 0;
    int our_num_dead = 0;
    int my_current_person_id = 0;
    int my_num_informed_nearby = 0;
    int my_person2 = 0;
	double fixcount = 0;
    /* Environment */
    int environment_width = 45;
    int environment_height = 20;

    /* Disease */
    int earshot_distance = 2;
    int length_of_news_cycle = 50;
    int intrigue_factor = 30;
    int mortality_rate_per_10k = 5;
//#ifdef SHOW_RESULTS
    double our_num_rumors = 0.0;
    double our_num_rumor_attempts = 0.0;
    double our_num_deaths = 0.0;
    double our_num_who_lost_interest = 0.0;
//#endif

    /* Time */
    int total_number_of_days = 250;
    int our_current_day = 0;
    int microseconds_per_day = 100000;

    /* Movement */
    int my_x_move_direction = 0; 
    int my_y_move_direction = 0;

    /* Distributed Memory Information */
    int total_number_of_processes = 1;
    int our_rank = 0;
    int current_rank = 0;
    int current_displ = 0;

    /* getopt */
    int c = 0;

    /* Integer arrays, a.k.a. integer pointers */
    int *x_locations;
    int *y_locations;
    int *our_x_locations;
    int *our_y_locations;
    int *our_informed_x_locations;
    int *our_informed_y_locations;
    int *their_informed_x_locations;
    int *their_informed_y_locations;
    int *our_num_days_informed;
    int *recvcounts;
    int *displs;

    /* Character arrays, a.k.a. character pointers */
    char *states;
    char *our_states;

#ifdef TEXT_DISPLAY
    /* Array of character arrays, a.k.a. array of character pointers, for text
     *  display */
    char **environment;
#endif


    /* Each process initializes the distributed memory environment */
    MPI_Init(&argc, &argv);

    /* ALG 1: Each process determines its rank and the total number of processes     */
    MPI_Comm_rank(MPI_COMM_WORLD, &our_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &total_number_of_processes);

    /* ALG 2: Each process is given the parameters of the simulation */
    /* Get command line options -- this follows the idiom presented in the
     *  getopt man page (enter 'man 3 getopt' on the shell for more) */
    while((c = getopt(argc, argv, "n:i:w:h:t:T:c:d:D:m:")) != -1)
    {
        switch(c)
        {
            case 'n':
                total_number_of_people = atoi(optarg);
                break;
            case 'i':
                total_num_initially_informed = atoi(optarg);
                break;
            case 'w':
                environment_width = atoi(optarg);
                break;
            case 'h':
                environment_height = atoi(optarg);
                break;
            case 't':
                total_number_of_days = atoi(optarg);
                break;
            case 'T':
                length_of_news_cycle = atoi(optarg);
                break;
            case 'c':
                intrigue_factor = atoi(optarg);
                break;
            case 'd':
                earshot_distance = atoi(optarg);
                break;
            case 'D':
                mortality_rate_per_10k = atoi(optarg);
                break;
            case 'm':
                microseconds_per_day = atoi(optarg);
                break;
                /* If the user entered "-?" or an unrecognized option, we need 
                 *  to print a usage message before exiting. */
            case '?':
            default:
                fprintf(stderr, "Usage: ");
                fprintf(stderr, "mpirun -np total_number_of_processes ");
                fprintf(stderr, "%s [-n total_number_of_people][-i total_num_initially_informed][-w environment_width][-h environment_height][-t total_number_of_days][-T length_of_news_cycle][-c intrigue_factor][-d earshot_distance][-D mortality_rate_per_10k][-m microseconds_per_day]\n", argv[0]);
                exit(-1);
        }
    }
    argc -= optind;
    argv += optind;

    /* ALG 3: Each process makes sure that the total number of initially 
     *  informed people is less than the total number of people */
    if(total_num_initially_informed > total_number_of_people)
    {
        fprintf(stderr, "ERROR: initial number of informed (%d) must be less than total number of people (%d)\n", total_num_initially_informed, 
                total_number_of_people);
        exit(-1);
    }

    /* ALG 4: Each process determines the number of people for which it is 
     *  responsible */
    our_number_of_people = total_number_of_people / total_number_of_processes;

    /* ALG 5: The last process is responsible for the remainder */
    if(our_rank == total_number_of_processes - 1)
    {
        our_number_of_people += total_number_of_people % total_number_of_processes;
    }

    /* ALG 6: Each process determines the number of initially informed people 
     *  for which it is responsible */
    our_num_initially_informed = total_num_initially_informed 
        / total_number_of_processes;

    /* ALG 7: The last process is responsible for the remainder */
    if(our_rank == total_number_of_processes - 1)
    {
        our_num_initially_informed += total_num_initially_informed 
            % total_number_of_processes;
    }

    /* Allocate the arrays */
    x_locations = (int*)malloc(total_number_of_people * sizeof(int));
    y_locations = (int*)malloc(total_number_of_people * sizeof(int));
    our_x_locations = (int*)malloc(our_number_of_people * sizeof(int));
    our_y_locations = (int*)malloc(our_number_of_people * sizeof(int));
    our_informed_x_locations = (int*)malloc(our_number_of_people * sizeof(int));
    our_informed_y_locations = (int*)malloc(our_number_of_people * sizeof(int));
    their_informed_x_locations = (int*)malloc(total_number_of_people 
            * sizeof(int));
    their_informed_y_locations = (int*)malloc(total_number_of_people 
            * sizeof(int));
    our_num_days_informed = (int*)malloc(our_number_of_people * sizeof(int));
    recvcounts = (int*)malloc(total_number_of_processes * sizeof(int));
    displs = (int*)malloc(total_number_of_processes * sizeof(int));
    states = (char*)malloc(total_number_of_people * sizeof(char));
    our_states = (char*)malloc(our_number_of_people * sizeof(char));

#ifdef TEXT_DISPLAY
    environment = (char**)malloc(environment_width * environment_height
            * sizeof(char*));
    for(our_current_location_x = 0;
            our_current_location_x <= environment_width - 1;
            our_current_location_x++) {
        environment[our_current_location_x] = (char*)malloc(environment_height
                * sizeof(char));
    }
#endif

    /* ALG 8: Each process seeds the random number generator based on the
     *  current time */
    srandom(time(NULL) + our_rank * 12345);

    /* ALG 9: Each process spawns threads to set the states of the initially 
     *  informed people and set the count of its informed people */
#pragma omp parallel for private(my_current_person_id) \
    reduction(+:our_num_informed)
    for(my_current_person_id = 0; my_current_person_id 
            <= our_num_initially_informed - 1; my_current_person_id++) {	
        our_states[my_current_person_id] = INFORMED;
        our_num_informed++;
    }

    /* ALG 10: Each process spawns threads to set the states of the rest of its 
     *  people and set the count of its uninformed people */
#pragma omp parallel for private(my_current_person_id) \
    reduction(+:our_num_uninformed)
    for(my_current_person_id = our_num_initially_informed; 
            my_current_person_id <= our_number_of_people - 1; 
            my_current_person_id++) {
        our_states[my_current_person_id] = UNINFORMED;
        our_num_uninformed++;
    }

    /* ALG 11: Each process spawns threads to set random x and y locations for 
     *  each of its people */
#pragma omp parallel for private(my_current_person_id)
    for(my_current_person_id = 0;
            my_current_person_id <= our_number_of_people - 1; 
            my_current_person_id++) {
        our_x_locations[my_current_person_id] = random() % environment_width;
        our_y_locations[my_current_person_id] = random() % environment_height;
    }

    /* ALG 12: Each process spawns threads to initialize the number of days 
     *  informed of each of its people to 0 */
#pragma omp parallel for private(my_current_person_id)
    for(my_current_person_id = 0;
            my_current_person_id <= our_number_of_people - 1;
            my_current_person_id++) {
        our_num_days_informed[my_current_person_id] = 0;
    }


    /* ALG 14: Each process starts a loop to run the simulation for the
     *  specified number of days */
    for(our_current_day = 0; our_current_day <= total_number_of_days - 1; 
            our_current_day++) {
        /* ALG 14.A: Each process determines its informed x locations and 
         *  informed y locations */
        our_current_informed_person = 0;
        for(our_person1 = 0; our_person1 <= our_number_of_people - 1;
                our_person1++) {
            if(our_states[our_person1] == INFORMED) {
                our_informed_x_locations[our_current_informed_person] =
                    our_x_locations[our_person1];
                our_informed_y_locations[our_current_informed_person] =
                    our_y_locations[our_person1];
                our_current_informed_person++;
            }
        }
        /* ALG 14.B: Each process sends its count of informed people to all the
         *  other processes and receives their counts */
        MPI_Allgather(&our_num_informed, 1, MPI_INT, recvcounts, 1, 
                MPI_INT, MPI_COMM_WORLD);

        total_num_informed = 0;
        for(current_rank = 0; current_rank <= total_number_of_processes - 1;
                current_rank++) {
            total_num_informed += recvcounts[current_rank];
        }

        /* Set up the displacements in the receive buffer (see the man page for 
         *  MPI_Allgatherv) */
        current_displ = 0;
        for(current_rank = 0; current_rank <= total_number_of_processes - 1;
                current_rank++) {
            displs[current_rank] = current_displ;
            current_displ += recvcounts[current_rank];
        }
	
	/* ALG 14.C: Each process sends the x locations of its informed people 
         *  to all the other processes and receives the x locations of their 
         *  informed people */
        MPI_Allgatherv(our_informed_x_locations, our_num_informed, MPI_INT, 
                their_informed_x_locations, recvcounts, displs, 
                MPI_INT, MPI_COMM_WORLD);


        /* ALG 14.D: Each process sends the y locations of its informed people 
         *  to all the other processes and receives the y locations of their 
         *  informed people */
        MPI_Allgatherv(our_informed_y_locations, our_num_informed, MPI_INT, 
                their_informed_y_locations, recvcounts, displs, 
                MPI_INT, MPI_COMM_WORLD);

#ifdef TEXT_DISPLAY
        /* ALG 14.E: If display is enabled, Rank 0 gathers the states, x 
         *  locations, and y locations of the people for which each process is 
         *  responsible */
        /* Set up the receive counts and displacements in the receive buffer 
         *  (see the man page for MPI_Gatherv) */
        current_displ = 0;
        for(current_rank = 0; current_rank <= total_number_of_processes - 1;
                current_rank++) {
            displs[current_rank] = current_displ;
            recvcounts[current_rank] = total_number_of_people
                / total_number_of_processes;
            if(current_rank == total_number_of_processes - 1) {
                recvcounts[current_rank] += total_number_of_people
                    % total_number_of_processes;
            }
            current_displ += recvcounts[current_rank];
        }


        //if(our_number_of_people<0){our_number_of_people=0;fixcount++;printf("RANK %d fixed it to %d \n", our_rank,our_number_of_people);}

        MPI_Gatherv(our_states, our_number_of_people, MPI_CHAR, states,
                recvcounts, displs, MPI_CHAR, 0, MPI_COMM_WORLD);
        MPI_Gatherv(our_x_locations, our_number_of_people, MPI_INT, x_locations,
                recvcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Gatherv(our_y_locations, our_number_of_people, MPI_INT, y_locations,
                recvcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

        /* ALG 14.F: Rank 0 displays a graphic of the current day */
        if(our_rank == 0) {
            for(our_current_location_y = 0; 
                    our_current_location_y <= environment_height - 1;
                    our_current_location_y++) {
                for(our_current_location_x = 0; our_current_location_x 
                        <= environment_width - 1; our_current_location_x++) {
                    environment[our_current_location_x][our_current_location_y] = ' ';
                }
            }

            for(my_current_person_id = 0; 
                    my_current_person_id <= total_number_of_people - 1;
                    my_current_person_id++) {
                environment[x_locations[my_current_person_id]]
                    [y_locations[my_current_person_id]] = 
                    states[my_current_person_id];
            }
		for(int wd = 0; wd < environment_width; wd++){
			printf("-");
		}
		printf("\n");
            for(our_current_location_y = 0;
                    our_current_location_y <= environment_height - 1;
                    our_current_location_y++) {
                for(our_current_location_x = 0; our_current_location_x <= environment_width - 1; our_current_location_x++) {
                    printf("%c", environment[our_current_location_x][our_current_location_y]);
                }
                printf("\n");
            }
        }

        /* Wait between frames of animation */
        usleep(microseconds_per_day);
#endif

        /* ALG 14.G: For each of the process’s people, each process spawns 
         *  threads to do the following */
#pragma omp parallel for private(my_current_person_id, my_x_move_direction, my_y_move_direction)
        for(my_current_person_id = 0; my_current_person_id <= our_number_of_people - 1; my_current_person_id++)
        {
            /* ALG 14.G.1: If the person is not dead, then */
            if(our_states[my_current_person_id] != DEAD)
            {
                /* ALG 14.G.1.a: The thread randomly picks whether the person 
                 *  moves left or right or does not move in the x dimension */
                my_x_move_direction = (random() % 3) - 1;

                /* ALG 14.G.1.b: The thread randomly picks whether the person 
                 *  moves up or down or does not move in the y dimension */
                my_y_move_direction = (random() % 3) - 1;

                /* ALG 14.G.1.c: If the person will remain in the bounds of the
                 *  environment after moving, then */
                if((our_x_locations[my_current_person_id] + my_x_move_direction >= 0)
                        && (our_x_locations[my_current_person_id] + my_x_move_direction < environment_width)
                        && (our_y_locations[my_current_person_id] + my_y_move_direction >= 0)
                        && (our_y_locations[my_current_person_id] + my_y_move_direction < environment_height))
                {
                    /* ALG 14.G.i: The thread moves the person */
                    our_x_locations[my_current_person_id] += my_x_move_direction;
                    our_y_locations[my_current_person_id] += my_y_move_direction;
                }
            }
        }

        /* ALG 14.H: For each of the process’s people, each process spawns 
         *  threads to do the following */
#pragma omp parallel for private(my_current_person_id, my_num_informed_nearby, \
        my_person2) reduction(+:our_num_rumor_attempts) \
        reduction(+:our_num_informed) reduction(+:our_num_uninformed) \
        reduction(+:our_num_rumors)
        for(my_current_person_id = 0; my_current_person_id <= our_number_of_people - 1; my_current_person_id++) {
            /* ALG 14.H.1: If the person is uninformed, then */
            if(our_states[my_current_person_id] == UNINFORMED) {
                /* ALG 14.H.1.a: For each of the informed people (received
                 *  earlier from all processes) or until the number of informed 
                 *  people within earshot is 1, the thread does the following */
                my_num_informed_nearby = 0;
                for(my_person2 = 0; my_person2 <= total_num_informed - 1 && my_num_informed_nearby < 1; my_person2++) {
                    /* ALG 14.H.1.a.i: If person 1 is within earshot, then */
                    if((our_x_locations[my_current_person_id] 
                                > their_informed_x_locations[my_person2]
                                - earshot_distance)
                            && (our_x_locations[my_current_person_id] 
                                < their_informed_x_locations[my_person2] 
                                + earshot_distance)
                            && (our_y_locations[my_current_person_id]
                                > their_informed_y_locations[my_person2] 
                                - earshot_distance)
                            && (our_y_locations[my_current_person_id]
                                < their_informed_y_locations[my_person2] 
                                + earshot_distance)) {
                        /* ALG 14.H.1.a.i.1: The thread increments the number 
                         *  of informed people nearby */
                        my_num_informed_nearby++;
                    }
                }

#ifdef SHOW_RESULTS
                if(my_num_informed_nearby >= 1)
                    our_num_rumor_attempts++;
#endif

                /* ALG 14.H.1.b: If there is at least 1 informed person nearby, and a random 
		 * number between 0 and 100 is <= or to the intrigue factor, then */
                if(my_num_informed_nearby >= 1 && (random() % 100) <= intrigue_factor) {
                    /* ALG 14.H.1.b.i: The thread changes person1’s state to informed */
                    our_states[my_current_person_id] = INFORMED;

                    /* ALG 14.H.1.b.ii: The thread updates the counters */
                    our_num_informed++;
                    our_num_uninformed--;

#ifdef SHOW_RESULTS
                    our_num_rumors++;
#endif
                }
            }
        }

        /* ALG 14: For each of the process’s people, each process spawns 
         *  threads to do the following */
#pragma omp parallel for private(my_current_person_id) \
        reduction(+:our_num_who_lost_interest) reduction(+:our_num_dead) \
        reduction(+:our_num_informed) reduction(+:our_num_deaths) \
        reduction(+:our_num_apathetic)
        for(my_current_person_id = 0; my_current_person_id <= our_number_of_people - 1; my_current_person_id++) {

		/* ALG 14.I: If person has known the info for 1 news cycle, 
		 * there's a 15% chance of losing interest each time step */
		if((random() % 100) < 15 && 
			our_states[my_current_person_id]==INFORMED && our_num_days_informed[my_current_person_id] >= length_of_news_cycle) {
#ifdef SHOW_RESULTS
			our_num_who_lost_interest++;
#endif
			/* ALG 14.I.a: The thread changes the person’s state to apathetic */
			our_states[my_current_person_id] = APATHETIC;
			/* ALG 14.I.b: The thread updates the counters */
			our_num_apathetic++;
			our_num_informed--;
		}
		/* ALG 14.II: There's a chance each time step that any person will die, default is 10 per 10000 */
                if(our_states[my_current_person_id] != DEAD && (random() % 10000) <= mortality_rate_per_10k) {
			/* ALG 14.II.b: The thread updates the counters */
			our_num_dead++;
			if(our_states[my_current_person_id] == INFORMED){
				our_num_informed--;
			}else if(our_states[my_current_person_id] == UNINFORMED){
                                our_num_uninformed--;
                        }else if(our_states[my_current_person_id] == APATHETIC){
                                our_num_apathetic--;
                        }
			/* ALG 14.II.a: The thread changes the person’s state to dead */
                        our_states[my_current_person_id] = DEAD;
#ifdef SHOW_RESULTS
			our_num_deaths++;
#endif
                }

		/* ALG 14.III: If the person is informed, then */
		if(our_states[my_current_person_id] == INFORMED) {
                	/* ALG 14.III.a: Increment number of days the person has been informed */
			our_num_days_informed[my_current_person_id]++;
		}
        }
    }


#ifdef SHOW_RESULTS
    printf("Rank %d final counts: %d uninformed, %d informed, %d apathetic, %d dead\n", 
		our_rank, our_num_uninformed, our_num_informed, our_num_apathetic, our_num_dead); 
#endif

    /* Deallocate the arrays -- we have finished using the memory, so now we
     *  "free" it back to the heap */
#ifdef TEXT_DISPLAY 
    for(our_current_location_x = environment_width - 1; our_current_location_x >= 0; our_current_location_x--){
        free(environment[our_current_location_x]);
    }
    free(environment);
#endif
    free(our_states);
    free(states);
    free(displs);
    free(recvcounts);
    free(our_num_days_informed);
    free(their_informed_y_locations);
    free(their_informed_x_locations);
    free(our_informed_y_locations);
    free(our_informed_x_locations);
    free(our_y_locations);
    free(our_x_locations);
    free(y_locations);
    free(x_locations);

    /* MPI execution is finished; no MPI calls are allowed after this */
    MPI_Finalize();

    /* The program has finished executing successfully */
    return 0;
}
