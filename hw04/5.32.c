// Use of the Monitor will look something like this.
ReadFile.begin_access(pnum); // pass the assigned process number
/* Access file... */
ReadFile.end_access(pnum);

Monitor ReadFile {
	// data section
	int sum; // current sum of readers, sum < n by definition of problem
	condition available; // is the file available for reading
	
	// procedure section
	void begin_access(int pnum) {
		if (pnum >= n) {
			// ERROR - this process can never access the file
			// and would block all future processes from doing so
			// here I will just terminate the calling process
			exit(-1);
		}

		// wait until there is enough room, 
		// if sum = 8, n = 9 and pnum = 3
		// we may have to wait for a couple processes
		// to finish if their pnum is only 1
		// before we can continue
		while ((sum + pnum) >= n) {
			available.wait();
		}

		sum += pnum;
	}

	void end_access(int pnum) {
		// open up some space
		sum -= pnum;

		// check if there is enough room now for the next process
		// if not, they will continue to wait
		available.signal();
	}
}
