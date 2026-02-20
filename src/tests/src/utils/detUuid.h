// include only once
#ifndef _DETUUID_H_
#define _DETUUID_H_

// includes
#include "../detCase.h"


// class detUuid
class detUuid : public detCase{
public:
	detUuid();
	~detUuid() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
private:
	void TestInit();
	void TestCompare();
	void TestHexString();
	void TestDEHash();
};

// end of include only once
#endif

