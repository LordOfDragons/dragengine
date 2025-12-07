#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "detPRNG.h"
#include <dragengine/common/utils/decPRNG.h>
#include <dragengine/common/exceptions.h>



// Class detPRNG
//////////////////

// Constructors, destructor
/////////////////////////////

detPRNG::detPRNG(){
	Prepare();
}

detPRNG::~detPRNG(){
	CleanUp();
}



// Testing
////////////

void detPRNG::Prepare(){
}

void detPRNG::Run(){
	TestInt();
	TestFloat();
	TestDouble();
}

void detPRNG::CleanUp(){
}

const char *detPRNG::GetTestName(){return "PRNG";}



// Tests
//////////

void detPRNG::TestInt(){
	// test int
	SetSubTestNum(0);
	
	decPRNG prng(123456);
	const int a0 = 333247424;
	const int a1 = 875560663;
	const int a2 = 622511668;
	const int a3 = 520815413;
	const int a4 = 1741317749;
	const int a5 = 1998632356;
	const int a6 = 995354385;
	const int a7 = 450295858;
	const int a8 = 1850442406;
	const int a9 = 1884959630;
	
	ASSERT_EQUAL(prng.RandomInt(), a0);
	ASSERT_EQUAL(prng.RandomInt(), a1);
	ASSERT_EQUAL(prng.RandomInt(), a2);
	ASSERT_EQUAL(prng.RandomInt(), a3);
	ASSERT_EQUAL(prng.RandomInt(), a4);
	ASSERT_EQUAL(prng.RandomInt(), a5);
	ASSERT_EQUAL(prng.RandomInt(), a6);
	ASSERT_EQUAL(prng.RandomInt(), a7);
	ASSERT_EQUAL(prng.RandomInt(), a8);
	ASSERT_EQUAL(prng.RandomInt(), a9);
	
	// test int with upper less than lower
	SetSubTestNum(1);
	
	prng.Reset();
	ASSERT_EQUAL(prng.RandomInt(0, -1), 0);
	ASSERT_EQUAL(prng.RandomInt(0, -1), 0);
	ASSERT_EQUAL(prng.RandomInt(), a2);
	ASSERT_EQUAL(prng.RandomInt(0, -1), 0);
	ASSERT_EQUAL(prng.RandomInt(0, -1), 0);
	ASSERT_EQUAL(prng.RandomInt(0, -1), 0);
	ASSERT_EQUAL(prng.RandomInt(), a6);
	ASSERT_EQUAL(prng.RandomInt(0, -1), 0);
	ASSERT_EQUAL(prng.RandomInt(), a8);
	ASSERT_EQUAL(prng.RandomInt(0, -1), 0);
	
	// test int with range
	SetSubTestNum(2);
	
	prng.Reset();
	const int b0 = 1560;
	const int b1 = 4083;
	const int b2 = 2905;
	const int b3 = 2432;
	const int b4 = 8110;
	const int b5 = 9307;
	const int b6 = 4640;
	const int b7 = 2104;
	const int b8 = 8618;
	const int b9 = 8778;
	
	ASSERT_EQUAL(prng.RandomInt(10, 10000), b0);
	ASSERT_EQUAL(prng.RandomInt(10, 10000), b1);
	ASSERT_EQUAL(prng.RandomInt(10, 10000), b2);
	ASSERT_EQUAL(prng.RandomInt(10, 10000), b3);
	ASSERT_EQUAL(prng.RandomInt(10, 10000), b4);
	ASSERT_EQUAL(prng.RandomInt(10, 10000), b5);
	ASSERT_EQUAL(prng.RandomInt(10, 10000), b6);
	ASSERT_EQUAL(prng.RandomInt(10, 10000), b7);
	ASSERT_EQUAL(prng.RandomInt(10, 10000), b8);
	ASSERT_EQUAL(prng.RandomInt(10, 10000), b9);
}

void detPRNG::TestFloat(){
	// test float
	SetSubTestNum(3);
	
	decPRNG prng(123456);
	const float a0 = 0.155180f;
	const float a1 = 0.407715f;
	const float a2 = 0.289880f;
	const float a3 = 0.242524f;
	const float a4 = 0.810864f;
	const float a5 = 0.930686f;
	const float a6 = 0.463498f;
	const float a7 = 0.209685f;
	const float a8 = 0.861679f;
	const float a9 = 0.877753f;
	
	ASSERT_FEQUAL(prng.RandomFloat(), a0);
	ASSERT_FEQUAL(prng.RandomFloat(), a1);
	ASSERT_FEQUAL(prng.RandomFloat(), a2);
	ASSERT_FEQUAL(prng.RandomFloat(), a3);
	ASSERT_FEQUAL(prng.RandomFloat(), a4);
	ASSERT_FEQUAL(prng.RandomFloat(), a5);
	ASSERT_FEQUAL(prng.RandomFloat(), a6);
	ASSERT_FEQUAL(prng.RandomFloat(), a7);
	ASSERT_FEQUAL(prng.RandomFloat(), a8);
	ASSERT_FEQUAL(prng.RandomFloat(), a9);
	
	// test float with upper less than lower
	SetSubTestNum(4);
	
	prng.Reset();
	ASSERT_FEQUAL(prng.RandomFloat(0.0f, -1.0f), 0.0f);
	ASSERT_FEQUAL(prng.RandomFloat(0.0f, -1.0f), 0.0f);
	ASSERT_FEQUAL(prng.RandomFloat(), a2);
	ASSERT_FEQUAL(prng.RandomFloat(0.0f, -1.0f), 0.0f);
	ASSERT_FEQUAL(prng.RandomFloat(0.0f, -1.0f), 0.0f);
	ASSERT_FEQUAL(prng.RandomFloat(0.0f, -1.0f), 0.0f);
	ASSERT_FEQUAL(prng.RandomFloat(), a6);
	ASSERT_FEQUAL(prng.RandomFloat(0.0f, -1.0f), 0.0f);
	ASSERT_FEQUAL(prng.RandomFloat(), a8);
	ASSERT_FEQUAL(prng.RandomFloat(0.0f, -1.0f), 0.0f);
	
	// test int with range
	SetSubTestNum(5);
	
	prng.Reset();
	const float b0 = 11.551805f;
	const float b1 = 14.077147f;
	const float b2 = 12.898796f;
	const float b3 = 12.425236f;
	const float b4 = 18.108643f;
	const float b5 = 19.306858f;
	const float b6 = 14.634980f;
	const float b7 = 12.096853f;
	const float b8 = 18.616795f;
	const float b9 = 18.777527f;
	
	ASSERT_FEQUAL(prng.RandomFloat(10.0f, 20.0f), b0);
	ASSERT_FEQUAL(prng.RandomFloat(10.0f, 20.0f), b1);
	ASSERT_FEQUAL(prng.RandomFloat(10.0f, 20.0f), b2);
	ASSERT_FEQUAL(prng.RandomFloat(10.0f, 20.0f), b3);
	ASSERT_FEQUAL(prng.RandomFloat(10.0f, 20.0f), b4);
	ASSERT_FEQUAL(prng.RandomFloat(10.0f, 20.0f), b5);
	ASSERT_FEQUAL(prng.RandomFloat(10.0f, 20.0f), b6);
	ASSERT_FEQUAL(prng.RandomFloat(10.0f, 20.0f), b7);
	ASSERT_FEQUAL(prng.RandomFloat(10.0f, 20.0f), b8);
	ASSERT_FEQUAL(prng.RandomFloat(10.0f, 20.0f), b9);
}

void detPRNG::TestDouble(){
	// test double
	SetSubTestNum(6);
	
	decPRNG prng(123456);
	const double a0 = 0.1551804245240895;
	const double a1 = 0.4077147056384546;
	const double a2 = 0.2898795848199537;
	const double a3 = 0.2425235757802257;
	const double a4 = 0.8108642649887429;
	const double a5 = 0.9306857161832441;
	const double a6 = 0.4634980044623362;
	const double a7 = 0.2096853490032653;
	const double a8 = 0.8616793932680410;
	const double a9 = 0.8777527282376554;
	
	ASSERT_FEQUAL(prng.RandomDouble(), a0);
	ASSERT_FEQUAL(prng.RandomDouble(), a1);
	ASSERT_FEQUAL(prng.RandomDouble(), a2);
	ASSERT_FEQUAL(prng.RandomDouble(), a3);
	ASSERT_FEQUAL(prng.RandomDouble(), a4);
	ASSERT_FEQUAL(prng.RandomDouble(), a5);
	ASSERT_FEQUAL(prng.RandomDouble(), a6);
	ASSERT_FEQUAL(prng.RandomDouble(), a7);
	ASSERT_FEQUAL(prng.RandomDouble(), a8);
	ASSERT_FEQUAL(prng.RandomDouble(), a9);
	
	// test float with upper less than lower
	SetSubTestNum(7);
	
	prng.Reset();
	ASSERT_FEQUAL(prng.RandomDouble(0.0, -1.0), 0.0);
	ASSERT_FEQUAL(prng.RandomDouble(0.0, -1.0), 0.0);
	ASSERT_FEQUAL(prng.RandomDouble(), a2);
	ASSERT_FEQUAL(prng.RandomDouble(0.0, -1.0), 0.0);
	ASSERT_FEQUAL(prng.RandomDouble(0.0, -1.0), 0.0);
	ASSERT_FEQUAL(prng.RandomDouble(0.0, -1.0), 0.0);
	ASSERT_FEQUAL(prng.RandomDouble(), a6);
	ASSERT_FEQUAL(prng.RandomDouble(0.0, -1.0), 0.0);
	ASSERT_FEQUAL(prng.RandomDouble(), a8);
	ASSERT_FEQUAL(prng.RandomDouble(0.0, -1.0), 0.0);
	
	// test int with range
	SetSubTestNum(8);
	
	prng.Reset();
	const double b0 = 11.5518042452408949;
	const double b1 = 14.0771470563845469;
	const double b2 = 12.8987958481995371;
	const double b3 = 12.4252357578022572;
	const double b4 = 18.1086426498874289;
	const double b5 = 19.3068571618324398;
	const double b6 = 14.6349800446233616;
	const double b7 = 12.0968534900326539;
	const double b8 = 18.6167939326804088;
	const double b9 = 18.7775272823765533;
	
	ASSERT_FEQUAL(prng.RandomDouble(10.0f, 20.0f), b0);
	ASSERT_FEQUAL(prng.RandomDouble(10.0f, 20.0f), b1);
	ASSERT_FEQUAL(prng.RandomDouble(10.0f, 20.0f), b2);
	ASSERT_FEQUAL(prng.RandomDouble(10.0f, 20.0f), b3);
	ASSERT_FEQUAL(prng.RandomDouble(10.0f, 20.0f), b4);
	ASSERT_FEQUAL(prng.RandomDouble(10.0f, 20.0f), b5);
	ASSERT_FEQUAL(prng.RandomDouble(10.0f, 20.0f), b6);
	ASSERT_FEQUAL(prng.RandomDouble(10.0f, 20.0f), b7);
	ASSERT_FEQUAL(prng.RandomDouble(10.0f, 20.0f), b8);
	ASSERT_FEQUAL(prng.RandomDouble(10.0f, 20.0f), b9);
}
