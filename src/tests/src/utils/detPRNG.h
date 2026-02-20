#ifndef _DETPRNG_H_
#define _DETPRNG_H_

#include "../detCase.h"


class detPRNG : public detCase{
public:
	detPRNG();
	~detPRNG() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
private:
	void TestInt();
	void TestFloat();
	void TestDouble();
};

#endif

