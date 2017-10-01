do {
	flag[i] = true;

	while (flag[j]) {
		if (turn == j) {
			flag[i] = false;
			// do nothing
			while (turn == j) { }
			flag[i] = true;
		}
	}

	/* critical section */

	turn = j;
	flag[i] = false;

	/* remainder section */
} while (true)
