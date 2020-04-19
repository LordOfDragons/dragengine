// includes
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "detCurve2D.h"

#include <dragengine/common/curve/decCurve2D.h>
#include <dragengine/common/exceptions.h>



// Class detCurve2D
/////////////////////

// Constructors, destructor
/////////////////////////////

detCurve2D::detCurve2D(){
	Prepare();
}

detCurve2D::~detCurve2D(){
	CleanUp();
}



// Testing
////////////

void detCurve2D::Prepare(){
	threshold = 1e-5f;
	epsilon1 = threshold * 0.5f;
	epsilon2 = threshold * 1.5f;
}

void detCurve2D::Run(){
	TestInitial();
	TestAdd();
	TestClosest();
	TestBefore();
	TestRemove();
	TestRemoveAll();
	TestDefault();
	TestCopy();
	TestEvaluateConstant();
	TestEvaluateLinear();
}

void detCurve2D::CleanUp(){
}

const char *detCurve2D::GetTestName(){ return "Curve2D"; }



// Tests
//////////

void detCurve2D::TestInitial(){
	SetSubTestNum( 0 );
	
	// initial condition
	decCurve2D curve1;
	
	ASSERT_TRUE( curve1.GetPointCount() == 0 );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 0 ) );
}

void detCurve2D::TestAdd(){
	decVector2 point1( 0.0f, 0.0f );
	decVector2 point2( 1.0f, 1.0f );
	decVector2 point3( 0.5f, 0.8f );
	decVector2 point4( 0.2f, 0.1f );
	decVector2 point5( 0.2001f, 0.11f );
	decVector2 point6( 0.1999f, 0.09f );
	decVector2 point7( 0.2f, 0.15f );
	decCurve2D curve1;
	
	SetSubTestNum( 1 );
	
	// adding point: { (0,0) } => {1}
	ASSERT_TRUE( curve1.AddPoint( point1 ) == 0 );
	ASSERT_TRUE( curve1.GetPointCount() == 1 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 1 ) );
	
	// adding point: { (0,0), (1,1) } => {1,2}
	ASSERT_TRUE( curve1.AddPoint( point2 ) == 1 );
	ASSERT_TRUE( curve1.GetPointCount() == 2 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve1.GetPointAt( 1 ).IsEqualTo( point2 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 2 ) );
	
	// adding point: { (0,0), (0.5,0.8), (1,1) } => {1,3,2}
	ASSERT_TRUE( curve1.AddPoint( point3 ) == 1 );
	ASSERT_TRUE( curve1.GetPointCount() == 3 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve1.GetPointAt( 1 ).IsEqualTo( point3 ) );
	ASSERT_TRUE( curve1.GetPointAt( 2 ).IsEqualTo( point2 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 3 ) );
	
	// adding point: { (0,0), (0.2, 0.1), (0.5,0.8), (1,1) } => {1,4,3,2}
	ASSERT_TRUE( curve1.AddPoint( point4 ) == 1 );
	ASSERT_TRUE( curve1.GetPointCount() == 4 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve1.GetPointAt( 1 ).IsEqualTo( point4 ) );
	ASSERT_TRUE( curve1.GetPointAt( 2 ).IsEqualTo( point3 ) );
	ASSERT_TRUE( curve1.GetPointAt( 3 ).IsEqualTo( point2 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 4 ) );
	
	// adding point: { (0,0), (0.2, 0.1), ( 0.2001, 0.11 ), (0.5,0.8), (1,1) } => {1,4,5,3,2}
	ASSERT_TRUE( curve1.AddPoint( point5 ) == 2 );
	ASSERT_TRUE( curve1.GetPointCount() == 5 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve1.GetPointAt( 1 ).IsEqualTo( point4 ) );
	ASSERT_TRUE( curve1.GetPointAt( 2 ).IsEqualTo( point5 ) );
	ASSERT_TRUE( curve1.GetPointAt( 3 ).IsEqualTo( point3 ) );
	ASSERT_TRUE( curve1.GetPointAt( 4 ).IsEqualTo( point2 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 5 ) );
	
	// adding point: { (0,0), ( 0.1999, 0.09 ), (0.2, 0.1), ( 0.2001, 0.11 ), (0.5,0.8), (1,1) } => {1,6,4,5,3,2}
	ASSERT_TRUE( curve1.AddPoint( point6 ) == 1 );
	ASSERT_TRUE( curve1.GetPointCount() == 6 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve1.GetPointAt( 1 ).IsEqualTo( point6 ) );
	ASSERT_TRUE( curve1.GetPointAt( 2 ).IsEqualTo( point4 ) );
	ASSERT_TRUE( curve1.GetPointAt( 3 ).IsEqualTo( point5 ) );
	ASSERT_TRUE( curve1.GetPointAt( 4 ).IsEqualTo( point3 ) );
	ASSERT_TRUE( curve1.GetPointAt( 5 ).IsEqualTo( point2 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 6 ) );
	
	// adding point: { (0,0), ( 0.1999, 0.09 ), (0.2, 0.15), ( 0.2001, 0.11 ), (0.5,0.8), (1,1) } => {1,6,7,5,3,2}
	ASSERT_TRUE( curve1.AddPoint( point7 ) == 2 );
	ASSERT_TRUE( curve1.GetPointCount() == 6 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve1.GetPointAt( 1 ).IsEqualTo( point6 ) );
	ASSERT_TRUE( curve1.GetPointAt( 2 ).IsEqualTo( point7 ) );
	ASSERT_TRUE( curve1.GetPointAt( 3 ).IsEqualTo( point5 ) );
	ASSERT_TRUE( curve1.GetPointAt( 4 ).IsEqualTo( point3 ) );
	ASSERT_TRUE( curve1.GetPointAt( 5 ).IsEqualTo( point2 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 6 ) );
}

void detCurve2D::TestClosest(){
	decVector2 point1( 0.0f, 0.0f );
	decVector2 point2( 0.1999f, 0.09f );
	decVector2 point3( 0.2f, 0.15f );
	decVector2 point4( 0.2001f, 0.11f );
	decVector2 point5( 0.5f, 0.8f );
	decVector2 point6( 1.0f, 1.0f );
	
	SetSubTestNum( 2 );
	
	// build test curve
	decCurve2D curve1;
	
	ASSERT_TRUE( curve1.AddPoint( point1 ) == 0 );
	ASSERT_TRUE( curve1.AddPoint( point2 ) == 1 );
	ASSERT_TRUE( curve1.AddPoint( point3 ) == 2 );
	ASSERT_TRUE( curve1.AddPoint( point4 ) == 3 );
	ASSERT_TRUE( curve1.AddPoint( point5 ) == 4 );
	ASSERT_TRUE( curve1.AddPoint( point6 ) == 5 );
	
	ASSERT_TRUE( curve1.GetPointCount() == 6 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve1.GetPointAt( 1 ).IsEqualTo( point2 ) );
	ASSERT_TRUE( curve1.GetPointAt( 2 ).IsEqualTo( point3 ) );
	ASSERT_TRUE( curve1.GetPointAt( 3 ).IsEqualTo( point4 ) );
	ASSERT_TRUE( curve1.GetPointAt( 4 ).IsEqualTo( point5 ) );
	ASSERT_TRUE( curve1.GetPointAt( 5 ).IsEqualTo( point6 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 6 ) );
	
	// test closest
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point1.x, threshold ) == 0 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point1.x + epsilon1, threshold ) == 0 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point1.x - epsilon1, threshold ) == 0 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point1.x + epsilon2, threshold ) == -1 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point1.x - epsilon2, threshold ) == -1 );
	
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point2.x, threshold ) == 1 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point2.x + epsilon1, threshold ) == 1 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point2.x - epsilon1, threshold ) == 1 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point2.x + epsilon2, threshold ) == -1 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point2.x - epsilon2, threshold ) == -1 );
	
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point3.x, threshold ) == 2 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point3.x + epsilon1, threshold ) == 2 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point3.x - epsilon1, threshold ) == 2 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point3.x + epsilon2, threshold ) == -1 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point3.x - epsilon2, threshold ) == -1 );
	
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point4.x, threshold ) == 3 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point4.x + epsilon1, threshold ) == 3 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point4.x - epsilon1, threshold ) == 3 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point4.x + epsilon2, threshold ) == -1 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point4.x - epsilon2, threshold ) == -1 );
	
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point5.x, threshold ) == 4 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point5.x + epsilon1, threshold ) == 4 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point5.x - epsilon1, threshold ) == 4 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point5.x + epsilon2, threshold ) == -1 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point5.x - epsilon2, threshold ) == -1 );
	
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point6.x, threshold ) == 5 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point6.x + epsilon1, threshold ) == 5 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point6.x - epsilon1, threshold ) == 5 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point6.x + epsilon2, threshold ) == -1 );
	ASSERT_TRUE( curve1.IndexOfPointClosestTo( point6.x - epsilon2, threshold ) == -1 );
}

void detCurve2D::TestBefore(){
	decVector2 point1( 0.0f, 0.0f );
	decVector2 point2( 0.1999f, 0.09f );
	decVector2 point3( 0.2f, 0.15f );
	decVector2 point4( 0.2001f, 0.11f );
	decVector2 point5( 0.5f, 0.8f );
	decVector2 point6( 1.0f, 1.0f );
	
	SetSubTestNum( 3 );
	
	// build test curve
	decCurve2D curve1;
	
	ASSERT_TRUE( curve1.AddPoint( point1 ) == 0 );
	ASSERT_TRUE( curve1.AddPoint( point2 ) == 1 );
	ASSERT_TRUE( curve1.AddPoint( point3 ) == 2 );
	ASSERT_TRUE( curve1.AddPoint( point4 ) == 3 );
	ASSERT_TRUE( curve1.AddPoint( point5 ) == 4 );
	ASSERT_TRUE( curve1.AddPoint( point6 ) == 5 );
	
	ASSERT_TRUE( curve1.GetPointCount() == 6 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve1.GetPointAt( 1 ).IsEqualTo( point2 ) );
	ASSERT_TRUE( curve1.GetPointAt( 2 ).IsEqualTo( point3 ) );
	ASSERT_TRUE( curve1.GetPointAt( 3 ).IsEqualTo( point4 ) );
	ASSERT_TRUE( curve1.GetPointAt( 4 ).IsEqualTo( point5 ) );
	ASSERT_TRUE( curve1.GetPointAt( 5 ).IsEqualTo( point6 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 6 ) );
	
	// test before
	ASSERT_TRUE( curve1.IndexOfPointBefore( point1.x - epsilon2 ) == -1 );
	ASSERT_TRUE( curve1.IndexOfPointBefore( point1.x - epsilon1 ) == -1 );
	ASSERT_TRUE( curve1.IndexOfPointBefore( point1.x + epsilon1 ) == 0 );
	ASSERT_TRUE( curve1.IndexOfPointBefore( point1.x + epsilon2 ) == 0 );
	
	ASSERT_TRUE( curve1.IndexOfPointBefore( point2.x - epsilon2 ) == 0 );
	ASSERT_TRUE( curve1.IndexOfPointBefore( point2.x - epsilon1 ) == 0 );
	ASSERT_TRUE( curve1.IndexOfPointBefore( point2.x + epsilon1 ) == 1 );
	ASSERT_TRUE( curve1.IndexOfPointBefore( point2.x + epsilon2 ) == 1 );
	
	ASSERT_TRUE( curve1.IndexOfPointBefore( point3.x - epsilon2 ) == 1 );
	ASSERT_TRUE( curve1.IndexOfPointBefore( point3.x - epsilon1 ) == 1 );
	ASSERT_TRUE( curve1.IndexOfPointBefore( point3.x + epsilon1 ) == 2 );
	ASSERT_TRUE( curve1.IndexOfPointBefore( point3.x + epsilon2 ) == 2 );
	
	ASSERT_TRUE( curve1.IndexOfPointBefore( point4.x - epsilon2 ) == 2 );
	ASSERT_TRUE( curve1.IndexOfPointBefore( point4.x - epsilon1 ) == 2 );
	ASSERT_TRUE( curve1.IndexOfPointBefore( point4.x + epsilon1 ) == 3 );
	ASSERT_TRUE( curve1.IndexOfPointBefore( point4.x + epsilon2 ) == 3 );
	
	ASSERT_TRUE( curve1.IndexOfPointBefore( point5.x - epsilon2 ) == 3 );
	ASSERT_TRUE( curve1.IndexOfPointBefore( point5.x - epsilon1 ) == 3 );
	ASSERT_TRUE( curve1.IndexOfPointBefore( point5.x + epsilon1 ) == 4 );
	ASSERT_TRUE( curve1.IndexOfPointBefore( point5.x + epsilon2 ) == 4 );
	
	ASSERT_TRUE( curve1.IndexOfPointBefore( point6.x - epsilon2 ) == 4 );
	ASSERT_TRUE( curve1.IndexOfPointBefore( point6.x - epsilon1 ) == 4 );
	ASSERT_TRUE( curve1.IndexOfPointBefore( point6.x + epsilon1 ) == 5 );
	ASSERT_TRUE( curve1.IndexOfPointBefore( point6.x + epsilon2 ) == 5 );
	
	ASSERT_TRUE( curve1.IndexOfPointBefore( 5.0f ) == 5 );
}

void detCurve2D::TestRemove(){
	decVector2 point1( 0.0f, 0.0f );
	decVector2 point2( 0.2f, 0.1f );
	decVector2 point3( 0.5f, 0.8f );
	decVector2 point4( 1.0f, 1.0f );
	
	SetSubTestNum( 4 );
	
	// build test curve
	decCurve2D curve1;
	
	ASSERT_TRUE( curve1.AddPoint( point1 ) == 0 );
	ASSERT_TRUE( curve1.AddPoint( point2 ) == 1 );
	ASSERT_TRUE( curve1.AddPoint( point3 ) == 2 );
	ASSERT_TRUE( curve1.AddPoint( point4 ) == 3 );
	
	ASSERT_TRUE( curve1.GetPointCount() == 4 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve1.GetPointAt( 1 ).IsEqualTo( point2 ) );
	ASSERT_TRUE( curve1.GetPointAt( 2 ).IsEqualTo( point3 ) );
	ASSERT_TRUE( curve1.GetPointAt( 3 ).IsEqualTo( point4 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 4 ) );
	
	// remove point 2
	curve1.RemovePointFrom( 1 );
	
	ASSERT_TRUE( curve1.GetPointCount() == 3 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve1.GetPointAt( 1 ).IsEqualTo( point3 ) );
	ASSERT_TRUE( curve1.GetPointAt( 2 ).IsEqualTo( point4 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 3 ) );
	
	// remove point 4
	curve1.RemovePointFrom( 2 );
	
	ASSERT_TRUE( curve1.GetPointCount() == 2 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve1.GetPointAt( 1 ).IsEqualTo( point3 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 2 ) );
	
	// remove point 1
	curve1.RemovePointFrom( 0 );
	
	ASSERT_TRUE( curve1.GetPointCount() == 1 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point3 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 1 ) );
	
	// remove point 3
	curve1.RemovePointFrom( 0 );
	
	ASSERT_TRUE( curve1.GetPointCount() == 0 );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 0 ) );
}

void detCurve2D::TestRemoveAll(){
	decVector2 point1( 0.0f, 0.0f );
	decVector2 point2( 0.2f, 0.1f );
	decVector2 point3( 0.5f, 0.8f );
	decVector2 point4( 1.0f, 1.0f );
	
	SetSubTestNum( 3 );
	
	// build test curve
	decCurve2D curve1;
	
	ASSERT_TRUE( curve1.AddPoint( point1 ) == 0 );
	ASSERT_TRUE( curve1.AddPoint( point2 ) == 1 );
	ASSERT_TRUE( curve1.AddPoint( point3 ) == 2 );
	ASSERT_TRUE( curve1.AddPoint( point4 ) == 3 );
	
	ASSERT_TRUE( curve1.GetPointCount() == 4 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve1.GetPointAt( 1 ).IsEqualTo( point2 ) );
	ASSERT_TRUE( curve1.GetPointAt( 2 ).IsEqualTo( point3 ) );
	ASSERT_TRUE( curve1.GetPointAt( 3 ).IsEqualTo( point4 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 4 ) );
	
	// remove all points
	curve1.RemoveAllPoints();
	
	ASSERT_TRUE( curve1.GetPointCount() == 0 );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 0 ) );
}

void detCurve2D::TestDefault(){
	decVector2 point1( 0.0f, 0.0f );
	decVector2 point2( 1.0f, 1.0f );
	
	SetSubTestNum( 5 );
	
	// build test curve
	decCurve2D curve1;
	
	curve1.SetDefaultCurve();
	
	ASSERT_TRUE( curve1.GetPointCount() == 2 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve1.GetPointAt( 1 ).IsEqualTo( point2 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 2 ) );
}

void detCurve2D::TestCopy(){
	decVector2 point1( 0.0f, 0.0f );
	decVector2 point2( 0.2f, 0.1f );
	decVector2 point3( 0.5f, 0.8f );
	decVector2 point4( 1.0f, 1.0f );
	
	SetSubTestNum( 6 );
	
	// build test curve
	decCurve2D curve1;
	
	ASSERT_TRUE( curve1.AddPoint( point1 ) == 0 );
	ASSERT_TRUE( curve1.AddPoint( point2 ) == 1 );
	ASSERT_TRUE( curve1.AddPoint( point3 ) == 2 );
	ASSERT_TRUE( curve1.AddPoint( point4 ) == 3 );
	
	ASSERT_TRUE( curve1.GetPointCount() == 4 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve1.GetPointAt( 1 ).IsEqualTo( point2 ) );
	ASSERT_TRUE( curve1.GetPointAt( 2 ).IsEqualTo( point3 ) );
	ASSERT_TRUE( curve1.GetPointAt( 3 ).IsEqualTo( point4 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 4 ) );
	
	// copy using constructor
	decCurve2D curve2( curve1 );
	
	ASSERT_TRUE( curve2.GetPointCount() == 4 );
	ASSERT_TRUE( curve2.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve2.GetPointAt( 1 ).IsEqualTo( point2 ) );
	ASSERT_TRUE( curve2.GetPointAt( 2 ).IsEqualTo( point3 ) );
	ASSERT_TRUE( curve2.GetPointAt( 3 ).IsEqualTo( point4 ) );
	ASSERT_DOES_FAIL( curve2.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve2.GetPointAt( 4 ) );
	
	// copy using operator
	decCurve2D curve3;
	
	curve3.SetDefaultCurve();
	
	curve3 = curve1;
	
	ASSERT_TRUE( curve3.GetPointCount() == 4 );
	ASSERT_TRUE( curve3.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve3.GetPointAt( 1 ).IsEqualTo( point2 ) );
	ASSERT_TRUE( curve3.GetPointAt( 2 ).IsEqualTo( point3 ) );
	ASSERT_TRUE( curve3.GetPointAt( 3 ).IsEqualTo( point4 ) );
	ASSERT_DOES_FAIL( curve3.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve3.GetPointAt( 4 ) );
}

void detCurve2D::TestEvaluateConstant(){
	decVector2 point1( 0.0f, 0.0f );
	decVector2 point2( 0.2f, 0.1f );
	decVector2 point3( 0.5f, 0.8f );
	decVector2 point4( 1.0f, 1.0f );
	
	SetSubTestNum( 7 );
	
	// evaluate 0-point curve
	decCurve2D curve1;
	
	ASSERT_TRUE( curve1.GetPointCount() == 0 );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 0 ) );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( 0.0f ) ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( -epsilon2 ) ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( -epsilon1 ) ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( epsilon2 ) ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( epsilon1 ) ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( -1.0f ) ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( 1.0f ) ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( 2.0f ) ) < 1e-5f );
	
	// evaluate 1-point curve
	ASSERT_TRUE( curve1.AddPoint( point1 ) == 0 );
	
	ASSERT_TRUE( curve1.GetPointCount() == 1 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 1 ) );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point1.x - epsilon2 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point1.x - epsilon1 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point1.x + epsilon1 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point1.x + epsilon2 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( -1.0 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( 1.0f ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( 2.0f ) - point1.y ) < 1e-5f );
	
	// evaluate 2-point curve
	ASSERT_TRUE( curve1.AddPoint( point4 ) == 1 );
	
	ASSERT_TRUE( curve1.GetPointCount() == 2 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve1.GetPointAt( 1 ).IsEqualTo( point4 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 2 ) );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point1.x - epsilon2 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point1.x - epsilon1 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point1.x + epsilon1 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point1.x + epsilon2 ) - point1.y ) < 1e-5f );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point4.x - epsilon2 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point4.x - epsilon1 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point4.x + epsilon1 ) - point4.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point4.x + epsilon2 ) - point4.y ) < 1e-5f );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( -1.0 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( 2.0f ) - point4.y ) < 1e-5f );
	
	// evaluate 3-point curve
	ASSERT_TRUE( curve1.AddPoint( point2 ) == 1 );
	
	ASSERT_TRUE( curve1.GetPointCount() == 3 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve1.GetPointAt( 1 ).IsEqualTo( point2 ) );
	ASSERT_TRUE( curve1.GetPointAt( 2 ).IsEqualTo( point4 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 3 ) );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point1.x - epsilon2 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point1.x - epsilon1 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point1.x + epsilon1 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point1.x + epsilon2 ) - point1.y ) < 1e-5f );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point2.x - epsilon2 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point2.x - epsilon1 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point2.x + epsilon1 ) - point2.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point2.x + epsilon2 ) - point2.y ) < 1e-5f );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point4.x - epsilon2 ) - point2.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point4.x - epsilon1 ) - point2.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point4.x + epsilon1 ) - point4.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point4.x + epsilon2 ) - point4.y ) < 1e-5f );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( -1.0 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( 2.0f ) - point4.y ) < 1e-5f );
	
	// evaluate 4-point curve
	ASSERT_TRUE( curve1.AddPoint( point3 ) == 2 );
	
	ASSERT_TRUE( curve1.GetPointCount() == 4 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve1.GetPointAt( 1 ).IsEqualTo( point2 ) );
	ASSERT_TRUE( curve1.GetPointAt( 2 ).IsEqualTo( point3 ) );
	ASSERT_TRUE( curve1.GetPointAt( 3 ).IsEqualTo( point4 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 4 ) );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point1.x - epsilon2 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point1.x - epsilon1 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point1.x + epsilon1 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point1.x + epsilon2 ) - point1.y ) < 1e-5f );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point2.x - epsilon2 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point2.x - epsilon1 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point2.x + epsilon1 ) - point2.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point2.x + epsilon2 ) - point2.y ) < 1e-5f );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point3.x - epsilon2 ) - point2.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point3.x - epsilon1 ) - point2.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point3.x + epsilon1 ) - point3.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point3.x + epsilon2 ) - point3.y ) < 1e-5f );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point4.x - epsilon2 ) - point3.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point4.x - epsilon1 ) - point3.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point4.x + epsilon1 ) - point4.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( point4.x + epsilon2 ) - point4.y ) < 1e-5f );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( -1.0 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateConstant( 2.0f ) - point4.y ) < 1e-5f );
}

void detCurve2D::TestEvaluateLinear(){
	decVector2 point1( 0.0f, 0.0f );
	decVector2 point2( 0.2f, 0.1f );
	decVector2 point3( 0.5f, 0.8f );
	decVector2 point4( 1.0f, 1.0f );
	
	decVector2 point5( 0.2f, 0.2f );
	decVector2 point6( 0.5f, 0.5f );
	decVector2 point7( 0.9f, 0.9f );
	
	decVector2 point8( 0.05f, 0.025f );
	decVector2 point9( 0.15f, 0.075f );
	
	decVector2 point10( 0.4f, 0.325f );
	decVector2 point11( 0.8f, 0.775f );
	
	decVector2 point12( 0.3f, 0.3333333f );
	decVector2 point13( 0.4f, 0.5666667f );
	
	decVector2 point14( 0.6f, 0.84f );
	decVector2 point15( 0.9f, 0.96f );
	
	SetSubTestNum( 8 );
	
	// evaluate 0-point curve
	decCurve2D curve1;
	
	ASSERT_TRUE( curve1.GetPointCount() == 0 );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 0 ) );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( 0.0f ) ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( -epsilon2 ) ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( -epsilon1 ) ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( epsilon2 ) ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( epsilon1 ) ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( -1.0f ) ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( 1.0f ) ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( 2.0f ) ) < 1e-5f );
	
	// evaluate 1-point curve
	ASSERT_TRUE( curve1.AddPoint( point1 ) == 0 );
	
	ASSERT_TRUE( curve1.GetPointCount() == 1 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 1 ) );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point1.x - epsilon2 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point1.x - epsilon1 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point1.x + epsilon1 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point1.x + epsilon2 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( -1.0 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( 1.0f ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( 2.0f ) - point1.y ) < 1e-5f );
	
	// evaluate 2-point curve
	ASSERT_TRUE( curve1.AddPoint( point4 ) == 1 );
	
	ASSERT_TRUE( curve1.GetPointCount() == 2 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve1.GetPointAt( 1 ).IsEqualTo( point4 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 2 ) );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point1.x - epsilon2 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point1.x - epsilon1 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point1.x ) - point1.y ) < 1e-5f );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point4.x ) - point4.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point5.x ) - point5.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point6.x ) - point6.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point7.x ) - point7.y ) < 1e-5f );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( -1.0 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( 2.0f ) - point4.y ) < 1e-5f );
	
	// evaluate 3-point curve
	ASSERT_TRUE( curve1.AddPoint( point2 ) == 1 );
	
	ASSERT_TRUE( curve1.GetPointCount() == 3 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve1.GetPointAt( 1 ).IsEqualTo( point2 ) );
	ASSERT_TRUE( curve1.GetPointAt( 2 ).IsEqualTo( point4 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 3 ) );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point1.x - epsilon2 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point1.x - epsilon1 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point1.x ) - point1.y ) < 1e-5f );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point2.x ) - point2.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point4.x ) - point4.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point8.x ) - point8.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point9.x ) - point9.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point10.x ) - point10.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point11.x ) - point11.y ) < 1e-5f );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( -1.0 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( 2.0f ) - point4.y ) < 1e-5f );
	
	// evaluate 4-point curve
	ASSERT_TRUE( curve1.AddPoint( point3 ) == 2 );
	
	ASSERT_TRUE( curve1.GetPointCount() == 4 );
	ASSERT_TRUE( curve1.GetPointAt( 0 ).IsEqualTo( point1 ) );
	ASSERT_TRUE( curve1.GetPointAt( 1 ).IsEqualTo( point2 ) );
	ASSERT_TRUE( curve1.GetPointAt( 2 ).IsEqualTo( point3 ) );
	ASSERT_TRUE( curve1.GetPointAt( 3 ).IsEqualTo( point4 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( -1 ) );
	ASSERT_DOES_FAIL( curve1.GetPointAt( 4 ) );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point1.x - epsilon2 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point1.x - epsilon1 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point1.x ) - point1.y ) < 1e-5f );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point2.x ) - point2.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point3.x ) - point3.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point4.x ) - point4.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point8.x ) - point8.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point9.x ) - point9.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point12.x ) - point12.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point13.x ) - point13.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point14.x ) - point14.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( point15.x ) - point15.y ) < 1e-5f );
	
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( -1.0 ) - point1.y ) < 1e-5f );
	ASSERT_TRUE( fabsf( curve1.EvaluateLinear( 2.0f ) - point4.y ) < 1e-5f );
}
