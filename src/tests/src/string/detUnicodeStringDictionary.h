// include only once
#ifndef _DETUNICODESTRINGDICTIONARY_H_
#define _DETUNICODESTRINGDICTIONARY_H_

// includes
#include "../detCase.h"

// predefinitions


// class detUnicodeStringDictionary
class detUnicodeStringDictionary : public detCase{
public:
	detUnicodeStringDictionary();
	~detUnicodeStringDictionary();
	void Prepare();
	void Run();
	void CleanUp();
	const char *GetTestName();
	
private:
	void TestDictionary();
};

// end of include only once
#endif

