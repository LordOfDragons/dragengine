// include only once
#ifndef _DETPATH_H_
#define _DETPATH_H_

// includes
#include "../detCase.h"

// predefinitions
class decPath;


// class detPath
class detPath : public detCase{
public:
	detPath();
	~detPath();
	void Prepare();
	void Run();
	void CleanUp();
	const char *GetTestName();
	
private:
	void pTestCreatePath(int testNumber);
	void pTestParenting(int testNumber);
	void pTestComposite(int testNumber);
	void pTestMatchPattern(int testNumber);
};

// end of include only once
#endif

