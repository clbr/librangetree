#include "timing.h"
#include <omp.h>
#include <signal.h>

static rangetree<u32, u32> tree;
static volatile u32 running = 0;
static u32 loops = 0;

enum {
	numhigh = 1000000,
	looper = 10
};

static void doit() {
	running = 1;

	while (running) {
		s32 i;
		#pragma omp parallel for
		for (i = 0; i < looper; i++) {
			tree.count(i*67, i*431, i*767, i*2222);
		}
		loops += looper;
//		usleep(1);
	}
}

static void sig(int a) {
	if (a == SIGALRM)
		running = 0;
}

int main() {

	srand(time(NULL));

	const u32 max = omp_get_num_procs();
	printf("Running scaling tests, two seconds per core...\n");
	printf("Detected a total of %u cores.\n", max);

	u32 i;

	// Set the signaller
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = sig;
	sigaction(SIGALRM, &sa, NULL);

	// init the tree
	for (i = 0; i < numhigh / 1000; i++) {
		const u32 x = rand() % numhigh;
		const u32 y = rand() % numhigh;
		tree.add(x, y, 0);
	}
	tree.finalize();
	printf("Tree finalized, starting tests.\n");

	u32 results[max + 1];

	for (i = 1; i <= max; i++) {
		omp_set_num_threads(i);
		alarm(2);
		doit();

		results[i] = loops/2;
		loops = 0;
	}

	printf("\n\nScaling results:\n");
	for (i = 1; i <= max; i++) {
		printf("%u core%s: %u searches/sec\n",
			i, i > 1 ? "s" : "",
			results[i]);
	}

	return 0;
}
