// include only once
#ifndef _DETMATH_H_
#define _DETMATH_H_

// includes
#include "../detCase.h"

// class detMath
class detMath : public detCase{
public:
	detMath();
	~detMath() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
	
	void TestQuaternion();
};

// end of include only once
#endif

