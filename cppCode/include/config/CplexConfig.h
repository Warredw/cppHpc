#pragma once

class CplexConfig {
public:
	static int& getNumThreads() {
		static int threads = 1;
		return threads;
	}

	static void setNumThreads(int t) {
		getNumThreads() = t;
	}
};