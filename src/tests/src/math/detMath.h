// include only once
#ifndef _DETMATH_H_
#define _DETMATH_H_

// includes
#include "../detCase.h"

// class detMath
class detMath : public detCase{
public:
	detMath();
	~detMath();
	void Prepare();
	void Run();
	void CleanUp();
	const char *GetTestName();
	
	void TestQuaternion();
};

// end of include only once
#endif

