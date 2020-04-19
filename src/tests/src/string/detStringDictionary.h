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
	~detStringDictionary();
	void Prepare();
	void Run();
	void CleanUp();
	const char *GetTestName();
	
private:
	void TestDictionary();
};

// end of include only once
#endif

