// include only once
#ifndef _DETRUNNER_H_
#define _DETRUNNER_H_

// includes
#include "detCase.h"

// class detRunner
class detRunner{
private:
	detCase **pCases;
	int pCount;
public:
	detRunner();
	~detRunner();
	bool Run();
private:
	void pAddTest(detCase *testCase);
};

// end of include only once
#endif

