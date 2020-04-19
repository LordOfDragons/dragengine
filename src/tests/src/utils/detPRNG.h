#ifndef _DETPRNG_H_
#define _DETPRNG_H_

#include "../detCase.h"


class detPRNG : public detCase{
public:
	detPRNG();
	~detPRNG();
	void Prepare();
	void Run();
	void CleanUp();
	const char *GetTestName();
private:
	void TestInt();
	void TestFloat();
	void TestDouble();
};

#endif

