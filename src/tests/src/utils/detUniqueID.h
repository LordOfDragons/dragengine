// include only once
#ifndef _DETUNIQUEID_H_
#define _DETUNIQUEID_H_

// includes
#include "../detCase.h"


// class detUniqueID
class detUniqueID : public detCase{
public:
	detUniqueID();
	~detUniqueID() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
private:
	void TestInit();
	void TestIncrement();
	void TestIncrementByInt();
	void TestIncrementByID();
	void TestDecrement();
	void TestCompare();
	void TestHexString();
	void TestDEHash();
	void TestDECompare();
};

// end of include only once
#endif

