#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "detUuid.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decUuid.h>
#include <dragengine/common/collection/decGlobalFunctions.h>



// Class detUuid
//////////////////

// Constructors, destructor
/////////////////////////////

detUuid::detUuid(){
	Prepare();
}

detUuid::~detUuid(){
	CleanUp();
}



// Testing
////////////

void detUuid::Prepare(){
}

void detUuid::Run(){
	TestInit();
	TestCompare();
	TestHexString();
	TestDEHash();
}

void detUuid::CleanUp(){
}

const char *detUuid::GetTestName(){return "Uuid";}



// Tests
//////////

void detUuid::TestInit(){
	SetSubTestNum(0);
	
	// empty uuid
	int i;
	decUuid id1;
	for(i=0; i<16; i++){
		ASSERT_TRUE(id1.GetValueAt(i) == 0);
	}
	
	// new uuid
	const uint8_t values1[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	decUuid id2(values1);
	for(i=0; i<16; i++){
		ASSERT_TRUE(id2.GetValueAt(i) == values1[i]);
	}
	
	// set value
	for(i=0; i<16; i++){
		id2.SetValueAt(i, values1[i] + 20);
	}
	for(i=0; i<16; i++){
		ASSERT_TRUE(id2.GetValueAt(i) == values1[i] + 20);
	}
	
	// clear
	id2.Clear();
	for(i=0; i<16; i++){
		ASSERT_TRUE(id2.GetValueAt(i) == 0);
	}
}

void detUuid::TestCompare(){
	SetSubTestNum(1);
	
	const uint8_t values1[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const uint8_t values2[16] = {20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
	
	decUuid id1(values1), id2(values2);
	
	ASSERT_TRUE(id1 == id1);
	ASSERT_FALSE(id1 != id1);
	
	ASSERT_FALSE(id1 == id2);
	ASSERT_TRUE(id1 != id2);
	
	// random uuids have to be different
	decUuid id3(decUuid::Random()), id4(decUuid::Random());
	if(id3 == id4){
		// extremely unlikely that two random uuids are the same but it happened
		id4 = decUuid::Random();
	}
	
	ASSERT_FALSE(id3 == id4);
	ASSERT_TRUE(id3 != id4);
}

void detUuid::TestHexString(){
	SetSubTestNum(2);
	
	const uint8_t values1[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	decUuid id1(values1);
	
	ASSERT_EQUAL(id1.ToHexString(true), "000102030405060708090a0b0c0d0e0f");
	ASSERT_EQUAL(id1.ToHexString(false), "00010203-0405-0607-0809-0a0b0c0d0e0f");
	
	ASSERT_EQUAL(decUuid("000102030405060708090a0b0c0d0e0f", true), id1);
	ASSERT_EQUAL(decUuid("00010203-0405-0607-0809-0a0b0c0d0e0f", false), id1);
	
	ASSERT_DOES_FAIL(decUuid("000102030405060708090a0b0c0d0e", true));
	ASSERT_DOES_FAIL(decUuid("000102030405060708090a0b0c0d0e0", true));
	ASSERT_DOES_FAIL(decUuid("000102030405060708090a0b0c0d0e0f1", true));
	ASSERT_DOES_FAIL(decUuid("000102030405060708090a0b0c0d0e0f10", true));
	ASSERT_DOES_FAIL(decUuid("00010203-0405-0607-0809-0a0b0c0d0e0f", true));
	
	ASSERT_DOES_FAIL(decUuid("00010203-0405-0607-0809-0a0b0c0d0e", false));
	ASSERT_DOES_FAIL(decUuid("00010203-0405-0607-0809-0a0b0c0d0e0", false));
	ASSERT_DOES_FAIL(decUuid("00010203-0405-0607-0809-0a0b0c0d0e0f1", false));
	ASSERT_DOES_FAIL(decUuid("00010203-0405-0607-0809-0a0b0c0d0e0f10", false));
	ASSERT_DOES_FAIL(decUuid("000102030405060708090a0b0c0d0e0f", false));
	ASSERT_DOES_FAIL(decUuid("00010203--0405--0607--0809--0a0b0c0d0e", false));
	ASSERT_DOES_FAIL(decUuid("000102-030405-0607-08090a-0b0c0d0e", false));
	
	id1.SetFromHexString("000102030405060708090a0b0c0d0e0f", true);
	ASSERT_EQUAL(decUuid("000102030405060708090a0b0c0d0e0f", true), id1);
	
	id1.SetFromHexString("00010203-0405-0607-0809-0a0b0c0d0e0f", false);
	ASSERT_EQUAL(decUuid("00010203-0405-0607-0809-0a0b0c0d0e0f", false), id1);
}


// Test DEHash
//////////////

void detUuid::TestDEHash(){
	SetSubTestNum(3);
	
	// Test that same UUIDs produce same hash
	const uint8_t values1[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const uint8_t values2[16] = {20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
	
	decUuid id1(values1);
	decUuid id2(values1);
	decUuid id3(values2);
	
	unsigned int hash1 = DEHash(id1);
	unsigned int hash2 = DEHash(id2);
	unsigned int hash3 = DEHash(id3);
	
	ASSERT_EQUAL(hash1, hash2);  // Same UUID should have same hash
	ASSERT_TRUE(hash1 != hash3);  // Different UUIDs should (likely) have different hash
	
	// Test empty UUID
	decUuid empty1;
	decUuid empty2;
	ASSERT_EQUAL(DEHash(empty1), DEHash(empty2));
	
	// Test that DEHash uses the Hash() method
	ASSERT_EQUAL(DEHash(id1), id1.Hash());
	ASSERT_EQUAL(DEHash(id3), id3.Hash());
	
	// Test random UUIDs
	decUuid random1(decUuid::Random());
	decUuid random2(random1);
	ASSERT_EQUAL(DEHash(random1), DEHash(random2));
}
