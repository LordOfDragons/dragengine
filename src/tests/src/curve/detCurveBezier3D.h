// include only once
#ifndef _DETCURVEBEZIER3D_H_
#define _DETCURVEBEZIER3D_H_

// includes
#include "../detCase.h"

// predefinitions

// definitions

// class detCurveBezier3D
class detCurveBezier3D : public detCase{
private:
	float threshold;
	float epsilon1;
	float epsilon2;
	
public:
	detCurveBezier3D();
	~detCurveBezier3D();
	void Prepare();
	void Run();
	void CleanUp();
	const char *GetTestName();
	
private:
	void TestInitial();
	void TestEvaluate1();
	void TestEvaluate2();
	void TestMapping();
	void TestMapping2();
};

// end of include only once
#endif

