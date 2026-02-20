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
	~detPath() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
	
private:
	void pTestCreatePath(int testNumber);
	void pTestParenting(int testNumber);
	void pTestComposite(int testNumber);
	void pTestMatchPattern(int testNumber);
	void pTestStaticFactoryMethods(int testNumber);
	void pTestPrefixManagement(int testNumber);
	void pTestPathConversion(int testNumber);
	void pTestPathOperations(int testNumber);
	void pTestComponentManagement(int testNumber);
	void pTestParentPathFunctions(int testNumber);
	void pTestPathComparison(int testNumber);
	void pTestStaticHelpers(int testNumber);
	void pTestAbsoluteRelativePaths(int testNumber);
};

// end of include only once
#endif

