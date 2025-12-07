// includes
#include <stdio.h>
#include <stdlib.h>

#include "detTexMatrix2.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/exceptions.h>

	

// Class detTexMatrix2
////////////////////////

// Constructors, destructor
/////////////////////////////

detTexMatrix2::detTexMatrix2(){
}

detTexMatrix2::~detTexMatrix2(){
}



// Testing
////////////

void detTexMatrix2::Prepare(){
}

void detTexMatrix2::Run(){
	pTestInitial();
	pTestCreation();
	pTestSet();
	pTestOperators();
	pTestInvert();
}

void detTexMatrix2::CleanUp(){
}

const char *detTexMatrix2::GetTestName(){
	return "TexMatrix2";
}



// Private Functions
//////////////////////

void detTexMatrix2::pInitMatrix(decTexMatrix2 &matrix, float base){
	matrix.a11 = base + 0.0f;
	matrix.a12 = base + 0.1f;
	matrix.a13 = base + 0.2f;
	
	matrix.a21 = base + 1.0f;
	matrix.a22 = base + 1.1f;
	matrix.a23 = base + 1.2f;
}

void detTexMatrix2::pTestInitial(){
	SetSubTestNum(0);
	decTexMatrix2 matrix;
	
	ASSERT_FEQUAL(matrix.a11, 1.0f);
	ASSERT_FEQUAL(matrix.a12, 0.0f);
	ASSERT_FEQUAL(matrix.a13, 0.0f);
	
	ASSERT_FEQUAL(matrix.a21, 0.0f);
	ASSERT_FEQUAL(matrix.a22, 1.0f);
	ASSERT_FEQUAL(matrix.a23, 0.0f);
}

void detTexMatrix2::pTestCreation(){
	SetSubTestNum(1);
	decTexMatrix2 matrix, groundTruth;
	const float v1 = 0.1f;
	const float v2 = 0.2f;
	const float v3 = 0.3f;
	const float v4 = 0.4f;
	const float r1 = DEG2RAD * 10.0f;
	const float r1c = cosf(r1);
	const float r1s = sinf(r1);
	
	// test translation
	pInitMatrix(matrix, 10.0f);
	matrix = decTexMatrix2::CreateTranslation(v1, v2);
	
	ASSERT_FEQUAL(matrix.a11, 1.0f);
	ASSERT_FEQUAL(matrix.a12, 0.0f);
	ASSERT_FEQUAL(matrix.a13, v1);
	
	ASSERT_FEQUAL(matrix.a21, 0.0f);
	ASSERT_FEQUAL(matrix.a22, 1.0f);
	ASSERT_FEQUAL(matrix.a23, v2);
	
	pInitMatrix(matrix, 10.0f);
	matrix = decTexMatrix2::CreateTranslation(decVector2(v1, v2));
	
	ASSERT_FEQUAL(matrix.a11, 1.0f);
	ASSERT_FEQUAL(matrix.a12, 0.0f);
	ASSERT_FEQUAL(matrix.a13, v1);
	
	ASSERT_FEQUAL(matrix.a21, 0.0f);
	ASSERT_FEQUAL(matrix.a22, 1.0f);
	ASSERT_FEQUAL(matrix.a23, v2);
	
	// test rotation
	pInitMatrix(matrix, 10.0f);
	matrix = decTexMatrix2::CreateRotation(r1);
	
	ASSERT_FEQUAL(matrix.a11, r1c);
	ASSERT_FEQUAL(matrix.a12, -r1s);
	ASSERT_FEQUAL(matrix.a13, 0.0f);
	
	ASSERT_FEQUAL(matrix.a21, r1s);
	ASSERT_FEQUAL(matrix.a22, r1c);
	ASSERT_FEQUAL(matrix.a23, 0.0f);
	
	// test scaling
	pInitMatrix(matrix, 10.0f);
	matrix = decTexMatrix2::CreateScale(v1, v2);
	
	ASSERT_FEQUAL(matrix.a11, v1);
	ASSERT_FEQUAL(matrix.a12, 0.0f);
	ASSERT_FEQUAL(matrix.a13, 0.0f);
	
	ASSERT_FEQUAL(matrix.a21, 0.0f);
	ASSERT_FEQUAL(matrix.a22, v2);
	ASSERT_FEQUAL(matrix.a23, 0.0f);
	
	pInitMatrix(matrix, 10.0f);
	matrix = decTexMatrix2::CreateScale(decVector2(v1, v2));
	
	ASSERT_FEQUAL(matrix.a11, v1);
	ASSERT_FEQUAL(matrix.a12, 0.0f);
	ASSERT_FEQUAL(matrix.a13, 0.0f);
	
	ASSERT_FEQUAL(matrix.a21, 0.0f);
	ASSERT_FEQUAL(matrix.a22, v2);
	ASSERT_FEQUAL(matrix.a23, 0.0f);
	
	// test scale-translate
	pInitMatrix(matrix, 10.0f);
	matrix = decTexMatrix2::CreateST(v1, v2, v3, v4);
	groundTruth = decTexMatrix2::CreateScale(v1, v2)
		* decTexMatrix2::CreateTranslation( v3, v4 );
	ASSERT_TRUE(matrix.IsEqualTo(groundTruth));
	
	pInitMatrix(matrix, 10.0f);
	matrix = decTexMatrix2::CreateST(decVector2(v1, v2), decVector2(v3, v4));
	ASSERT_TRUE(matrix.IsEqualTo(groundTruth));
	
	// test scale-rotate-translate
	pInitMatrix(matrix, 10.0f);
	matrix = decTexMatrix2::CreateSRT(v1, v2, r1, v3, v4);
	groundTruth = decTexMatrix2::CreateScale(v1, v2)
		* decTexMatrix2::CreateRotation( r1 )
		* decTexMatrix2::CreateTranslation( v3, v4 );
	ASSERT_TRUE(matrix.IsEqualTo(groundTruth));
	
	pInitMatrix(matrix, 10.0f);
	matrix = decTexMatrix2::CreateSRT(decVector2(v1, v2), r1, decVector2(v3, v4));
	ASSERT_TRUE(matrix.IsEqualTo(groundTruth));
	
	// test center scale-rotate-translate
	pInitMatrix(matrix, 10.0f);
	matrix = decTexMatrix2::CreateCenterSRT(v1, v2, r1, v3, v4);
	groundTruth = 
		decTexMatrix2::CreateTranslation(-0.5f, -0.5f)
		* decTexMatrix2::CreateScale( v1, v2 )
		* decTexMatrix2::CreateRotation( r1 )
		* decTexMatrix2::CreateTranslation( v3, v4 )
		* decTexMatrix2::CreateTranslation( 0.5f, 0.5f );
	ASSERT_TRUE(matrix.IsEqualTo(groundTruth));
	
	pInitMatrix(matrix, 10.0f);
	matrix = decTexMatrix2::CreateCenterSRT(decVector2(v1, v2), r1, decVector2(v3, v4));
	ASSERT_TRUE(matrix.IsEqualTo(groundTruth));
}

void detTexMatrix2::pTestSet(){
	SetSubTestNum(1);
	decTexMatrix2 matrix, groundTruth;
	const float v1 = 0.1f;
	const float v2 = 0.2f;
	const float v3 = 0.3f;
	const float v4 = 0.4f;
	const float r1 = DEG2RAD * 10.0f;
	const float r1c = cosf(r1);
	const float r1s = sinf(r1);
	
	// test translation
	pInitMatrix(matrix, 10.0f);
	matrix.SetTranslation(v1, v2);
	
	ASSERT_FEQUAL(matrix.a11, 1.0f);
	ASSERT_FEQUAL(matrix.a12, 0.0f);
	ASSERT_FEQUAL(matrix.a13, v1);
	
	ASSERT_FEQUAL(matrix.a21, 0.0f);
	ASSERT_FEQUAL(matrix.a22, 1.0f);
	ASSERT_FEQUAL(matrix.a23, v2);
	
	pInitMatrix(matrix, 10.0f);
	matrix.SetTranslation(decVector2(v1, v2));
	
	ASSERT_FEQUAL(matrix.a11, 1.0f);
	ASSERT_FEQUAL(matrix.a12, 0.0f);
	ASSERT_FEQUAL(matrix.a13, v1);
	
	ASSERT_FEQUAL(matrix.a21, 0.0f);
	ASSERT_FEQUAL(matrix.a22, 1.0f);
	ASSERT_FEQUAL(matrix.a23, v2);
	
	// test rotation
	pInitMatrix(matrix, 10.0f);
	matrix.SetRotation(r1);
	
	ASSERT_FEQUAL(matrix.a11, r1c);
	ASSERT_FEQUAL(matrix.a12, -r1s);
	ASSERT_FEQUAL(matrix.a13, 0.0f);
	
	ASSERT_FEQUAL(matrix.a21, r1s);
	ASSERT_FEQUAL(matrix.a22, r1c);
	ASSERT_FEQUAL(matrix.a23, 0.0f);
	
	// test scaling
	pInitMatrix(matrix, 10.0f);
	matrix.SetScale(v1, v2);
	
	ASSERT_FEQUAL(matrix.a11, v1);
	ASSERT_FEQUAL(matrix.a12, 0.0f);
	ASSERT_FEQUAL(matrix.a13, 0.0f);
	
	ASSERT_FEQUAL(matrix.a21, 0.0f);
	ASSERT_FEQUAL(matrix.a22, v2);
	ASSERT_FEQUAL(matrix.a23, 0.0f);
	
	pInitMatrix(matrix, 10.0f);
	matrix.SetScale(decVector2(v1, v2));
	
	ASSERT_FEQUAL(matrix.a11, v1);
	ASSERT_FEQUAL(matrix.a12, 0.0f);
	ASSERT_FEQUAL(matrix.a13, 0.0f);
	
	ASSERT_FEQUAL(matrix.a21, 0.0f);
	ASSERT_FEQUAL(matrix.a22, v2);
	ASSERT_FEQUAL(matrix.a23, 0.0f);
	
	// test scale-translate
	pInitMatrix(matrix, 10.0f);
	matrix.SetST(v1, v2, v3, v4);
	groundTruth = decTexMatrix2::CreateScale(v1, v2)
		* decTexMatrix2::CreateTranslation( v3, v4 );
	ASSERT_TRUE(matrix.IsEqualTo(groundTruth));
	
	pInitMatrix(matrix, 10.0f);
	matrix.SetST(decVector2(v1, v2), decVector2(v3, v4));
	ASSERT_TRUE(matrix.IsEqualTo(groundTruth));
	
	// test scale-rotate-translate
	pInitMatrix(matrix, 10.0f);
	matrix.SetSRT(v1, v2, r1, v3, v4);
	groundTruth = decTexMatrix2::CreateScale(v1, v2)
		* decTexMatrix2::CreateRotation( r1 )
		* decTexMatrix2::CreateTranslation( v3, v4 );
	ASSERT_TRUE(matrix.IsEqualTo(groundTruth));
	
	pInitMatrix(matrix, 10.0f);
	matrix.SetSRT(decVector2(v1, v2), r1, decVector2(v3, v4));
	ASSERT_TRUE(matrix.IsEqualTo(groundTruth));
	
	// test center scale-rotate-translate
	pInitMatrix(matrix, 10.0f);
	matrix.SetCenterSRT(v1, v2, r1, v3, v4);
	groundTruth = 
		decTexMatrix2::CreateTranslation(-0.5f, -0.5f)
		* decTexMatrix2::CreateScale( v1, v2 )
		* decTexMatrix2::CreateRotation( r1 )
		* decTexMatrix2::CreateTranslation( v3, v4 )
		* decTexMatrix2::CreateTranslation( 0.5f, 0.5f );
	ASSERT_TRUE(matrix.IsEqualTo(groundTruth));
	
	pInitMatrix(matrix, 10.0f);
	matrix.SetCenterSRT(decVector2(v1, v2), r1, decVector2(v3, v4));
	ASSERT_TRUE(matrix.IsEqualTo(groundTruth));
}

void detTexMatrix2::pTestOperators(){
	SetSubTestNum(2);
	
	const float v1 = 0.1f;
	const float v2 = 0.2f;
	const float scale = 50.0f;
	const decVector2 vec1 = decVector2(v1, v2);
	decVector2 vecTest, vecGroundTruth;
	
	decTexMatrix2 matrix1, matrix2, matrix3, matrixGroundTruth;
	
	// operator +
	pInitMatrix(matrix1, 10.0f);
	pInitMatrix(matrix2, 20.0f);
	matrix3 = matrix1 + matrix2;
	
	ASSERT_FEQUAL(matrix3.a11, matrix1.a11 + matrix2.a11);
	ASSERT_FEQUAL(matrix3.a12, matrix1.a12 + matrix2.a12);
	ASSERT_FEQUAL(matrix3.a13, matrix1.a13 + matrix2.a13);
	
	ASSERT_FEQUAL(matrix3.a21, matrix1.a21 + matrix2.a21);
	ASSERT_FEQUAL(matrix3.a22, matrix1.a22 + matrix2.a22);
	ASSERT_FEQUAL(matrix3.a23, matrix1.a23 + matrix2.a23);
	
	// operator -
	matrix3 = matrix1 - matrix2;
	
	ASSERT_FEQUAL(matrix3.a11, matrix1.a11 - matrix2.a11);
	ASSERT_FEQUAL(matrix3.a12, matrix1.a12 - matrix2.a12);
	ASSERT_FEQUAL(matrix3.a13, matrix1.a13 - matrix2.a13);
	
	ASSERT_FEQUAL(matrix3.a21, matrix1.a21 - matrix2.a21);
	ASSERT_FEQUAL(matrix3.a22, matrix1.a22 - matrix2.a22);
	ASSERT_FEQUAL(matrix3.a23, matrix1.a23 - matrix2.a23);
	
	// operator +=
	matrix3 = matrix1;
	matrix3 += matrix2;
	
	ASSERT_FEQUAL(matrix3.a11, matrix1.a11 + matrix2.a11);
	ASSERT_FEQUAL(matrix3.a12, matrix1.a12 + matrix2.a12);
	ASSERT_FEQUAL(matrix3.a13, matrix1.a13 + matrix2.a13);
	
	ASSERT_FEQUAL(matrix3.a21, matrix1.a21 + matrix2.a21);
	ASSERT_FEQUAL(matrix3.a22, matrix1.a22 + matrix2.a22);
	ASSERT_FEQUAL(matrix3.a23, matrix1.a23 + matrix2.a23);
	
	// operator -=
	matrix3 = matrix1;
	matrix3 -= matrix2;
	
	ASSERT_FEQUAL(matrix3.a11, matrix1.a11 - matrix2.a11);
	ASSERT_FEQUAL(matrix3.a12, matrix1.a12 - matrix2.a12);
	ASSERT_FEQUAL(matrix3.a13, matrix1.a13 - matrix2.a13);
	
	ASSERT_FEQUAL(matrix3.a21, matrix1.a21 - matrix2.a21);
	ASSERT_FEQUAL(matrix3.a22, matrix1.a22 - matrix2.a22);
	ASSERT_FEQUAL(matrix3.a23, matrix1.a23 - matrix2.a23);
	
	// operator * skalar
	matrix3 = matrix1 * scale;
	
	ASSERT_FEQUAL(matrix3.a11, matrix1.a11 * scale);
	ASSERT_FEQUAL(matrix3.a12, matrix1.a12 * scale);
	ASSERT_FEQUAL(matrix3.a13, matrix1.a13 * scale);
	
	ASSERT_FEQUAL(matrix3.a21, matrix1.a21 * scale);
	ASSERT_FEQUAL(matrix3.a22, matrix1.a22 * scale);
	ASSERT_FEQUAL(matrix3.a23, matrix1.a23 * scale);
	
	// operator / skalar
	matrix3 = matrix1 / scale;
	
	ASSERT_FEQUAL(matrix3.a11, matrix1.a11 / scale);
	ASSERT_FEQUAL(matrix3.a12, matrix1.a12 / scale);
	ASSERT_FEQUAL(matrix3.a13, matrix1.a13 / scale);
	
	ASSERT_FEQUAL(matrix3.a21, matrix1.a21 / scale);
	ASSERT_FEQUAL(matrix3.a22, matrix1.a22 / scale);
	ASSERT_FEQUAL(matrix3.a23, matrix1.a23 / scale);
	
	// operator += skalar
	matrix3 = matrix1;
	matrix3 *= scale;
	
	ASSERT_FEQUAL(matrix3.a11, matrix1.a11 * scale);
	ASSERT_FEQUAL(matrix3.a12, matrix1.a12 * scale);
	ASSERT_FEQUAL(matrix3.a13, matrix1.a13 * scale);
	
	ASSERT_FEQUAL(matrix3.a21, matrix1.a21 * scale);
	ASSERT_FEQUAL(matrix3.a22, matrix1.a22 * scale);
	ASSERT_FEQUAL(matrix3.a23, matrix1.a23 * scale);
	
	// operator /= skalar
	matrix3 = matrix1;
	matrix3 /= scale;
	
	ASSERT_FEQUAL(matrix3.a11, matrix1.a11 / scale);
	ASSERT_FEQUAL(matrix3.a12, matrix1.a12 / scale);
	ASSERT_FEQUAL(matrix3.a13, matrix1.a13 / scale);
	
	ASSERT_FEQUAL(matrix3.a21, matrix1.a21 / scale);
	ASSERT_FEQUAL(matrix3.a22, matrix1.a22 / scale);
	ASSERT_FEQUAL(matrix3.a23, matrix1.a23 / scale);
	
	// operator * vector2
	pInitMatrix(matrix1, 10.0f);
	vecTest = matrix1 * vec1;
	vecGroundTruth.x = matrix1.a11 * vec1.x + matrix1.a12 * vec1.y + matrix1.a13;
	vecGroundTruth.y = matrix1.a21 * vec1.x + matrix1.a22 * vec1.y + matrix1.a23;
	
	ASSERT_TRUE(vecTest.IsEqualTo(vecGroundTruth));
	
	// operator *= matrix
	pInitMatrix(matrix1, 10.0f);
	pInitMatrix(matrix2, 20.0f);
	matrix3 = matrix1 * matrix2;
	matrixGroundTruth.a11 = matrix1.a11 * matrix2.a11 + matrix1.a21 * matrix2.a12;
	matrixGroundTruth.a12 = matrix1.a12 * matrix2.a11 + matrix1.a22 * matrix2.a12;
	matrixGroundTruth.a13 = matrix1.a13 * matrix2.a11 + matrix1.a23 * matrix2.a12 + matrix2.a13;
	matrixGroundTruth.a21 = matrix1.a11 * matrix2.a21 + matrix1.a21 * matrix2.a22;
	matrixGroundTruth.a22 = matrix1.a12 * matrix2.a21 + matrix1.a22 * matrix2.a22;
	matrixGroundTruth.a23 = matrix1.a13 * matrix2.a21 + matrix1.a23 * matrix2.a22 + matrix2.a23;
	
	ASSERT_TRUE(matrix3.IsEqualTo(matrixGroundTruth));
}

void detTexMatrix2::pTestInvert(){
	SetSubTestNum(3);
	decTexMatrix2 m1;
	
	m1.a11 = 1.0f; m1.a12 = 2.0f; m1.a13 = 3.0f;
	m1.a21 = 4.0f; m1.a22 = 5.0f; m1.a23 = 6.0f;
	
	ASSERT_TRUE(m1.ToTexMatrix().IsEqualTo(m1.Invert().Invert()));
}
