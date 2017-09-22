#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>

// every thread will have access to these thread id's
// that way, I can have threads wait for (join with)
// any thread that I want
pthread_t pt_hello;
pthread_t pt_world;

void * world(void * param);
void * hello(void * param);

int main(int argc, char ** argv)  {
	// create the two threads
	// here, I will create pt_hello first so it's handle
	// is available for pthread_join in the world thread
	// if you wanted to create world first, a pthread_yield
	// could be used to halt the world thread until the 
	// main thread creates the hello thread, however 
	// I found this solution to be a bit more elegant
	// and less bug prone if you were for example to introduce
	// additional threads to further complicate things
	pthread_create(&pt_hello, NULL, hello, NULL);
	pthread_create(&pt_world, NULL, world, NULL);

	// wait for world to finish (which in turn waits for hello)
	// before going on and printing the new line character
	pthread_join(pt_world, NULL);

	printf("\n");
	return 0;
}

void * world(void * param) {
	// wait for hello to finish before printing world
	pthread_join(pt_hello, NULL);
	printf("world");
	pthread_exit(0);
}

void * hello(void * param) {
	// hello doesn't have to wait for anything
	// it can print hello whenever it sees fit
	printf("hello ");
	pthread_exit(0);
}
