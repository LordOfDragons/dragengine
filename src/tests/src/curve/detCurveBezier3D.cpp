// includes
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "detCurveBezier3D.h"

#include <dragengine/common/curve/decCurveBezier3D.h>
#include <dragengine/common/curve/decCurveBezier3DPoint.h>
#include <dragengine/common/curve/decCurveBezier3DEvaluator.h>
#include <dragengine/common/curve/decCurveDistanceMapping.h>
#include <dragengine/common/exceptions.h>



// Class detCurveBezier3D
////////////////////////////

// Constructors, destructor
/////////////////////////////

detCurveBezier3D::detCurveBezier3D(){
	Prepare();
}

detCurveBezier3D::~detCurveBezier3D(){
	CleanUp();
}



// Testing
////////////

void detCurveBezier3D::Prepare(){
	threshold = 1e-5f;
	epsilon1 = threshold * 0.5f;
	epsilon2 = threshold * 1.5f;
}

void detCurveBezier3D::Run(){
	TestInitial();
	TestEvaluate1();
	TestEvaluate2();
	TestMapping();
	TestMapping2();
}

void detCurveBezier3D::CleanUp(){
}

const char *detCurveBezier3D::GetTestName(){return "CurveBezier3D";}



// Tests
//////////

void detCurveBezier3D::TestInitial(){
	SetSubTestNum(0);
	
	// initial condition
	decCurveBezier3D curve1;
	
	ASSERT_TRUE(curve1.GetPointCount() == 0);
	ASSERT_DOES_FAIL(curve1.GetPointAt(-1));
	ASSERT_DOES_FAIL(curve1.GetPointAt(0));
}

void detCurveBezier3D::TestEvaluate1(){
	SetSubTestNum(1);
	
	// create curve
	const decCurveBezier3DPoint point1(decVector(0.0f, 0.0f, 0.0f),
		decVector(0.0f, 0.0f, -22.0f), decVector(0.0f, 0.0f, 22.0f));
	const decCurveBezier3DPoint point2(decVector(40.0f, 0.0f, 40.0f),
		decVector(18.0f, 0.0f, 40.0f), decVector(62.0f, 0.0f, 40.0f));
	decCurveBezier3D curve;
	
	curve.AddPoint(point1);
	curve.AddPoint(point2);
	
	// test evaluation
	const decCurveBezier3DEvaluator evaluator(curve);
	
	ASSERT_TRUE(evaluator.EvaluateAt(0, 0.0f / 18.0f).IsEqualTo(decVector(0.0f, 0.0f, 0.0f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 1.0f / 18.0f).IsEqualTo(decVector(0.164266f, 0.0f, 3.627229f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 2.0f / 18.0f).IsEqualTo(decVector(0.647462f, 0.0f, 7.165981f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 3.0f / 18.0f).IsEqualTo(decVector(1.435185f, 0.0f, 10.601852f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 4.0f / 18.0f).IsEqualTo(decVector(2.513031f, 0.0f, 13.920440f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 5.0f / 18.0f).IsEqualTo(decVector(3.866598f, 0.0f, 17.107340f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 6.0f / 18.0f).IsEqualTo(decVector(5.481482f, 0.0f, 20.148148f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 7.0f / 18.0f).IsEqualTo(decVector(7.343279f, 0.0f, 23.028463f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 8.0f / 18.0f).IsEqualTo(decVector(9.437586f, 0.0f, 25.733883f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 9.0f / 18.0f).IsEqualTo(decVector(11.750000f, 0.0f, 28.250000f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 10.0f / 18.0f).IsEqualTo(decVector(14.266119f, 0.0f, 30.562414f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 11.0f / 18.0f).IsEqualTo(decVector(16.971537f, 0.0f, 32.656723f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 12.0f / 18.0f).IsEqualTo(decVector(19.851852f, 0.0f, 34.518520f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 13.0f / 18.0f).IsEqualTo(decVector(22.892660f, 0.0f, 36.133400f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 14.0f / 18.0f).IsEqualTo(decVector(26.079559f, 0.0f, 37.486969f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 15.0f / 18.0f).IsEqualTo(decVector(29.398149f, 0.0f, 38.564812f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 16.0f / 18.0f).IsEqualTo(decVector(32.834023f, 0.0f, 39.352539f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 17.0f / 18.0f).IsEqualTo(decVector(36.372772f, 0.0f, 39.835739f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 18.0f / 18.0f).IsEqualTo(decVector(40.0f, 0.0f, 40.0f), threshold));
}

void detCurveBezier3D::TestEvaluate2(){
	SetSubTestNum(2);
	
	// create curve
	const decCurveBezier3DPoint point1(decVector(0.0f, 0.0f, 0.0f),
		decVector(0.0f, 0.0f, -22.0f), decVector(0.0f, 0.0f, 22.0f));
	const decCurveBezier3DPoint point2(decVector(40.0f, 0.0f, 40.0f),
		decVector(18.0f, 0.0f, 40.0f), decVector(62.0f, 0.0f, 40.0f));
	const decCurveBezier3DPoint point3(decVector(0.0f, 0.0f, 80.0f),
		decVector(62.0f, 0.0f, 80.0f), decVector(22.0f, 0.0f, 80.0f));
	decCurveBezier3D curve;
	
	curve.AddPoint(point1);
	curve.AddPoint(point2);
	curve.AddPoint(point3);
	
	// test evaluation
	const decCurveBezier3DEvaluator evaluator(curve);
	
	ASSERT_TRUE(evaluator.EvaluateAt(0, 0.0f / 19.0f).IsEqualTo(decVector(0.000000f, 0.000000f, 0.000000f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 1.0f / 19.0f).IsEqualTo(decVector(0.147543f, 0.000000f, 3.438402f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 2.0f / 19.0f).IsEqualTo(decVector(0.582009f, 0.000000f, 6.798075f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 3.0f / 19.0f).IsEqualTo(decVector(1.291150f, 0.000000f, 10.066773f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 4.0f / 19.0f).IsEqualTo(decVector(2.262721f, 0.000000f, 13.232249f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 5.0f / 19.0f).IsEqualTo(decVector(3.484473f, 0.000000f, 16.282257f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 6.0f / 19.0f).IsEqualTo(decVector(4.944160f, 0.000000f, 19.204548f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 7.0f / 19.0f).IsEqualTo(decVector(6.629538f, 0.000000f, 21.986877f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 8.0f / 19.0f).IsEqualTo(decVector(8.528358f, 0.000000f, 24.617001f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 9.0f / 19.0f).IsEqualTo(decVector(10.628371f, 0.000000f, 27.082668f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 10.0f / 19.0f).IsEqualTo(decVector(12.917335f, 0.000000f, 29.371626f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 11.0f / 19.0f).IsEqualTo(decVector(15.383000f, 0.000000f, 31.471642f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 12.0f / 19.0f).IsEqualTo(decVector(18.013121f, 0.000000f, 33.370461f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 13.0f / 19.0f).IsEqualTo(decVector(20.795452f, 0.000000f, 35.055840f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 14.0f / 19.0f).IsEqualTo(decVector(23.717743f, 0.000000f, 36.515526f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 15.0f / 19.0f).IsEqualTo(decVector(26.767754f, 0.000000f, 37.737282f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 16.0f / 19.0f).IsEqualTo(decVector(29.933226f, 0.000000f, 38.708851f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 17.0f / 19.0f).IsEqualTo(decVector(33.201923f, 0.000000f, 39.417992f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 18.0f / 19.0f).IsEqualTo(decVector(36.561600f, 0.000000f, 39.852455f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(0, 19.0f / 19.0f).IsEqualTo(decVector(40.000000f, 0.000000f, 40.000000f), threshold));
	
	ASSERT_TRUE(evaluator.EvaluateAt(1, 0.0f / 19.0f).IsEqualTo(decVector(40.000000f, 0.000000f, 40.000000f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(1, 1.0f / 19.0f).IsEqualTo(decVector(43.285030f, 0.000000f, 40.320744f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(1, 2.0f / 19.0f).IsEqualTo(decVector(46.169411f, 0.000000f, 41.236336f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(1, 3.0f / 19.0f).IsEqualTo(decVector(48.618168f, 0.000000f, 42.676773f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(1, 4.0f / 19.0f).IsEqualTo(decVector(50.596294f, 0.000000f, 44.572090f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(1, 5.0f / 19.0f).IsEqualTo(decVector(52.068817f, 0.000000f, 46.852310f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(1, 6.0f / 19.0f).IsEqualTo(decVector(53.000729f, 0.000000f, 49.447441f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(1, 7.0f / 19.0f).IsEqualTo(decVector(53.357048f, 0.000000f, 52.287502f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(1, 8.0f / 19.0f).IsEqualTo(decVector(53.102783f, 0.000000f, 55.302528f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(1, 9.0f / 19.0f).IsEqualTo(decVector(52.202950f, 0.000000f, 58.422512f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(1, 10.0f / 19.0f).IsEqualTo(decVector(50.622540f, 0.000000f, 61.577492f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(1, 11.0f / 19.0f).IsEqualTo(decVector(48.326576f, 0.000000f, 64.697479f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(1, 12.0f / 19.0f).IsEqualTo(decVector(45.280075f, 0.000000f, 67.712494f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(1, 13.0f / 19.0f).IsEqualTo(decVector(41.448021f, 0.000000f, 70.552559f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(1, 14.0f / 19.0f).IsEqualTo(decVector(36.795452f, 0.000000f, 73.147690f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(1, 15.0f / 19.0f).IsEqualTo(decVector(31.287357f, 0.000000f, 75.427910f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(1, 16.0f / 19.0f).IsEqualTo(decVector(24.888760f, 0.000000f, 77.323227f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(1, 17.0f / 19.0f).IsEqualTo(decVector(17.564661f, 0.000000f, 78.763680f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(1, 18.0f / 19.0f).IsEqualTo(decVector(9.280066f, 0.000000f, 79.679253f), threshold));
	ASSERT_TRUE(evaluator.EvaluateAt(1, 19.0f / 19.0f).IsEqualTo(decVector(0.000000f, 0.000000f, 80.000000f), threshold));
	
	/*
	printf("\n");
	for(i=0; i<20; i++){
		const decVector point = evaluator.EvaluateAt(0, (float)i / 19.0f);
		printf("ASSERT_TRUE(evaluator.EvaluateAt(0, %i.0f / 19.0f).IsEqualTo(decVector(%ff, %ff, %ff), threshold));\n", i, point.x, point.y, point.z);
	}
	*/
}

void detCurveBezier3D::TestMapping(){
	SetSubTestNum(3);
	
	// create curve
	const decCurveBezier3DPoint point1(decVector(0.0f, 0.0f, 0.0f),
		decVector(0.0f, 0.0f, -22.0f), decVector(0.0f, 0.0f, 22.0f));
	const decCurveBezier3DPoint point2(decVector(40.0f, 0.0f, 40.0f),
		decVector(18.0f, 0.0f, 40.0f), decVector(62.0f, 0.0f, 40.0f));
	const decCurveBezier3DPoint point3(decVector(0.0f, 0.0f, 80.0f),
		decVector(62.0f, 0.0f, 80.0f), decVector(22.0f, 0.0f, 80.0f));
	decCurveBezier3D curve;
	
	curve.AddPoint(point1);
	curve.AddPoint(point2);
	curve.AddPoint(point3);
	
	// test mapping.
	// resolution=2.0 sampleCount=200: errorRange=0.011775
	// resolution=1.0 sampleCount=50: errorRange=0.018223
	// resolution=1.0 sampleCount=100: errorRange=0.009115
	// resolution=1.0 sampleCount=200: errorRange=0.004782
	// resolution=0.5 sampleCount=50: errorRange=0.018026
	// resolution=0.5 sampleCount=100: errorRange=0.005396
	// resolution=0.5 sampleCount=200: errorRange=0.002586
	// resolution=0.25 sampleCount=100: errorRange=0.003593
	// resolution=0.25 sampleCount=200: errorRange=0.001538
	const decCurveBezier3DEvaluator evaluator(curve);
	decCurveDistanceMapping mapping;
	
	mapping.Init(curve, 1.0f, 50);
	
	ASSERT_FEQUAL(mapping.GetLength(), 148.890076f);
	
	int segment;
	float blend;
	mapping.Map(0.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.000000f);
	mapping.Map(1.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.058491f);
	mapping.Map(2.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.117909f);
	mapping.Map(3.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.178149f);
	mapping.Map(4.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.239034f);
	mapping.Map(5.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.300400f);
	mapping.Map(6.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.362116f);
	mapping.Map(7.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.424029f);
	mapping.Map(8.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.486059f);
	mapping.Map(9.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.548109f);
	mapping.Map(10.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.610087f);
	mapping.Map(11.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.671906f);
	mapping.Map(12.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.733438f);
	mapping.Map(13.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.794544f);
	mapping.Map(14.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.855114f);
	mapping.Map(15.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.914952f);
	mapping.Map(16.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.973861f);
	mapping.Map(17.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.032989f);
	mapping.Map(18.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.098150f);
	mapping.Map(19.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.168464f);
	mapping.Map(20.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.242043f);
	mapping.Map(21.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.315014f);
	mapping.Map(22.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.384407f);
	mapping.Map(23.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.448972f);
	mapping.Map(24.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.507887f);
	mapping.Map(25.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.561331f);
	mapping.Map(26.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.609983f);
	mapping.Map(27.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.654379f);
	mapping.Map(28.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.695090f);
	mapping.Map(29.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.732493f);
	mapping.Map(30.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.767058f);
	mapping.Map(31.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.799423f);
	mapping.Map(32.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.829520f);
	mapping.Map(33.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.858066f);
	mapping.Map(34.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.884767f);
	mapping.Map(35.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.910074f);
	mapping.Map(36.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.934141f);
	mapping.Map(37.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.957032f);
	mapping.Map(38.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 1);
	ASSERT_FEQUAL(blend, 0.978994f);
	
	/*
	printf("\n");
	for(int i=0; i<39; i++){
		int segment;
		float blend;
		mapping.Map((float)i * mapping.GetLength() / 39.0f, segment, blend);
		printf("mapping.Map(%i.0f * mapping.GetLength() / 39.0f, segment, blend);\n", i);
		printf("ASSERT_EQUAL(segment, %i);\n", segment);
		printf("ASSERT_FEQUAL(blend, %ff);\n", blend);
	}
	*/
	
	/*
	int segment1;
	int segment2;
	float blend1;
	float blend2;
	float minlen = 0.0f;
	float maxlen = 0.0f;
	
	for(int i=0; i<99; i++){
		mapping.Map((float)i * mapping.GetLength() / 99.0f, segment1, blend1);
		mapping.Map((float)(i+1) * mapping.GetLength() / 99.0f, segment2, blend2);
		
		const decVector p1 = evaluator.EvaluateAt(segment1, blend1);
		const decVector p2 = evaluator.EvaluateAt(segment2, blend2);
		const float len = (p2 - p1).Length();
		
		if(i == 0){
			minlen = maxlen = len;
			
		}else{
			minlen = decMath::min(minlen, len);
			maxlen = decMath::max(maxlen, len);
		}
		
		printf("mapping %3i (%i|%f) -> (%i|%f) length=%f\n", i, segment1, blend1, segment2, blend2, len);
	}
	
	printf("mapping spread min=%f max=%f range=%f\n", minlen, maxlen, (maxlen - minlen) * 0.5f);
	*/
}

void detCurveBezier3D::TestMapping2(){
	SetSubTestNum(4);
	
	// create curve
	const decCurveBezier3DPoint point1(decVector(0.0f, 0.0f, 0.0f),
		decVector(0.0f, 0.0f, -5.5f), decVector(0.0f, 0.0f, 5.5f));
	const decCurveBezier3DPoint point2(decVector(-4.019f, 0.0f, 15.0f),
		decVector(-1.612f, 0.0f, 10.902f), decVector(-6.630f, 0.0f, 19.444f));
	decCurveBezier3D curve;
	
	curve.AddPoint(point1);
	curve.AddPoint(point2);
	
	// test mapping.
	const decCurveBezier3DEvaluator evaluator(curve);
	decCurveDistanceMapping mapping;
	
	//printf( "\n\n\n" );
	mapping.Init(curve, 1.0f, 50);
	
	ASSERT_FEQUAL(mapping.GetLength(), 15.705114f);
	
	int segment;
	float blend;
	mapping.Map(0.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.000000f);
	mapping.Map(1.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.024443f);
	mapping.Map(2.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.048887f);
	mapping.Map(3.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.073360f);
	mapping.Map(4.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.097861f);
	mapping.Map(5.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.122364f);
	mapping.Map(6.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.146939f);
	mapping.Map(7.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.171515f);
	mapping.Map(8.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.196140f);
	mapping.Map(9.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.220807f);
	mapping.Map(10.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.245481f);
	mapping.Map(11.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.270257f);
	mapping.Map(12.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.295033f);
	mapping.Map(13.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.319885f);
	mapping.Map(14.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.344791f);
	mapping.Map(15.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.369712f);
	mapping.Map(16.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.394772f);
	mapping.Map(17.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.419831f);
	mapping.Map(18.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.445002f);
	mapping.Map(19.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.470241f);
	mapping.Map(20.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.495508f);
	mapping.Map(21.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.520956f);
	mapping.Map(22.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.546404f);
	mapping.Map(23.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.572009f);
	mapping.Map(24.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.597699f);
	mapping.Map(25.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.623443f);
	mapping.Map(26.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.649459f);
	mapping.Map(27.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.675476f);
	mapping.Map(28.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.701685f);
	mapping.Map(29.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.727983f);
	mapping.Map(30.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.754352f);
	mapping.Map(31.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.781005f);
	mapping.Map(32.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.807658f);
	mapping.Map(33.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.834661f);
	mapping.Map(34.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.861802f);
	mapping.Map(35.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.889040f);
	mapping.Map(36.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.916594f);
	mapping.Map(37.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.944149f);
	mapping.Map(38.0f * mapping.GetLength() / 39.0f, segment, blend);
	ASSERT_EQUAL(segment, 0);
	ASSERT_FEQUAL(blend, 0.972022f);
	
	/*
	printf("\n");
	for(int i=0; i<39; i++){
		int segment;
		float blend;
		mapping.Map((float)i * mapping.GetLength() / 39.0f, segment, blend);
		printf("mapping.Map(%i.0f * mapping.GetLength() / 39.0f, segment, blend);\n", i);
		printf("ASSERT_EQUAL(segment, %i);\n", segment);
		printf("ASSERT_FEQUAL(blend, %ff);\n", blend);
	}
	*/
	
	/*
	int segment1;
	int segment2;
	float blend1;
	float blend2;
	float minlen = 0.0f;
	float maxlen = 0.0f;
	
	printf("mapping length=%f\n", mapping.GetLength());
	
	for(int i=0; i<99; i++){
		mapping.Map((float)i * mapping.GetLength() / 99.0f, segment1, blend1);
		mapping.Map((float)(i+1) * mapping.GetLength() / 99.0f, segment2, blend2);
		
		const decVector p1 = evaluator.EvaluateAt(segment1, blend1);
		const decVector p2 = evaluator.EvaluateAt(segment2, blend2);
		const float len = (p2 - p1).Length();
		
		if(i == 0){
			minlen = maxlen = len;
			
		}else{
			minlen = decMath::min(minlen, len);
			maxlen = decMath::max(maxlen, len);
		}
		
		printf("mapping %2i (%i|%f) -> (%i|%f) length=%f\n", i, segment1, blend1, segment2, blend2, len);
	}
	
	printf("mapping spread min=%f max=%f range=%f\n", minlen, maxlen, (maxlen - minlen) * 0.5f);
	*/
}
