// include only once
#ifndef _DETCURVE2D_H_
#define _DETCURVE2D_H_

// includes
#include "../detCase.h"

// predefinitions

// definitions

// class detCurve2D
class detCurve2D : public detCase{
private:
	float threshold;
	float epsilon1;
	float epsilon2;
	
public:
	detCurve2D();
	~detCurve2D();
	void Prepare();
	void Run();
	void CleanUp();
	const char *GetTestName();
	
private:
	void TestInitial();
	void TestAdd();
	void TestClosest();
	void TestBefore();
	void TestRemove();
	void TestRemoveAll();
	void TestDefault();
	void TestCopy();
	void TestEvaluateConstant();
	void TestEvaluateLinear();
};

// end of include only once
#endif

