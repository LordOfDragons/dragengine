// include only once
#ifndef _DETUNIQUEID_H_
#define _DETUNIQUEID_H_

// includes
#include "../detCase.h"


// class detUniqueID
class detUniqueID : public detCase{
public:
	detUniqueID();
	~detUniqueID();
	void Prepare();
	void Run();
	void CleanUp();
	const char *GetTestName();
private:
	void TestInit();
	void TestIncrement();
	void TestIncrementByInt();
	void TestIncrementByID();
	void TestDecrement();
	void TestCompare();
	void TestHexString();
};

// end of include only once
#endif

