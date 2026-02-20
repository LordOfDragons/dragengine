// include only once
#ifndef _DETSTRINGDICTIONARY_H_
#define _DETSTRINGDICTIONARY_H_

// includes
#include "../detCase.h"

// predefinitions


// class detStringDictionary
class detStringDictionary : public detCase{
public:
	detStringDictionary();
	~detStringDictionary() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
	
private:
	void TestDictionary();
};

// end of include only once
#endif

