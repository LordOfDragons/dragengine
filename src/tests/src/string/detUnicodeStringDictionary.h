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
	~detUnicodeStringDictionary() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
	
private:
	void TestDictionary();
};

// end of include only once
#endif

