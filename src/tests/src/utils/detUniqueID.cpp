#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "detUniqueID.h"

#include <dragengine/common/utils/decUniqueID.h>
#include <dragengine/common/collection/decGlobalFunctions.h>
#include <dragengine/common/exceptions.h>



// Class detUniqueID
//////////////////////

// Constructors, destructor
/////////////////////////////

detUniqueID::detUniqueID(){
	Prepare();
}

detUniqueID::~detUniqueID(){
	CleanUp();
}



// Testing
////////////

void detUniqueID::Prepare(){
}

void detUniqueID::Run(){
	TestInit();
	TestIncrement();
	TestIncrementByInt();
	TestIncrementByID();
	TestDecrement();
	TestCompare();
	TestHexString();
	TestDEHash();
	TestDECompare();
}

void detUniqueID::CleanUp(){
}

const char *detUniqueID::GetTestName(){return "UniqueID";}



// Tests
//////////

void detUniqueID::TestInit(){
	SetSubTestNum(0);
	
	// new id
	decUniqueID id1;
	ASSERT_TRUE(id1.GetBitCount() == 8);
	ASSERT_TRUE(id1.GetByteAt(0) == 0);
	
	// set value
	id1.SetByteAt(0, 10);
	ASSERT_TRUE(id1.GetBitCount() == 8);
	ASSERT_TRUE(id1.GetByteAt(0) == 10);
	
	// change bit count
	id1.SetBitCount(32);
	ASSERT_TRUE(id1.GetBitCount() == 32);
	ASSERT_TRUE(id1.GetByteAt(0) == 0);
	ASSERT_TRUE(id1.GetByteAt(1) == 0);
	ASSERT_TRUE(id1.GetByteAt(2) == 0);
	ASSERT_TRUE(id1.GetByteAt(3) == 0);
	
	// set value
	id1.SetByteAt(0, 10);
	id1.SetByteAt(1, 20);
	id1.SetByteAt(2, 30);
	id1.SetByteAt(3, 40);
	ASSERT_TRUE(id1.GetBitCount() == 32);
	ASSERT_TRUE(id1.GetByteAt(0) == 10);
	ASSERT_TRUE(id1.GetByteAt(1) == 20);
	ASSERT_TRUE(id1.GetByteAt(2) == 30);
	ASSERT_TRUE(id1.GetByteAt(3) == 40);
	
	// copy id
	decUniqueID id2(id1);
	ASSERT_TRUE(id2.GetBitCount() == 32);
	ASSERT_TRUE(id2.GetByteAt(0) == 10);
	ASSERT_TRUE(id2.GetByteAt(1) == 20);
	ASSERT_TRUE(id2.GetByteAt(2) == 30);
	ASSERT_TRUE(id2.GetByteAt(3) == 40);
	
	decUniqueID id3;
	id3 = id1;
	ASSERT_TRUE(id3.GetBitCount() == 32);
	ASSERT_TRUE(id3.GetByteAt(0) == 10);
	ASSERT_TRUE(id3.GetByteAt(1) == 20);
	ASSERT_TRUE(id3.GetByteAt(2) == 30);
	ASSERT_TRUE(id3.GetByteAt(3) == 40);
	
	// init from value
	decUniqueID id4((0x7a << 16) + (0x12 << 8) + 0xb4);
	ASSERT_TRUE(id4.GetBitCount() == 24);
	ASSERT_TRUE(id4.GetByteAt(0) == 0xb4);
	ASSERT_TRUE(id4.GetByteAt(1) == 0x12);
	ASSERT_TRUE(id4.GetByteAt(2) == 0x7a);
}

void detUniqueID::TestIncrement(){
	SetSubTestNum(1);
	
	// increment without wrap
	decUniqueID id;
	
	id.Increment();
	ASSERT_TRUE(id.GetBitCount() == 8);
	ASSERT_TRUE(id.GetByteAt(0) == 1);
	
	id.SetByteAt(0, 10);
	ASSERT_TRUE(id.GetBitCount() == 8);
	ASSERT_TRUE(id.GetByteAt(0) == 10);
	
	id.Increment();
	ASSERT_TRUE(id.GetBitCount() == 8);
	ASSERT_TRUE(id.GetByteAt(0) == 11);
	
	// increment with wrap
	id.SetByteAt(0, 255);
	id.Increment();
	ASSERT_TRUE(id.GetBitCount() == 16);
	ASSERT_TRUE(id.GetByteAt(0) == 0);
	ASSERT_TRUE(id.GetByteAt(1) == 1);
	
	id.Increment();
	ASSERT_TRUE(id.GetBitCount() == 16);
	ASSERT_TRUE(id.GetByteAt(0) == 1);
	ASSERT_TRUE(id.GetByteAt(1) == 1);
	
	// test increment with wrap without changing bit count
	id.SetBitCount(32);
	
	id.SetByteAt(0, 255);
	id.SetByteAt(1, 20);
	id.SetByteAt(2, 30);
	id.SetByteAt(3, 40);
	
	id.Increment();
	ASSERT_TRUE(id.GetBitCount() == 32);
	ASSERT_TRUE(id.GetByteAt(0) == 0);
	ASSERT_TRUE(id.GetByteAt(1) == 21);
	ASSERT_TRUE(id.GetByteAt(2) == 30);
	ASSERT_TRUE(id.GetByteAt(3) == 40);
}

void detUniqueID::TestIncrementByInt()
{
	SetSubTestNum(2);
	
	// test increment by amount no wrap
	decUniqueID id2(55);
	id2.IncrementBy(200);
	ASSERT_TRUE(id2.GetBitCount() == 8);
	ASSERT_TRUE(id2.GetByteAt(0) == 255);
	
	decUniqueID id3((12 << 8) + 55);
	id3.IncrementBy(200);
	ASSERT_TRUE(id3.GetBitCount() == 16);
	ASSERT_TRUE(id3.GetByteAt(0) == 255);
	ASSERT_TRUE(id3.GetByteAt(1) == 12);
	
	// test increment by amount wrapping and changing bit count
	decUniqueID id4(55);
	id4.IncrementBy(800);
	ASSERT_TRUE(id4.GetBitCount() == 16);
	ASSERT_TRUE(id4.GetByteAt(0) == 87);
	ASSERT_TRUE(id4.GetByteAt(1) == 3);
	
	// test increment by amount wrapping but not changing bit count
	decUniqueID id5(850);
	id5.IncrementBy(200);
	ASSERT_TRUE(id5.GetBitCount() == 16);
	ASSERT_TRUE(id5.GetByteAt(0) == 26);
	ASSERT_TRUE(id5.GetByteAt(1) == 4);
	
	// test increment by amount wrapping and changing bit count multiple times
	decUniqueID id6(80);
	id6.IncrementBy(80220000);
	ASSERT_TRUE(id6.GetBitCount() == 32);
	ASSERT_TRUE(id6.GetByteAt(0) == 176);
	ASSERT_TRUE(id6.GetByteAt(1) == 15);
	ASSERT_TRUE(id6.GetByteAt(2) == 200);
	ASSERT_TRUE(id6.GetByteAt(3) == 4);
}

void detUniqueID::TestIncrementByID()
{
	SetSubTestNum(3);
	
	// test increment by amount no wrap
	decUniqueID id2(55);
	id2.IncrementBy(decUniqueID(200));
	ASSERT_TRUE(id2.GetBitCount() == 8);
	ASSERT_TRUE(id2.GetByteAt(0) == 255);
	
	decUniqueID id3((12 << 8) + 55);
	id3.IncrementBy(decUniqueID(200));
	ASSERT_TRUE(id3.GetBitCount() == 16);
	ASSERT_TRUE(id3.GetByteAt(0) == 255);
	ASSERT_TRUE(id3.GetByteAt(1) == 12);
	
	// test increment by amount wrapping and changing bit count
	decUniqueID id4(55);
	id4.IncrementBy(decUniqueID(800));
	ASSERT_TRUE(id4.GetBitCount() == 16);
	ASSERT_TRUE(id4.GetByteAt(0) == 87);
	ASSERT_TRUE(id4.GetByteAt(1) == 3);
	
	// test increment by amount wrapping but not changing bit count
	decUniqueID id5(850);
	id5.IncrementBy(decUniqueID(200));
	ASSERT_TRUE(id5.GetBitCount() == 16);
	ASSERT_TRUE(id5.GetByteAt(0) == 26);
	ASSERT_TRUE(id5.GetByteAt(1) == 4);
	
	// test increment by amount wrapping and changing bit count multiple times
	decUniqueID id6(80);
	id6.IncrementBy(decUniqueID(80220000));
	ASSERT_TRUE(id6.GetBitCount() == 32);
	ASSERT_TRUE(id6.GetByteAt(0) == 176);
	ASSERT_TRUE(id6.GetByteAt(1) == 15);
	ASSERT_TRUE(id6.GetByteAt(2) == 200);
	ASSERT_TRUE(id6.GetByteAt(3) == 4);
}

void detUniqueID::TestCompare(){
	SetSubTestNum(4);
	
	// compare various ids
	decUniqueID id1, id2;
	
	id1.SetBitCount(16);
	id1.SetByteAt(0, 10);
	id1.SetByteAt(1, 20);
	id2.SetBitCount(16);
	id2.SetByteAt(0, 10);
	id2.SetByteAt(1, 20);
	ASSERT_TRUE(id1 == id2);
	ASSERT_FALSE(id1 != id2);
	
	id2.SetBitCount(32);
	id2.SetByteAt(0, 10);
	id2.SetByteAt(1, 20);
	id2.SetByteAt(2, 0);
	id2.SetByteAt(3, 0);
	ASSERT_TRUE(id1 == id2);
	ASSERT_FALSE(id1 != id2);
	
	id2.SetBitCount(16);
	id2.SetByteAt(0, 11);
	id2.SetByteAt(1, 20);
	ASSERT_FALSE(id1 == id2);
	ASSERT_TRUE(id1 != id2);
	
	id2.SetBitCount(16);
	id2.SetByteAt(0, 10);
	id2.SetByteAt(1, 21);
	ASSERT_FALSE(id1 == id2);
	ASSERT_TRUE(id1 != id2);
	
	id2.SetBitCount(32);
	id2.SetByteAt(0, 11);
	id2.SetByteAt(1, 20);
	id2.SetByteAt(2, 0);
	id2.SetByteAt(3, 0);
	ASSERT_FALSE(id1 == id2);
	ASSERT_TRUE(id1 != id2);
	
	id2.SetBitCount(32);
	id2.SetByteAt(0, 10);
	id2.SetByteAt(1, 21);
	id2.SetByteAt(2, 0);
	id2.SetByteAt(3, 0);
	ASSERT_FALSE(id1 == id2);
	ASSERT_TRUE(id1 != id2);
	
	id2.SetBitCount(32);
	id2.SetByteAt(0, 10);
	id2.SetByteAt(1, 20);
	id2.SetByteAt(2, 1);
	id2.SetByteAt(3, 0);
	ASSERT_FALSE(id1 == id2);
	ASSERT_TRUE(id1 != id2);
	
	id2.SetBitCount(32);
	id2.SetByteAt(0, 10);
	id2.SetByteAt(1, 20);
	id2.SetByteAt(2, 0);
	id2.SetByteAt(3, 1);
	ASSERT_FALSE(id1 == id2);
	ASSERT_TRUE(id1 != id2);
	
	id2.SetBitCount(16);
	id2.SetByteAt(0, 10);
	id2.SetByteAt(1, 20);
	id1.SetBitCount(8);
	id1.SetByteAt(0, 10);
	ASSERT_FALSE(id1 == id2);
	ASSERT_TRUE(id1 != id2);
	
	id2.SetBitCount(16);
	id2.SetByteAt(0, 10);
	id2.SetByteAt(1, 20);
	id1.SetBitCount(8);
	id1.SetByteAt(0, 11);
	ASSERT_FALSE(id1 == id2);
	ASSERT_TRUE(id1 != id2);
	
	
	
	// 1 byte
	ASSERT_TRUE(decUniqueID(0x12) < decUniqueID(0xf4));
	ASSERT_TRUE(decUniqueID(0x12) <= decUniqueID(0xf4));
	ASSERT_FALSE(decUniqueID(0xf4) < decUniqueID(0x12));
	ASSERT_FALSE(decUniqueID(0xf4) <= decUniqueID(0x12));
	ASSERT_FALSE(decUniqueID(0x12) > decUniqueID(0xf4));
	ASSERT_FALSE(decUniqueID(0x12) >= decUniqueID(0xf4));
	ASSERT_TRUE(decUniqueID(0xf4) > decUniqueID(0x12));
	ASSERT_TRUE(decUniqueID(0xf4) >= decUniqueID(0x12));
	
	// 1 vs 2 byte
	ASSERT_TRUE(decUniqueID(0x12) < decUniqueID(0x057a));
	ASSERT_TRUE(decUniqueID(0x12) <= decUniqueID(0x057a));
	ASSERT_FALSE(decUniqueID(0x057a) < decUniqueID(0x12));
	ASSERT_FALSE(decUniqueID(0x057a) <= decUniqueID(0x12));
	ASSERT_FALSE(decUniqueID(0x12) > decUniqueID(0x057a));
	ASSERT_FALSE(decUniqueID(0x12) >= decUniqueID(0x057a));
	ASSERT_TRUE(decUniqueID(0x057a) > decUniqueID(0x12));
	ASSERT_TRUE(decUniqueID(0x057a) >= decUniqueID(0x12));
	
	// 1 vs 3 byte
	ASSERT_TRUE(decUniqueID(0x12) < decUniqueID(0xf144ab));
	ASSERT_TRUE(decUniqueID(0x12) <= decUniqueID(0xf144ab));
	ASSERT_FALSE(decUniqueID(0xf144ab) < decUniqueID(0x12));
	ASSERT_FALSE(decUniqueID(0xf144ab) <= decUniqueID(0x12));
	ASSERT_FALSE(decUniqueID(0x12) > decUniqueID(0xf144ab));
	ASSERT_FALSE(decUniqueID(0x12) >= decUniqueID(0xf144ab));
	ASSERT_TRUE(decUniqueID(0xf144ab) > decUniqueID(0x12));
	ASSERT_TRUE(decUniqueID(0xf144ab) >= decUniqueID(0x12));
	
	// 2 bytes, first important
	ASSERT_TRUE(decUniqueID(0x2638) < decUniqueID(0x7312));
	ASSERT_TRUE(decUniqueID(0x2638) <= decUniqueID(0x7312));
	ASSERT_FALSE(decUniqueID(0x7312) < decUniqueID(0x2638));
	ASSERT_FALSE(decUniqueID(0x7312) <= decUniqueID(0x2638));
	ASSERT_FALSE(decUniqueID(0x2638) > decUniqueID(0x7312));
	ASSERT_FALSE(decUniqueID(0x2638) >= decUniqueID(0x7312));
	ASSERT_TRUE(decUniqueID(0x7312) > decUniqueID(0x2638));
	ASSERT_TRUE(decUniqueID(0x7312) >= decUniqueID(0x2638));
	
	// 2 vs 3 byte
	ASSERT_TRUE(decUniqueID(0x2638) < decUniqueID(0xf144ab));
	ASSERT_TRUE(decUniqueID(0x2638) <= decUniqueID(0xf144ab));
	ASSERT_FALSE(decUniqueID(0xf144ab) < decUniqueID(0x2638));
	ASSERT_FALSE(decUniqueID(0xf144ab) <= decUniqueID(0x2638));
	ASSERT_FALSE(decUniqueID(0x2638) > decUniqueID(0xf144ab));
	ASSERT_FALSE(decUniqueID(0x2638) >= decUniqueID(0xf144ab));
	ASSERT_TRUE(decUniqueID(0xf144ab) > decUniqueID(0x2638));
	ASSERT_TRUE(decUniqueID(0xf144ab) >= decUniqueID(0x2638));
	
	// 2 bytes, first equal
	ASSERT_TRUE(decUniqueID(0x2612) < decUniqueID(0x2683));
	ASSERT_TRUE(decUniqueID(0x2612) <= decUniqueID(0x2683));
	ASSERT_FALSE(decUniqueID(0x2683) < decUniqueID(0x2612));
	ASSERT_FALSE(decUniqueID(0x2683) <= decUniqueID(0x2612));
	ASSERT_FALSE(decUniqueID(0x2612) > decUniqueID(0x2683));
	ASSERT_FALSE(decUniqueID(0x2612) >= decUniqueID(0x2683));
	ASSERT_TRUE(decUniqueID(0x2683) > decUniqueID(0x2612));
	ASSERT_TRUE(decUniqueID(0x2683) >= decUniqueID(0x2612));
	
	// 3 bytes, first equal
	ASSERT_TRUE(decUniqueID(0x261267) < decUniqueID(0x2683fa));
	ASSERT_TRUE(decUniqueID(0x261267) <= decUniqueID(0x2683fa));
	ASSERT_FALSE(decUniqueID(0x2683fa) < decUniqueID(0x261267));
	ASSERT_FALSE(decUniqueID(0x2683fa) <= decUniqueID(0x261267));
	ASSERT_FALSE(decUniqueID(0x261267) > decUniqueID(0x2683fa));
	ASSERT_FALSE(decUniqueID(0x261267) >= decUniqueID(0x2683fa));
	ASSERT_TRUE(decUniqueID(0x2683fa) > decUniqueID(0x261267));
	ASSERT_TRUE(decUniqueID(0x2683fa) >= decUniqueID(0x261267));
	
	// 3 bytes, first and second equal
	ASSERT_TRUE(decUniqueID(0x261267) < decUniqueID(0x2612fa));
	ASSERT_TRUE(decUniqueID(0x261267) <= decUniqueID(0x2612fa));
	ASSERT_FALSE(decUniqueID(0x2612fa) < decUniqueID(0x261267));
	ASSERT_FALSE(decUniqueID(0x2612fa) <= decUniqueID(0x261267));
	ASSERT_FALSE(decUniqueID(0x261267) > decUniqueID(0x2612fa));
	ASSERT_FALSE(decUniqueID(0x261267) >= decUniqueID(0x2612fa));
	ASSERT_TRUE(decUniqueID(0x2612fa) > decUniqueID(0x261267));
	ASSERT_TRUE(decUniqueID(0x2612fa) >= decUniqueID(0x261267));
	
	// 3 bytes, all equal
	ASSERT_FALSE(decUniqueID(0x2612fa) < decUniqueID(0x2612fa));
	ASSERT_TRUE(decUniqueID(0x2612fa) <= decUniqueID(0x2612fa));
	ASSERT_FALSE(decUniqueID(0x2612fa) > decUniqueID(0x2612fa));
	ASSERT_TRUE(decUniqueID(0x2612fa) >= decUniqueID(0x2612fa));
}

void detUniqueID::TestDecrement(){
	SetSubTestNum(5);
	
	// decrement
	decUniqueID id1(0);
	ASSERT_DOES_FAIL(id1.Decrement());
	ASSERT_EQUAL(id1, decUniqueID(0));
	
	decUniqueID id2(120);
	id2.Decrement();
	ASSERT_EQUAL(id2, decUniqueID(119));
	
	decUniqueID id3((120 << 8) + 35);
	id3.Decrement();
	ASSERT_EQUAL(id3, decUniqueID((120 << 8) + 34));
	
	decUniqueID id4((120 << 8) + 0);
	id4.Decrement();
	ASSERT_EQUAL(id4, decUniqueID((119 << 8) + 255));
	
	decUniqueID id5((230 << 16) + (0 << 8) + 35);
	id5.Decrement();
	ASSERT_EQUAL(id5, decUniqueID((230 << 16) + (0 << 8) + 34));
	
	decUniqueID id6((230 << 16) + (0 << 8) + 0);
	id6.Decrement();
	ASSERT_EQUAL(id6, decUniqueID((229 << 16) + (255 << 8) + 255));
	
	// decrement by integer
	decUniqueID id10(100);
	ASSERT_DOES_FAIL(id10.DecrementBy(101));
	ASSERT_EQUAL(id10, decUniqueID(100));
	ASSERT_DOES_FAIL(id10.DecrementBy(103345));
	ASSERT_EQUAL(id10, decUniqueID(100));
	
	id10.DecrementBy(99);
	ASSERT_EQUAL(id10, decUniqueID(1));
	
	decUniqueID id11(200);
	id11.DecrementBy(200);
	ASSERT_EQUAL(id11, decUniqueID(0));
	
	decUniqueID id12(0x125a85);
	ASSERT_DOES_FAIL(id12.DecrementBy(0x125a86));
	ASSERT_EQUAL(id12, decUniqueID(0x125a85));
	ASSERT_DOES_FAIL(id12.DecrementBy(0x125b85));
	ASSERT_EQUAL(id12, decUniqueID(0x125a85));
	ASSERT_DOES_FAIL(id12.DecrementBy(0x135a85));
	ASSERT_EQUAL(id12, decUniqueID(0x125a85));
	ASSERT_DOES_FAIL(id12.DecrementBy(0x1125a85));
	ASSERT_EQUAL(id12, decUniqueID(0x125a85));
	
	id12.DecrementBy(20);
	ASSERT_EQUAL(id12, decUniqueID(0x125a85 - 20));
	
	decUniqueID id13(0x125a85);
	id13.DecrementBy(0x95);
	ASSERT_EQUAL(id13, decUniqueID(0x125a85 - 0x95));
	
	decUniqueID id14(0x125a85);
	id14.DecrementBy(0x0178a1);
	ASSERT_EQUAL(id14, decUniqueID(0x125a85 - 0x0178a1));
	
	// decrement by uniqueid
	decUniqueID id20(100);
	ASSERT_DOES_FAIL(id20.DecrementBy(decUniqueID(101)));
	ASSERT_EQUAL(id20, decUniqueID(100));
	ASSERT_DOES_FAIL(id20.DecrementBy(decUniqueID(103345)));
	ASSERT_EQUAL(id20, decUniqueID(100));
	
	id20.DecrementBy(decUniqueID(99));
	ASSERT_EQUAL(id20, decUniqueID(1));
	
	decUniqueID id21(200);
	id21.DecrementBy(decUniqueID(200));
	ASSERT_EQUAL(id21, decUniqueID(0));
	
	decUniqueID id22(0x125a85);
	ASSERT_DOES_FAIL(id22.DecrementBy(decUniqueID(0x125a86)));
	ASSERT_EQUAL(id22, decUniqueID(0x125a85));
	ASSERT_DOES_FAIL(id22.DecrementBy(decUniqueID(0x125b85)));
	ASSERT_EQUAL(id22, decUniqueID(0x125a85));
	ASSERT_DOES_FAIL(id22.DecrementBy(decUniqueID(0x135a85)));
	ASSERT_EQUAL(id22, decUniqueID(0x125a85));
	ASSERT_DOES_FAIL(id22.DecrementBy(decUniqueID(0x1125a85)));
	ASSERT_EQUAL(id22, decUniqueID(0x125a85));
	
	id22.DecrementBy(decUniqueID(20));
	ASSERT_EQUAL(id22, decUniqueID(0x125a85 - 20));
	
	decUniqueID id23(0x125a85);
	id23.DecrementBy(decUniqueID(0x95));
	ASSERT_EQUAL(id23, decUniqueID(0x125a85 - 0x95));
	
	decUniqueID id24(0x125a85);
	id24.DecrementBy(decUniqueID(0x0178a1));
	ASSERT_EQUAL(id24, decUniqueID(0x125a85 - 0x0178a1));
}

void detUniqueID::TestHexString(){
	SetSubTestNum(6);
	
	// from string
	ASSERT_EQUAL(decUniqueID("0"), decUniqueID(0x0));
	ASSERT_EQUAL(decUniqueID("5"), decUniqueID(0x5));
	ASSERT_EQUAL(decUniqueID("c"), decUniqueID(0xc));
	ASSERT_EQUAL(decUniqueID("C"), decUniqueID(0xc));
	ASSERT_EQUAL(decUniqueID("12"), decUniqueID(0x12));
	ASSERT_EQUAL(decUniqueID("cD"), decUniqueID(0xcd));
	ASSERT_EQUAL(decUniqueID("0f"), decUniqueID(0xf));
	ASSERT_EQUAL(decUniqueID("123"), decUniqueID(0x123));
	ASSERT_EQUAL(decUniqueID("aBc"), decUniqueID(0xabc));
	ASSERT_EQUAL(decUniqueID("1234"), decUniqueID(0x1234));
	ASSERT_EQUAL(decUniqueID("12345678"), decUniqueID(0x12345678));
	
	// to string
	ASSERT_EQUAL(decUniqueID(0x0).ToHexString(), "0");
	ASSERT_EQUAL(decUniqueID(0x5).ToHexString(), "5");
	ASSERT_EQUAL(decUniqueID(0xc).ToHexString(), "c");
	ASSERT_EQUAL(decUniqueID(0x12).ToHexString(), "12");
	ASSERT_EQUAL(decUniqueID(0xcd).ToHexString(), "cd");
	ASSERT_EQUAL(decUniqueID(0xf).ToHexString(), "f");
	ASSERT_EQUAL(decUniqueID(0x123).ToHexString(), "123");
	ASSERT_EQUAL(decUniqueID(0xabc).ToHexString(), "abc");
	ASSERT_EQUAL(decUniqueID(0x1234).ToHexString(), "1234");
	ASSERT_EQUAL(decUniqueID(0x12345678).ToHexString(), "12345678");
	
	ASSERT_EQUAL(decUniqueID("12005600").ToHexString(), "12005600");
	ASSERT_EQUAL(decUniqueID("00005600").ToHexString(), "5600");
	ASSERT_EQUAL(decUniqueID("00000000").ToHexString(), "0");
	ASSERT_EQUAL(decUniqueID("1234567890abcdef").ToHexString(), "1234567890abcdef");
}


// Test DEHash
//////////////

void detUniqueID::TestDEHash(){
	SetSubTestNum(7);
	
	// Test that same IDs produce same hash
	decUniqueID id1(0x12345678);
	decUniqueID id2(0x12345678);
	decUniqueID id3(0x7bcdef00);  // Use positive int value
	
	unsigned int hash1 = DEHash(id1);
	unsigned int hash2 = DEHash(id2);
	unsigned int hash3 = DEHash(id3);
	
	ASSERT_EQUAL(hash1, hash2);  // Same ID should have same hash
	ASSERT_TRUE(hash1 != hash3);  // Different IDs should (likely) have different hash
	
	// Test empty ID
	decUniqueID empty1;
	decUniqueID empty2;
	ASSERT_EQUAL(DEHash(empty1), DEHash(empty2));
	
	// Test that DEHash uses the Hash() method
	ASSERT_EQUAL(DEHash(id1), id1.Hash());
	ASSERT_EQUAL(DEHash(id3), id3.Hash());
	
	// Test with larger IDs
	decUniqueID large1("1234567890abcdef");
	decUniqueID large2("1234567890abcdef");
	ASSERT_EQUAL(DEHash(large1), DEHash(large2));
	ASSERT_EQUAL(DEHash(large1), large1.Hash());
}


// Test DECompare
/////////////////

void detUniqueID::TestDECompare(){
	SetSubTestNum(8);
	
	decUniqueID id1(100);
	decUniqueID id2(200);
	decUniqueID id3(100);
	
	// Test less than
	ASSERT_TRUE(DECompare(id1, id2) < 0);  // 100 < 200
	
	// Test greater than
	ASSERT_TRUE(DECompare(id2, id1) > 0);  // 200 > 100
	
	// Test equal
	ASSERT_EQUAL(DECompare(id1, id3), 0);  // 100 == 100
	
	// Test that DECompare uses the Compare() method
	ASSERT_EQUAL(DECompare(id1, id2), id1.Compare(id2));
	ASSERT_EQUAL(DECompare(id2, id1), id2.Compare(id1));
	ASSERT_EQUAL(DECompare(id1, id3), id1.Compare(id3));
	
	// Test with larger IDs
	decUniqueID large1("1234567890abcdef");
	decUniqueID large2("fedcba0987654321");
	ASSERT_TRUE(DECompare(large1, large2) < 0);
	ASSERT_EQUAL(DECompare(large1, large2), large1.Compare(large2));
}
