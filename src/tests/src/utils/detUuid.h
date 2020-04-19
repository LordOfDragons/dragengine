// include only once
#ifndef _DETUUID_H_
#define _DETUUID_H_

// includes
#include "../detCase.h"


// class detUuid
class detUuid : public detCase{
public:
	detUuid();
	~detUuid();
	void Prepare();
	void Run();
	void CleanUp();
	const char *GetTestName();
private:
	void TestInit();
	void TestCompare();
	void TestHexString();
};

// end of include only once
#endif

