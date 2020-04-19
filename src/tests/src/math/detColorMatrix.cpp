// includes
#include <stdio.h>
#include <stdlib.h>
#include "detColorMatrix.h"
#include "dragengine/common/math/decMath.h"
#include "dragengine/common/exceptions.h"

	

// Class detColorMatrix
/////////////////////////

// Constructors, destructor
/////////////////////////////

detColorMatrix::detColorMatrix(){
}

detColorMatrix::~detColorMatrix(){
}



// Testing
////////////

void detColorMatrix::Prepare(){
}

void detColorMatrix::Run(){
	pTestInitial();
	pTestCreationTranslation();
	pTestCreationScaling();
	pTestCreationFrom();
	pTestAddition();
	pTestMultiply();
	pTestTransforming();
	pTestConcatenation();
}

void detColorMatrix::CleanUp(){
}

const char *detColorMatrix::GetTestName(){
	return "ColorMatrix";
}



// Private Functions
//////////////////////

void detColorMatrix::pRandomInitMatrix( decColorMatrix &matrix ){
	matrix.a11 = 10.0f;
	matrix.a12 = 10.1f;
	matrix.a13 = 10.2f;
	matrix.a14 = 10.3f;
	matrix.a15 = 10.4f;
	
	matrix.a21 = 11.0f;
	matrix.a22 = 11.1f;
	matrix.a23 = 11.2f;
	matrix.a24 = 11.3f;
	matrix.a25 = 11.4f;
	
	matrix.a31 = 12.0f;
	matrix.a32 = 12.1f;
	matrix.a33 = 12.2f;
	matrix.a34 = 12.3f;
	matrix.a35 = 12.4f;
	
	matrix.a41 = 13.0f;
	matrix.a42 = 13.1f;
	matrix.a43 = 13.2f;
	matrix.a44 = 13.3f;
	matrix.a45 = 13.4f;
}

void detColorMatrix::pTestInitial(){
	SetSubTestNum( 0 );
	decColorMatrix matrix;
	
	ASSERT_TRUE( fabs( matrix.a11 - 1.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a12 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a13 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a14 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a15 - 0.0f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a21 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a22 - 1.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a23 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a24 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a25 - 0.0f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a31 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a32 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a33 - 1.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a34 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a35 - 0.0f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a41 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a42 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a43 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a44 - 1.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a45 - 0.0f ) < 1e-5 );
}

void detColorMatrix::pTestCreationTranslation(){
	SetSubTestNum( 1 );
	decColorMatrix matrix;
	
	// set matrix to translation using individual values
	pRandomInitMatrix( matrix );
	matrix.SetTranslation( 0.1f, 0.2, 0.3f, 0.4f );
	
	ASSERT_TRUE( fabs( matrix.a11 - 1.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a12 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a13 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a14 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a15 - 0.1f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a21 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a22 - 1.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a23 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a24 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a25 - 0.2f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a31 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a32 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a33 - 1.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a34 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a35 - 0.3f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a41 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a42 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a43 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a44 - 1.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a45 - 0.4f ) < 1e-5 );
	
	// set matrix to translation using a color
	pRandomInitMatrix( matrix );
	matrix.SetTranslation( decColor( 0.1f, 0.2, 0.3f, 0.4f ) );
	
	ASSERT_TRUE( fabs( matrix.a11 - 1.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a12 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a13 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a14 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a15 - 0.1f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a21 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a22 - 1.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a23 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a24 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a25 - 0.2f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a31 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a32 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a33 - 1.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a34 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a35 - 0.3f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a41 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a42 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a43 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a44 - 1.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a45 - 0.4f ) < 1e-5 );
	
	// create translation matrix using individual values
	pRandomInitMatrix( matrix );
	matrix = decColorMatrix::CreateTranslation( 0.1f, 0.2, 0.3f, 0.4f );
	
	ASSERT_TRUE( fabs( matrix.a11 - 1.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a12 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a13 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a14 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a15 - 0.1f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a21 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a22 - 1.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a23 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a24 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a25 - 0.2f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a31 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a32 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a33 - 1.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a34 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a35 - 0.3f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a41 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a42 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a43 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a44 - 1.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a45 - 0.4f ) < 1e-5 );
	
	// create translation matrix using a color
	pRandomInitMatrix( matrix );
	matrix = decColorMatrix::CreateTranslation( decColor( 0.1f, 0.2, 0.3f, 0.4f ) );
	
	ASSERT_TRUE( fabs( matrix.a11 - 1.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a12 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a13 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a14 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a15 - 0.1f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a21 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a22 - 1.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a23 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a24 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a25 - 0.2f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a31 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a32 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a33 - 1.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a34 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a35 - 0.3f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a41 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a42 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a43 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a44 - 1.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a45 - 0.4f ) < 1e-5 );
}

void detColorMatrix::pTestCreationScaling(){
	SetSubTestNum( 2 );
	decColorMatrix matrix;
	
	// set matrix to scaling using individual values
	pRandomInitMatrix( matrix );
	matrix.SetScaling( 0.1f, 0.2, 0.3f, 0.4f );
	
	ASSERT_TRUE( fabs( matrix.a11 - 0.1f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a12 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a13 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a14 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a15 - 0.0f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a21 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a22 - 0.2f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a23 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a24 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a25 - 0.0f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a31 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a32 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a33 - 0.3f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a34 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a35 - 0.0f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a41 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a42 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a43 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a44 - 0.4f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a45 - 0.0f ) < 1e-5 );
	
	// set matrix to scaling using a color
	pRandomInitMatrix( matrix );
	matrix.SetScaling( decColor( 0.1f, 0.2, 0.3f, 0.4f ) );
	
	ASSERT_TRUE( fabs( matrix.a11 - 0.1f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a12 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a13 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a14 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a15 - 0.0f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a21 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a22 - 0.2f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a23 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a24 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a25 - 0.0f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a31 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a32 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a33 - 0.3f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a34 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a35 - 0.0f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a41 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a42 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a43 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a44 - 0.4f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a45 - 0.0f ) < 1e-5 );
	
	// create scaling matrix using individual values
	pRandomInitMatrix( matrix );
	matrix = decColorMatrix::CreateScaling( 0.1f, 0.2, 0.3f, 0.4f );
	
	ASSERT_TRUE( fabs( matrix.a11 - 0.1f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a12 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a13 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a14 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a15 - 0.0f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a21 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a22 - 0.2f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a23 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a24 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a25 - 0.0f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a31 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a32 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a33 - 0.3f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a34 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a35 - 0.0f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a41 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a42 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a43 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a44 - 0.4f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a45 - 0.0f ) < 1e-5 );
	
	// create scaling matrix using a color
	pRandomInitMatrix( matrix );
	matrix = decColorMatrix::CreateScaling( decColor( 0.1f, 0.2, 0.3f, 0.4f ) );
	
	ASSERT_TRUE( fabs( matrix.a11 - 0.1f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a12 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a13 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a14 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a15 - 0.0f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a21 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a22 - 0.2f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a23 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a24 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a25 - 0.0f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a31 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a32 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a33 - 0.3f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a34 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a35 - 0.0f ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a41 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a42 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a43 - 0.0f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a44 - 0.4f ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a45 - 0.0f ) < 1e-5 );
}

void detColorMatrix::pTestCreationFrom(){
	SetSubTestNum( 3 );
	decColor red( 0.10f, 0.12f, 0.14f, 0.16f );
	decColor green( 0.20f, 0.22f, 0.24f, 0.26f );
	decColor blue( 0.30f, 0.32f, 0.34f, 0.36f );
	decColor alpha( 0.40f, 0.42f, 0.44f, 0.46f );
	decColor translation( 0.50f, 0.52f, 0.54f, 0.56f );
	decColorMatrix matrix;
	
	// set matrix from colors
	pRandomInitMatrix( matrix );
	matrix.SetFrom( red, green, blue, alpha, translation );
	
	ASSERT_TRUE( fabs( matrix.a11 - red.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a12 - green.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a13 - blue.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a14 - alpha.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a15 - translation.r ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a21 - red.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a22 - green.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a23 - blue.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a24 - alpha.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a25 - translation.g ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a31 - red.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a32 - green.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a33 - blue.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a34 - alpha.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a35 - translation.b ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a41 - red.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a42 - green.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a43 - blue.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a44 - alpha.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a45 - translation.a ) < 1e-5 );
	
	// create matrix from colors
	pRandomInitMatrix( matrix );
	matrix = decColorMatrix::CreateFrom( red, green, blue, alpha, translation );
	
	ASSERT_TRUE( fabs( matrix.a11 - red.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a12 - green.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a13 - blue.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a14 - alpha.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a15 - translation.r ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a21 - red.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a22 - green.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a23 - blue.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a24 - alpha.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a25 - translation.g ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a31 - red.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a32 - green.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a33 - blue.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a34 - alpha.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a35 - translation.b ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix.a41 - red.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a42 - green.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a43 - blue.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a44 - alpha.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix.a45 - translation.a ) < 1e-5 );
}

void detColorMatrix::pTestAddition(){
	SetSubTestNum( 4 );
	
	decColor red1( 0.10f, 0.12f, 0.14f, 0.16f );
	decColor green1( 0.20f, 0.22f, 0.24f, 0.26f );
	decColor blue1( 0.30f, 0.32f, 0.34f, 0.36f );
	decColor alpha1( 0.40f, 0.42f, 0.44f, 0.46f );
	decColor translation1( 0.50f, 0.52f, 0.54f, 0.56f );
	
	decColor red2 = red1 * 5.0f;
	decColor green2 = green1 * 5.0f;
	decColor blue2 = blue1 * 5.0f;
	decColor alpha2 = alpha1 * 5.0f;
	decColor translation2 = translation1 * 5.0f;
	
	decColor red3 = red1 + red2;
	decColor green3 = green1 + green2;
	decColor blue3 = blue1 + blue2;
	decColor alpha3 = alpha1 + alpha2;
	decColor translation3 = translation1 + translation2;
	
	decColor red4 = red1 - red2;
	decColor green4 = green1 - green2;
	decColor blue4 = blue1 - blue2;
	decColor alpha4 = alpha1 - alpha2;
	decColor translation4 = translation1 - translation2;
	
	decColorMatrix matrix1, matrix2, matrix3;
	
	// add matrix1 to matrix2 and store it in matrix3
	matrix1.SetFrom( red1, green1, blue1, alpha1, translation1 );
	matrix2.SetFrom( red2, green2, blue2, alpha2, translation2 );
	matrix3 = matrix1 + matrix2;
	
	ASSERT_TRUE( fabs( matrix3.a11 - red3.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a12 - green3.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a13 - blue3.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a14 - alpha3.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a15 - translation3.r ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix3.a21 - red3.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a22 - green3.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a23 - blue3.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a24 - alpha3.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a25 - translation3.g ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix3.a31 - red3.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a32 - green3.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a33 - blue3.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a34 - alpha3.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a35 - translation3.b ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix3.a41 - red3.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a42 - green3.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a43 - blue3.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a44 - alpha3.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a45 - translation3.a ) < 1e-5 );
	
	// subtract matrix2 from matrix1 and store it in matrix3
	matrix3 = matrix1 - matrix2;
	
	ASSERT_TRUE( fabs( matrix3.a11 - red4.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a12 - green4.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a13 - blue4.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a14 - alpha4.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a15 - translation4.r ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix3.a21 - red4.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a22 - green4.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a23 - blue4.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a24 - alpha4.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a25 - translation4.g ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix3.a31 - red4.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a32 - green4.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a33 - blue4.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a34 - alpha4.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a35 - translation4.b ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix3.a41 - red4.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a42 - green4.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a43 - blue4.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a44 - alpha4.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a45 - translation4.a ) < 1e-5 );
	
	// add matrix2 to matrix1 and store it in matrix1
	matrix3 = matrix1;
	matrix3 += matrix2;
	
	ASSERT_TRUE( fabs( matrix3.a11 - red3.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a12 - green3.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a13 - blue3.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a14 - alpha3.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a15 - translation3.r ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix3.a21 - red3.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a22 - green3.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a23 - blue3.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a24 - alpha3.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a25 - translation3.g ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix3.a31 - red3.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a32 - green3.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a33 - blue3.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a34 - alpha3.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a35 - translation3.b ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix3.a41 - red3.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a42 - green3.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a43 - blue3.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a44 - alpha3.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a45 - translation3.a ) < 1e-5 );
	
	// subtract matrix2 from matrix1 and store it in matrix3
	matrix3 = matrix1;
	matrix3 -= matrix2;
	
	ASSERT_TRUE( fabs( matrix3.a11 - red4.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a12 - green4.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a13 - blue4.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a14 - alpha4.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a15 - translation4.r ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix3.a21 - red4.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a22 - green4.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a23 - blue4.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a24 - alpha4.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a25 - translation4.g ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix3.a31 - red4.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a32 - green4.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a33 - blue4.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a34 - alpha4.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a35 - translation4.b ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix3.a41 - red4.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a42 - green4.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a43 - blue4.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a44 - alpha4.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix3.a45 - translation4.a ) < 1e-5 );
}

void detColorMatrix::pTestMultiply(){
	SetSubTestNum( 5 );
	
	decColor red1( 0.10f, 0.12f, 0.14f, 0.16f );
	decColor green1( 0.20f, 0.22f, 0.24f, 0.26f );
	decColor blue1( 0.30f, 0.32f, 0.34f, 0.36f );
	decColor alpha1( 0.40f, 0.42f, 0.44f, 0.46f );
	decColor translation1( 0.50f, 0.52f, 0.54f, 0.56f );
	float scalar = 0.2;
	
	decColor red2 = red1 * scalar;
	decColor green2 = green1 * scalar;
	decColor blue2 = blue1 * scalar;
	decColor alpha2 = alpha1 * scalar;
	decColor translation2 = translation1 * scalar;
	
	decColor red3 = red1 / scalar;
	decColor green3 = green1 / scalar;
	decColor blue3 = blue1 / scalar;
	decColor alpha3 = alpha1 / scalar;
	decColor translation3 = translation1 / scalar;
	
	decColorMatrix matrix1, matrix2;
	
	// multiply matrix with scalar
	matrix1.SetFrom( red1, green1, blue1, alpha1, translation1 );
	matrix2 = matrix1 * scalar;
	
	ASSERT_TRUE( fabs( matrix2.a11 - red2.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a12 - green2.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a13 - blue2.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a14 - alpha2.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a15 - translation2.r ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix2.a21 - red2.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a22 - green2.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a23 - blue2.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a24 - alpha2.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a25 - translation2.g ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix2.a31 - red2.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a32 - green2.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a33 - blue2.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a34 - alpha2.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a35 - translation2.b ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix2.a41 - red2.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a42 - green2.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a43 - blue2.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a44 - alpha2.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a45 - translation2.a ) < 1e-5 );
	
	// divide matrix by scalar
	matrix2 = matrix1 / scalar;
	
	ASSERT_TRUE( fabs( matrix2.a11 - red3.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a12 - green3.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a13 - blue3.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a14 - alpha3.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a15 - translation3.r ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix2.a21 - red3.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a22 - green3.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a23 - blue3.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a24 - alpha3.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a25 - translation3.g ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix2.a31 - red3.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a32 - green3.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a33 - blue3.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a34 - alpha3.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a35 - translation3.b ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix2.a41 - red3.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a42 - green3.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a43 - blue3.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a44 - alpha3.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a45 - translation3.a ) < 1e-5 );
	
	// multiply matrix with scalar
	matrix2 = matrix1;
	matrix2 *= scalar;
	
	ASSERT_TRUE( fabs( matrix2.a11 - red2.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a12 - green2.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a13 - blue2.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a14 - alpha2.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a15 - translation2.r ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix2.a21 - red2.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a22 - green2.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a23 - blue2.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a24 - alpha2.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a25 - translation2.g ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix2.a31 - red2.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a32 - green2.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a33 - blue2.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a34 - alpha2.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a35 - translation2.b ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix2.a41 - red2.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a42 - green2.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a43 - blue2.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a44 - alpha2.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a45 - translation2.a ) < 1e-5 );
	
	// divide matrix by scalar
	matrix2 = matrix1;
	matrix2 /= scalar;
	
	ASSERT_TRUE( fabs( matrix2.a11 - red3.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a12 - green3.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a13 - blue3.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a14 - alpha3.r ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a15 - translation3.r ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix2.a21 - red3.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a22 - green3.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a23 - blue3.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a24 - alpha3.g ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a25 - translation3.g ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix2.a31 - red3.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a32 - green3.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a33 - blue3.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a34 - alpha3.b ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a35 - translation3.b ) < 1e-5 );
	
	ASSERT_TRUE( fabs( matrix2.a41 - red3.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a42 - green3.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a43 - blue3.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a44 - alpha3.a ) < 1e-5 );
	ASSERT_TRUE( fabs( matrix2.a45 - translation3.a ) < 1e-5 );
}

void detColorMatrix::pTestTransforming(){
	SetSubTestNum( 6 );
	
	decColorMatrix matrix;
	decColor testColor( 0.5f, 0.6f, 0.7f, 0.8f );
	decColor translation( 0.1f, 0.2f, -0.3f, -0.4f );
	decColor scaling( 0.1f, 0.2f, 0.3f, 0.4f );
	
	decColor baseRed( 0.10f, 0.12f, 0.14f, 0.16f );
	decColor baseGreen( 0.20f, 0.22f, 0.24f, 0.26f );
	decColor baseBlue( 0.30f, 0.32f, 0.34f, 0.36f );
	decColor baseAlpha( 0.40f, 0.42f, 0.44f, 0.46f );
	decColor baseTranslation( 0.50f, 0.52f, 0.54f, 0.56f );
	
	decColor result, expectedResult;
	
	// translation color matrix
	matrix.SetTranslation( translation );
	result = matrix * testColor;
	ASSERT_TRUE( result.IsEqualTo( testColor + translation ) );
	
	// scaling color matrix
	matrix.SetScaling( scaling );
	result = matrix * testColor;
	ASSERT_TRUE( result.IsEqualTo( testColor * scaling ) );
	
	// base color matrix
	matrix.SetFrom( baseRed, baseGreen, baseBlue, baseAlpha, baseTranslation );
	result = matrix * testColor;
	expectedResult.r = testColor.r * baseRed.r + testColor.g * baseGreen.r
		+ testColor.b * baseBlue.r + testColor.a * baseAlpha.r + baseTranslation.r;
	expectedResult.g = testColor.r * baseRed.g + testColor.g * baseGreen.g
		+ testColor.b * baseBlue.g + testColor.a * baseAlpha.g + baseTranslation.g;
	expectedResult.b = testColor.r * baseRed.b + testColor.g * baseGreen.b
		+ testColor.b * baseBlue.b + testColor.a * baseAlpha.b + baseTranslation.b;
	expectedResult.a = testColor.r * baseRed.a + testColor.g * baseGreen.a
		+ testColor.b * baseBlue.a + testColor.a * baseAlpha.a + baseTranslation.a;
	ASSERT_TRUE( result.IsEqualTo( expectedResult ) );
}

void detColorMatrix::pTestConcatenation(){
	SetSubTestNum( 7 );
	
	decColor red1( 0.10f, 0.12f, 0.14f, 0.16f );
	decColor green1( 0.20f, 0.22f, 0.24f, 0.26f );
	decColor blue1( 0.30f, 0.32f, 0.34f, 0.36f );
	decColor alpha1( 0.40f, 0.42f, 0.44f, 0.46f );
	decColor translation1( 0.50f, 0.52f, 0.54f, 0.56f );
	
	decColor red2 = red1 * 5.0f;
	decColor green2 = green1 * 5.0f;
	decColor blue2 = blue1 * 5.0f;
	decColor alpha2 = alpha1 * 5.0f;
	decColor translation2 = translation1 * 5.0f;
	
	decColor testColor( 0.5f, 0.6f, 0.7f, 0.8f );
	decColor result, expectedResult;
	
	decColorMatrix matrix1, matrix2, matrix3;
	
	// concatenate two base matrices
	matrix1.SetFrom( red1, green1, blue1, alpha1, translation1 );
	matrix2.SetFrom( red2, green2, blue2, alpha2, translation2 );
	matrix3 = matrix1 * matrix2;
	
	result = matrix3 * testColor;
	expectedResult = matrix2 * ( matrix1 * testColor );
	ASSERT_TRUE( result.IsEqualTo( expectedResult ) );
	
	// concatenate two base matrices
	matrix3 = matrix1;
	matrix3 *= matrix2;
	
	result = matrix3 * testColor;
	expectedResult = matrix2 * ( matrix1 * testColor );
	ASSERT_TRUE( result.IsEqualTo( expectedResult ) );
}
