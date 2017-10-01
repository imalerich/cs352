acquire() {
	if (!available) {
		// add this process to the waiting queue
		block();
	}

	// take control of available
	available = false;
}

release() {
	// open up the mutex for the next process we are going to release
	available = true;
	// then release the next process P from the waiting queue
	wakeup(P);

}
