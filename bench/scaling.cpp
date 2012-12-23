#include "timing.h"
#include <omp.h>
#include <signal.h>

static rangetree<u32, u32> tree;
static volatile u32 running = 0;
static u32 loops = 0;

enum {
	numhigh = 1000000,
	looper = 5000
};

static inline u32 subrand() {
	return rand() % numhigh;
}

static void doit() {
	running = 1;

	while (running) {
		s32 i;
		#pragma omp parallel for
		for (i = 0; i < looper; i++) {
			tree.count(subrand(), subrand(), subrand(), subrand());
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
	for (i = 0; i < numhigh; i++) {
		const u32 x = subrand();
		const u32 y = subrand();
		tree.add(x, y, 0);
	}
	tree.finalize();
	printf("Tree finalized, starting tests.\n");

	u32 results[max + 1];

	for (i = 1; i <= max; i++) {
		omp_set_num_threads(i);
		printf("%u.. ", i);
		fflush(stdout);

		alarm(2);
		doit();

		results[i] = loops/2;
		loops = 0;
	}

	printf("\n\n\nScaling results:\n");
	for (i = 1; i <= max; i++) {
		printf("%u core%s: %u searches/sec, normalized %.2f\n",
			i, i > 1 ? "s" : "",
			results[i],
			(float) results[i]/results[1]);
	}

	return 0;
}
