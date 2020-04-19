#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "detUnicodeString.h"

#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeStringList.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/exceptions.h>



// Class detUnicode
/////////////////////


// Constructors, destructor
/////////////////////////////
detUnicodeString::detUnicodeString(){
	Prepare();
}

detUnicodeString::~detUnicodeString(){
	CleanUp();
}



// Testing
////////////

void detUnicodeString::Prepare(){
}

void detUnicodeString::Run(){
	TestDecodeUTF8();
	TestConstructor();
	TestAccessors();
	TestMutators();
	TestOperators();
	TestAppend();
	TestConversion();
	TestFormat();
	TestFind();
	TestSubString();
	TestReverse();
	TestSplit();
	TestReplace();
	TestTrim();
	TestLowerUpper();
}

void detUnicodeString::CleanUp(){
}

const char *detUnicodeString::GetTestName(){
	return "UnicodeString";
}



// Tests
//////////

void detUnicodeString::TestDecodeUTF8(){
	SetSubTestNum( 0 );
	decUnicodeString string;
	
	// test with a 1 byte wide utf8 string
	string.SetFromUTF8( "Hi!" );
	ASSERT_TRUE( string.GetLength() == 3 );
	ASSERT_TRUE( string.GetAt( 0 ) == 'H' );
	ASSERT_TRUE( string.GetAt( 1 ) == 'i' );
	ASSERT_TRUE( string.GetAt( 2 ) == '!' );
	
	// test with a 2 byte wide utf8 string
	string.SetFromUTF8( "H\xC3""\xA4""?" );
	ASSERT_TRUE( string.GetLength() == 3 );
	ASSERT_TRUE( string.GetAt( 0 ) == 'H' );
	ASSERT_TRUE( string.GetAt( 1 ) == 0xE4 );
	ASSERT_TRUE( string.GetAt( 2 ) == '?' );
	
	// test with another 2 byte wide utf8 string
	string.SetFromUTF8( "H\xC2""\xAE""." );
	ASSERT_TRUE( string.GetLength() == 3 );
	ASSERT_TRUE( string.GetAt( 0 ) == 'H' );
	ASSERT_TRUE( string.GetAt( 1 ) == 0xAE );
	ASSERT_TRUE( string.GetAt( 2 ) == '.' );
	
	// test with a 3 byte wide utf8 string
	string.SetFromUTF8( "H\xE2""\x82""\xAC""," );
	ASSERT_TRUE( string.GetLength() == 3 );
	ASSERT_TRUE( string.GetAt( 0 ) == 'H' );
	ASSERT_TRUE( string.GetAt( 1 ) == 0x20AC );
	ASSERT_TRUE( string.GetAt( 2 ) == ',' );
	
	// test with a 4 byte wide utf8 string
	string.SetFromUTF8( "H\xF0""\x9D""\x84""\x9E""=" );
	ASSERT_TRUE( string.GetLength() == 3 );
	ASSERT_TRUE( string.GetAt( 0 ) == 'H' );
	ASSERT_TRUE( string.GetAt( 1 ) == 0x1D11E );
	ASSERT_TRUE( string.GetAt( 2 ) == '=' );
	
	// test with a mixed byte wide utf8 string
	string.SetFromUTF8( "A\xC3""\xA4""B\xE2""\x82""\xAC""C\xF0""\x9D""\x84""\x9E""D" );
	ASSERT_TRUE( string.GetLength() == 7 );
	ASSERT_TRUE( string.GetAt( 0 ) == 'A' );
	ASSERT_TRUE( string.GetAt( 1 ) == 0xE4 );
	ASSERT_TRUE( string.GetAt( 2 ) == 'B' );
	ASSERT_TRUE( string.GetAt( 3 ) == 0x20AC );
	ASSERT_TRUE( string.GetAt( 4 ) == 'C' );
	ASSERT_TRUE( string.GetAt( 5 ) == 0x1D11E );
	ASSERT_TRUE( string.GetAt( 6 ) == 'D' );
	
	// test with another mixed byte wide utf8 string
	string.SetFromUTF8( "A\xC3""\xA4""\xE2""\x82""\xAC""\xF0""\x9D""\x84""\x9E""B" );
	ASSERT_TRUE( string.GetLength() == 5 );
	ASSERT_TRUE( string.GetAt( 0 ) == 'A' );
	ASSERT_TRUE( string.GetAt( 1 ) == 0xE4 );
	ASSERT_TRUE( string.GetAt( 2 ) == 0x20AC );
	ASSERT_TRUE( string.GetAt( 3 ) == 0x1D11E );
	ASSERT_TRUE( string.GetAt( 4 ) == 'B' );
	
	// same with adding
	string.SetFromUTF8( "A\xC3\xA4" );
	string.AppendFromUTF8( "B\xE2\x82\xAC" );
	string.AppendFromUTF8( "C\xF0\x9D\x84\x9E" );
	string.AppendFromUTF8( "D" );
	ASSERT_TRUE( string.GetLength() == 7 );
	ASSERT_TRUE( string.GetAt( 0 ) == 'A' );
	ASSERT_TRUE( string.GetAt( 1 ) == 0xE4 );
	ASSERT_TRUE( string.GetAt( 2 ) == 'B' );
	ASSERT_TRUE( string.GetAt( 3 ) == 0x20AC );
	ASSERT_TRUE( string.GetAt( 4 ) == 'C' );
	ASSERT_TRUE( string.GetAt( 5 ) == 0x1D11E );
	ASSERT_TRUE( string.GetAt( 6 ) == 'D' );
}

void detUnicodeString::TestConstructor(){
	SetSubTestNum( 1 );
	
	// empty unicode string
	decUnicodeString string1;
	ASSERT_TRUE( string1.GetLength() == 0 );
	
	// string created from a unicode character
	decUnicodeString string2( 0x79 ); // letter y
	ASSERT_TRUE( string2.GetLength() == 1 );
	ASSERT_TRUE( string2.GetAt( 0 ) == 0x79 );
	ASSERT_TRUE( string2[ 0 ] == 0x79 );
	decUnicodeString string3( 0xE4 ); // german ?
	ASSERT_TRUE( string3.GetLength() == 1 );
	ASSERT_TRUE( string3.GetAt( 0 ) == 0xE4 );
	ASSERT_TRUE( string3[ 0 ] == 0xE4 );
	decUnicodeString string4( 0xAE ); // trademark (c)
	ASSERT_TRUE( string4.GetLength() == 1 );
	ASSERT_TRUE( string4.GetAt( 0 ) == 0xAE );
	ASSERT_TRUE( string4[ 0 ] == 0xAE );
	decUnicodeString string5( 0x20AC ); // euro sign
	ASSERT_TRUE( string5.GetLength() == 1 );
	ASSERT_TRUE( string5.GetAt( 0 ) == 0x20AC );
	ASSERT_TRUE( string5[ 0 ] == 0x20AC );
	decUnicodeString string6( 0x1D11E ); // violin key
	ASSERT_TRUE( string6.GetLength() == 1 );
	ASSERT_TRUE( string6.GetAt( 0 ) == 0x1D11E );
	ASSERT_TRUE( string6[ 0 ] == 0x1D11E );
	
	// string created from another unicode string
	decUnicodeString string7;
	string7.SetFromUTF8( "A\xC3""\xA4""\xE2""\x82""\xAC""\xF0""\x9D""\x84""\x9E""B" );
	ASSERT_TRUE( string7.GetLength() == 5 );
	ASSERT_TRUE( string7.GetAt( 0 ) == 'A' );
	ASSERT_TRUE( string7.GetAt( 1 ) == 0xE4 );
	ASSERT_TRUE( string7.GetAt( 2 ) == 0x20AC );
	ASSERT_TRUE( string7.GetAt( 3 ) == 0x1D11E );
	ASSERT_TRUE( string7.GetAt( 4 ) == 'B' );
	ASSERT_TRUE( string7[ 0 ] == 'A' );
	ASSERT_TRUE( string7[ 1 ] == 0xE4 );
	ASSERT_TRUE( string7[ 2 ] == 0x20AC );
	ASSERT_TRUE( string7[ 3 ] == 0x1D11E );
	ASSERT_TRUE( string7[ 4 ] == 'B' );
	decUnicodeString string7b( string7 );
	ASSERT_TRUE( string7b.GetLength() == 5 );
	ASSERT_TRUE( string7b.GetAt( 0 ) == 'A' );
	ASSERT_TRUE( string7b.GetAt( 1 ) == 0xE4 );
	ASSERT_TRUE( string7b.GetAt( 2 ) == 0x20AC );
	ASSERT_TRUE( string7b.GetAt( 3 ) == 0x1D11E );
	ASSERT_TRUE( string7b.GetAt( 4 ) == 'B' );
	ASSERT_TRUE( string7b[ 0 ] == 'A' );
	ASSERT_TRUE( string7b[ 1 ] == 0xE4 );
	ASSERT_TRUE( string7b[ 2 ] == 0x20AC );
	ASSERT_TRUE( string7b[ 3 ] == 0x1D11E );
	ASSERT_TRUE( string7b[ 4 ] == 'B' );
	
	// string created from another unicode string
	string7.SetFromUTF8( "A\xC3""\xA4""\xE2""\x82""\xAC""\xF0""\x9D""\x84""\x9E" );
	ASSERT_TRUE( string7.GetLength() == 4 );
	ASSERT_TRUE( string7.GetAt( 0 ) == 'A' );
	ASSERT_TRUE( string7.GetAt( 1 ) == 0xE4 );
	ASSERT_TRUE( string7.GetAt( 2 ) == 0x20AC );
	ASSERT_TRUE( string7.GetAt( 3 ) == 0x1D11E );
	ASSERT_TRUE( string7[ 0 ] == 'A' );
	ASSERT_TRUE( string7[ 1 ] == 0xE4 );
	ASSERT_TRUE( string7[ 2 ] == 0x20AC );
	ASSERT_TRUE( string7[ 3 ] == 0x1D11E );
	decUnicodeString string7c( string7 );
	ASSERT_TRUE( string7c.GetLength() == 4 );
	ASSERT_TRUE( string7c.GetAt( 0 ) == 'A' );
	ASSERT_TRUE( string7c.GetAt( 1 ) == 0xE4 );
	ASSERT_TRUE( string7c.GetAt( 2 ) == 0x20AC );
	ASSERT_TRUE( string7c.GetAt( 3 ) == 0x1D11E );
	ASSERT_TRUE( string7c[ 0 ] == 'A' );
	ASSERT_TRUE( string7c[ 1 ] == 0xE4 );
	ASSERT_TRUE( string7c[ 2 ] == 0x20AC );
	ASSERT_TRUE( string7c[ 3 ] == 0x1D11E );
}

void detUnicodeString::TestAccessors(){
	SetSubTestNum( 2 );
}

void detUnicodeString::TestMutators(){
	decUnicodeString string1;
	SetSubTestNum( 3 );
	
	// set characters
	string1.SetFromUTF8( "A\xC3""\xA4""\xE2""\x82""\xAC""\xF0""\x9D""\x84""\x9E""B" );
	
	string1.SetAt( 1, 0x1D11E );
	ASSERT_TRUE( string1.GetLength() == 5 );
	ASSERT_TRUE( string1.GetAt( 0 ) == 'A' );
	ASSERT_TRUE( string1.GetAt( 1 ) == 0x1D11E );
	ASSERT_TRUE( string1.GetAt( 2 ) == 0x20AC );
	ASSERT_TRUE( string1.GetAt( 3 ) == 0x1D11E );
	ASSERT_TRUE( string1.GetAt( 4 ) == 'B' );
	
	string1.SetAt( 3, 0xE4 );
	ASSERT_TRUE( string1.GetLength() == 5 );
	ASSERT_TRUE( string1.GetAt( 0 ) == 'A' );
	ASSERT_TRUE( string1.GetAt( 1 ) == 0x1D11E );
	ASSERT_TRUE( string1.GetAt( 2 ) == 0x20AC );
	ASSERT_TRUE( string1.GetAt( 3 ) == 0xE4 );
	ASSERT_TRUE( string1.GetAt( 4 ) == 'B' );
	
	string1.SetAt( 2, 'a' );
	ASSERT_TRUE( string1.GetLength() == 5 );
	ASSERT_TRUE( string1.GetAt( 0 ) == 'A' );
	ASSERT_TRUE( string1.GetAt( 1 ) == 0x1D11E );
	ASSERT_TRUE( string1.GetAt( 2 ) == 'a' );
	ASSERT_TRUE( string1.GetAt( 3 ) == 0xE4 );
	ASSERT_TRUE( string1.GetAt( 4 ) == 'B' );
	
	string1.SetAt( 0, 'H' );
	string1.SetAt( 1, 'a' );
	string1.SetAt( 2, 'l' );
	string1.SetAt( 3, 'l' );
	string1.SetAt( 4, 'o' );
	ASSERT_TRUE( string1.GetLength() == 5 );
	ASSERT_TRUE( string1.GetAt( 0 ) == 'H' );
	ASSERT_TRUE( string1.GetAt( 1 ) == 'a' );
	ASSERT_TRUE( string1.GetAt( 2 ) == 'l' );
	ASSERT_TRUE( string1.GetAt( 3 ) == 'l' );
	ASSERT_TRUE( string1.GetAt( 4 ) == 'o' );
	
	string1.SetAt( 0, 'A' );
	string1.SetAt( 1, 0xE4 );
	string1.SetAt( 2, 0x20AC );
	string1.SetAt( 3, 0x1D11E );
	string1.SetAt( 4, 'B' );
	ASSERT_TRUE( string1.GetLength() == 5 );
	ASSERT_TRUE( string1.GetAt( 0 ) == 'A' );
	ASSERT_TRUE( string1.GetAt( 1 ) == 0xE4 );
	ASSERT_TRUE( string1.GetAt( 2 ) == 0x20AC );
	ASSERT_TRUE( string1.GetAt( 3 ) == 0x1D11E );
	ASSERT_TRUE( string1.GetAt( 4 ) == 'B' );
	
	
	
	// set string to another string, same length
	decUnicodeString string2 = decUnicodeString::NewFromUTF8( "g5fd 66hgj8 a" );
	string1.Set( string2 );
	ASSERT_EQUAL( string1, string2 );
	
	// set string to another string, shorter
	decUnicodeString string3 = decUnicodeString::NewFromUTF8( "fhh994 sf4" );
	string1.Set( string3 );
	ASSERT_EQUAL( string1, string3 );
	
	// set string to another string, longer
	decUnicodeString string4 = decUnicodeString::NewFromUTF8( "sh8373495 s883 5fhf743" );
	string1.Set( string4 );
	ASSERT_EQUAL( string1, string4 );
	
	// set string to another string, shorter
	decUnicodeString string5 = decUnicodeString::NewFromUTF8( "aa4" );
	string1.Set( string5 );
	ASSERT_EQUAL( string1, string5 );
	
	// set string to another string, empty
	decUnicodeString string6;
	string1.Set( string6 );
	ASSERT_EQUAL( string1, string6 );
	
	
	
	// set string to a run of characters
	string1.Set( 'h', 10 );
	ASSERT_EQUAL( string1, decUnicodeString::NewFromUTF8( "hhhhhhhhhh" ) );
	ASSERT_FALSE( string1.IsEmpty() );
	
	// set string to a run of no characters
	string1.Set( 'h', 0 );
	ASSERT_EQUAL( string1, decUnicodeString() );
	ASSERT_TRUE( string1.IsEmpty() );
}

void detUnicodeString::TestOperators(){
	decUnicodeString string1, string2, string3, string4;
	SetSubTestNum( 4 );

	// set string
	string1.SetFromUTF8( "Hallo\xE2\x82\xAC Welt" );
	string2 = string1;
	ASSERT_TRUE( string1.Compare( string2 ) == 0 );
	
	// test add unicode string
	string1.SetFromUTF8( "Hallo\xE2\x82\xAC Welt" );
	string2.SetFromUTF8( "Hallo Welt" );
	string3.SetFromUTF8( "Hallo\xE2\x82\xAC WeltHallo Welt" );
	ASSERT_TRUE( string3.Compare( string1 + string2 ) == 0 );
	string1 += string2;
	ASSERT_TRUE( string3.Compare( string1 ) == 0 );
	
	string1.SetFromUTF8( "Hallo\xE2\x82\xAC Welt" );
	string2.SetFromUTF8( "A\xC3\xA4\xE2\x82\xAC\xF0\x9D\x84\x9E""B" );
	string3.SetFromUTF8( "Hallo\xE2\x82\xAC WeltA\xC3\xA4\xE2\x82\xAC\xF0\x9D\x84\x9E""B" );
	ASSERT_TRUE( string3.Compare( string1 + string2 ) == 0 );
	string1 += string2;
	ASSERT_TRUE( string3.Compare( string1 ) == 0 );
	
	string1.SetFromUTF8( "Hallo" );
	string2.SetFromUTF8( " Welt" );
	string3.SetFromUTF8( "Hallo Welt" );
	ASSERT_TRUE( string3.Compare( string1 + string2 ) == 0 );
	string1 += string2;
	ASSERT_TRUE( string3.Compare( string1 ) == 0 );
	
	
	
	string1 = decUnicodeString();
	string2 = decUnicodeString::NewFromUTF8( "test string 1" );
	string3 = decUnicodeString::NewFromUTF8( "teAt string 1" );
	string4 = decUnicodeString::NewFromUTF8( "test striXg 1" );
	decUnicodeString string5 = decUnicodeString::NewFromUTF8( "6est string 1" );
	decUnicodeString string6 = decUnicodeString::NewFromUTF8( "test string #" );
	decUnicodeString string7 = decUnicodeString::NewFromUTF8( "test stzing 1" );
	
	// operator ! , is empty
	ASSERT_TRUE( string1.IsEmpty() );
	ASSERT_TRUE( ! string1 );
	
	string1 = string2;
	ASSERT_FALSE( string1.IsEmpty() );
	ASSERT_FALSE( ! string1 );
	
	// operator == , equality
	string1.Set( string2 );
	ASSERT_TRUE( string1 == string1 );
	ASSERT_TRUE( string1 == string2 );
	ASSERT_FALSE( string1 == string3 );
	ASSERT_FALSE( string1 == string4 );
	ASSERT_FALSE( string1 == string5 );
	ASSERT_FALSE( string1 == string6 );
	
	ASSERT_TRUE( string1 == string2 );
	ASSERT_FALSE( string1 == string3 );
	ASSERT_FALSE( string1 == string4 );
	ASSERT_FALSE( string1 == string5 );
	ASSERT_FALSE( string1 == string6 );
	
	// operator != , non-equality
	string1.Set( string2 );
	ASSERT_FALSE( string1 != string1 );
	ASSERT_FALSE( string1 != string2 );
	ASSERT_TRUE( string1 != string3 );
	ASSERT_TRUE( string1 != string4 );
	ASSERT_TRUE( string1 != string5 );
	ASSERT_TRUE( string1 != string6 );
	
	ASSERT_FALSE( string1 != string2 );
	ASSERT_TRUE( string1 != string3 );
	ASSERT_TRUE( string1 != string4 );
	ASSERT_TRUE( string1 != string5 );
	ASSERT_TRUE( string1 != string6 );
	
	// operator < , lexicographic less
	string1.Set( string2 );
	ASSERT_FALSE( string1 < string1 );
	ASSERT_FALSE( string1 < string2 );
	ASSERT_FALSE( string1 < string3 );
	ASSERT_FALSE( string1 < string4 );
	ASSERT_TRUE( string1 < string7 );
	ASSERT_TRUE( string3 < string1 );
	ASSERT_TRUE( string4 < string1 );
	
	ASSERT_FALSE( string1 < string2 );
	ASSERT_FALSE( string1 < string3 );
	ASSERT_FALSE( string1 < string4 );
	ASSERT_TRUE( string1 < string7 );
	
	// operator <= , lexicographic less or equal
	string1.Set( string2 );
	ASSERT_TRUE( string1 <= string1 );
	ASSERT_TRUE( string1 <= string2 );
	ASSERT_FALSE( string1 < string3 );
	ASSERT_FALSE( string1 < string4 );
	ASSERT_TRUE( string1 < string7 );
	ASSERT_TRUE( string3 < string1 );
	ASSERT_TRUE( string4 < string1 );
	
	ASSERT_TRUE( string1 <= string2 );
	ASSERT_FALSE( string1 < string3 );
	ASSERT_FALSE( string1 < string4 );
	ASSERT_TRUE( string1 < string7 );
	
	// operator > , lexicographic larger
	string1.Set( string2 );
	ASSERT_FALSE( string1 > string1 );
	ASSERT_FALSE( string1 > string2 );
	ASSERT_TRUE( string1 > string3 );
	ASSERT_TRUE( string1 > string4 );
	ASSERT_FALSE( string1 > string7 );
	ASSERT_FALSE( string3 > string1 );
	ASSERT_FALSE( string4 > string1 );
	
	ASSERT_FALSE( string1 > string2 );
	ASSERT_TRUE( string1 > string3 );
	ASSERT_TRUE( string1 > string4 );
	ASSERT_FALSE( string1 > string7 );
	
	// operator >= , lexicographic larger or equal
	string1.Set( string2 );
	ASSERT_TRUE( string1 >= string1 );
	ASSERT_TRUE( string1 >= string2 );
	ASSERT_TRUE( string1 >= string3 );
	ASSERT_TRUE( string1 >= string4 );
	ASSERT_FALSE( string1 >= string7 );
	ASSERT_FALSE( string3 >= string1 );
	ASSERT_FALSE( string4 >= string1 );
	
	ASSERT_TRUE( string1 >= string2 );
	ASSERT_TRUE( string1 >= string3 );
	ASSERT_TRUE( string1 >= string4 );
	ASSERT_FALSE( string1 >= string7 );
	
	// operator + , concatenate strings
	decUnicodeString string8 = decUnicodeString::NewFromUTF8( " hello world" );
	decUnicodeString string9 = decUnicodeString::NewFromUTF8( "test string 1 hello world" );
	
	string1.Set( string2 );
	ASSERT_TRUE( string1 + string8 == string9 );
	ASSERT_TRUE( string1 == string2 );
	
	string1.Set( string2 );
	ASSERT_TRUE( string1 + string8 == string9 );
	ASSERT_TRUE( string1 == string2 );
	
	string1.Set( string2 );
	ASSERT_TRUE( string1 + decUnicodeString() == string2 );
	ASSERT_TRUE( string1 == string2 );
	
	// operator = , assign string
	string1.Set( string2 );
	ASSERT_TRUE( string1 == string2 );
	string1 = string9;
	ASSERT_TRUE( string1 == string9 );
	
	string1.Set( string2 );
	ASSERT_TRUE( string1 == string2 );
	string1 = string9;
	ASSERT_TRUE( string1 == string9 );
	
	string1.Set( string2 );
	ASSERT_TRUE( string1 == string2 );
	string1 = decUnicodeString();
	ASSERT_TRUE( string1.IsEmpty() );
	
	// operator += , append string
	string1.Set( string2 );
	string1 += string8;
	ASSERT_TRUE( string1 == string9 );
	
	string1.Set( string2 );
	string1 += string8;
	ASSERT_TRUE( string1 == string9 );
	
	string1.Set( string2 );
	string1 += decUnicodeString();
	ASSERT_TRUE( string1 == string2 );
	
	
	
	// old tests
	// compare case sensitive
	string1.SetFromUTF8( "Hallo" );
	string2.SetFromUTF8( "Hallo" );
	ASSERT_TRUE( string1.Compare( string2 ) == 0 );
	ASSERT_TRUE( string1 == string2 );
	string1.SetFromUTF8( "Hallo\xE2\x82\xAC_Welt\xC2\xAE!" );
	string2.SetFromUTF8( "Hallo\xE2\x82\xAC_Welt\xC2\xAE!" );
	ASSERT_TRUE( string1.Compare( string2 ) == 0 );
	ASSERT_TRUE( string1 == string2 );
	string1.SetFromUTF8( "Hallo" );
	string2.SetFromUTF8( "HaLLo" );
	ASSERT_TRUE( string1.Compare( string2 ) > 0 );
	ASSERT_FALSE( string1 == string2 );
	string2.SetFromUTF8( "hallo" );
	ASSERT_TRUE( string1.Compare( string2 ) < 0 );
	ASSERT_FALSE( string1 == string2 );
	string2.SetFromUTF8( "Hal" );
	ASSERT_TRUE( string1.Compare( string2 ) > 0 );
	ASSERT_FALSE( string1 == string2 );
	string2.SetFromUTF8( "HalloWe" );
	ASSERT_TRUE( string1.Compare( string2 ) < 0 );
	ASSERT_FALSE( string1 == string2 );
	
	// compare case insensitive
	string1.SetFromUTF8( "Hallo" );
	string2.SetFromUTF8( "Hallo" );
	ASSERT_TRUE( string1.CompareInsensitive( string2 ) == 0 );
	string1.SetFromUTF8( "Hallo\xE2\x82\xAC_Welt\xC2\xAE!" );
	string2.SetFromUTF8( "Hallo\xE2\x82\xAC_Welt\xC2\xAE!" );
	ASSERT_TRUE( string1.CompareInsensitive( string2 ) == 0 );
	string1.SetFromUTF8( "Hallo" );
	string2.SetFromUTF8( "HaLLo" );
	ASSERT_TRUE( string1.CompareInsensitive( string2 ) == 0 );
	string2.SetFromUTF8( "HalIo" );
	ASSERT_TRUE( string1.CompareInsensitive( string2 ) > 0 );
	string2.SetFromUTF8( "Zallo" );
	ASSERT_TRUE( string1.CompareInsensitive( string2 ) < 0 );
	string2.SetFromUTF8( "Hal" );
	ASSERT_TRUE( string1.CompareInsensitive( string2 ) > 0 );
	string2.SetFromUTF8( "HalloWe" );
	ASSERT_TRUE( string1.CompareInsensitive( string2 ) < 0 );
	// test problematic tests
	string1.SetFromUTF8( "Hallo\xE2\x82\xAC\r\t \n\n " );
	string2.SetFromUTF8( "Hallo\xE2\x82\xAC\r\t \n\n " );
	ASSERT_TRUE( string1.CompareInsensitive( string2 ) == 0 );
}

void detUnicodeString::TestAppend(){
	SetSubTestNum( 5 );
	
	signed char valueC1 = 108;
	signed char valueC2 = -108;
	unsigned char valueUC = 239;
	short valueS1 = 12564;
	short valueS2 = -12564;
	unsigned short valueUS = 63139;
	int valueI1 = 2147281608;
	int valueI2 = -2147281608;
	unsigned int valueUI = 4192965065UL;
	float valueF = 5.234f;
	double valueD = 1.6654;
	decUnicodeString string1;
	decUnicodeString string2 = decUnicodeString::NewFromUTF8( "hallo" );
	decUnicodeString string3 = decUnicodeString::NewFromUTF8( " welt" );
	decUnicodeString string4 = decUnicodeString::NewFromUTF8( "hallo welt" );
	
	// append string
	string1.Set( string2 );
	string1.Append( string3 );
	ASSERT_TRUE( string1 == string4 );
	
	string1.Set( string2 );
	string1.Append( string3 );
	ASSERT_TRUE( string1 == string4 );
	
	// append a character
	string1.Set( string2 );
	string1.AppendCharacter( ( unsigned char )' ' );
	string1.AppendCharacter( ( signed char )'w' );
	string1.AppendCharacter( ( int )'e' );
	string1.AppendCharacter( ( unsigned char )'l' );
	string1.AppendCharacter( ( char )'t' );
	
	ASSERT_TRUE( string1 == string4 );
	// append value
	string1.Set( string2 );
	string1.AppendValue( valueC1 );
	ASSERT_TRUE( string1 == decUnicodeString::NewFromUTF8( "hallo108" ) );
	
	string1.Set( string2 );
	string1.AppendValue( valueC2 );
	ASSERT_TRUE( string1 == decUnicodeString::NewFromUTF8( "hallo-108" ) );
	
	string1.Set( string2 );
	string1.AppendValue( valueUC );
	ASSERT_TRUE( string1 == decUnicodeString::NewFromUTF8( "hallo239" ) );
	
	string1.Set( string2 );
	string1.AppendValue( valueS1 );
	ASSERT_TRUE( string1 == decUnicodeString::NewFromUTF8( "hallo12564" ) );
	
	string1.Set( string2 );
	string1.AppendValue( valueS2 );
	ASSERT_TRUE( string1 == decUnicodeString::NewFromUTF8( "hallo-12564" ) );
	
	string1.Set( string2 );
	string1.AppendValue( valueUS );
	ASSERT_TRUE( string1 == decUnicodeString::NewFromUTF8( "hallo63139" ) );
	
	string1.Set( string2 );
	string1.AppendValue( valueI1 );
	ASSERT_TRUE( string1 == decUnicodeString::NewFromUTF8( "hallo2147281608" ) );
	
	string1.Set( string2 );
	string1.AppendValue( valueI2 );
	ASSERT_TRUE( string1 == decUnicodeString::NewFromUTF8( "hallo-2147281608" ) );
	
	string1.Set( string2 );
	string1.AppendValue( valueUI );
	ASSERT_TRUE( string1 == decUnicodeString::NewFromUTF8( "hallo4192965065" ) );
	
	string1.Set( string2 );
	string1.AppendValue( valueF );
	ASSERT_TRUE( string1 == decUnicodeString::NewFromUTF8( "hallo5.234" ) );
	
	string1.Set( string2 );
	string1.AppendValue( valueD );
	ASSERT_TRUE( string1 == decUnicodeString::NewFromUTF8( "hallo1.6654" ) );
}

void detUnicodeString::TestConversion(){
	SetSubTestNum( 6 );
	
	ASSERT_TRUE( decUnicodeString::NewFromUTF8( "2147281608" ).ToInt() == 2147281608 );
	ASSERT_TRUE( decUnicodeString::NewFromUTF8( "-2147281608" ).ToInt() == -2147281608 );
	ASSERT_TRUE( fabsf( decUnicodeString::NewFromUTF8( "5.234" ).ToFloat() - 5.234f ) < 1e-6f );
	ASSERT_TRUE( fabs( decUnicodeString::NewFromUTF8( "1.6654" ).ToDouble() - 1.6654 ) < 1e-6 );
	
	
	
	// old tests
	// lower case
	decUnicodeString string1, string2, string3;
	string1.SetFromUTF8( "Hallo\xE2\x82\xAC Welt" );
	string2 = string1.GetLower();
	string3.SetFromUTF8( "hallo\xE2\x82\xAC welt" );
	ASSERT_TRUE( string2.Compare( string3 ) == 0 );
	
	// upper case
	string1.SetFromUTF8( "Hallo\xE2\x82\xAC Welt" );
	string2 = string1.GetUpper();
	string3.SetFromUTF8( "HALLO\xE2\x82\xAC WELT" );
	ASSERT_TRUE( string2.Compare( string3 ) == 0 );
	
	// to integer
	string1.SetFromUTF8( "120" );
	ASSERT_TRUE( string1.ToInt() == 120 );
	string1.SetFromUTF8( "0" );
	ASSERT_TRUE( string1.ToInt() == 0 );
	string1.SetFromUTF8( "" );
	ASSERT_TRUE( string1.ToInt() == 0 );
	string1.SetFromUTF8( "1234567890" );
	ASSERT_TRUE( string1.ToInt() == 1234567890 );
	string1.SetFromUTF8( "120hallo" );
	ASSERT_TRUE( string1.ToInt() == 120 );
	string1.SetFromUTF8( "-120" );
	ASSERT_TRUE( string1.ToInt() == -120 );
	string1.SetFromUTF8( "-1234567890" );
	ASSERT_TRUE( string1.ToInt() == -1234567890 );
	string1.SetFromUTF8( "-120Edf" );
	ASSERT_TRUE( string1.ToInt() == -120 );
	string1.SetFromUTF8( "Edf-120" );
	ASSERT_TRUE( string1.ToInt() == 0 );
	string1.SetFromUTF8( "Edf120" );
	ASSERT_TRUE( string1.ToInt() == 0 );
	string1.SetFromUTF8( "12345.67890" );
	ASSERT_TRUE( string1.ToInt() == 12345 );
	string1.SetFromUTF8( "-12345.67890" );
	ASSERT_TRUE( string1.ToInt() == -12345 );
	string1.SetFromUTF8( "+120" );
	ASSERT_TRUE( string1.ToInt() == 120 );
	string1.SetFromUTF8( "+1234567890" );
	ASSERT_TRUE( string1.ToInt() == 1234567890 );
	
	// to float
	string1.SetFromUTF8( "120" );
	ASSERT_TRUE( fabs( string1.ToFloat() - 120.0f ) < 0.00001 );
	string1.SetFromUTF8( "0" );
	ASSERT_TRUE( fabs( string1.ToFloat() ) < 0.00001 );
	string1.SetFromUTF8( "" );
	ASSERT_TRUE( fabs( string1.ToFloat() ) < 0.00001 );
	string1.SetFromUTF8( "12.45" );
	ASSERT_TRUE( fabs( string1.ToFloat() - 12.45f ) < 0.00001 );
	string1.SetFromUTF8( ".45" );
	ASSERT_TRUE( fabs( string1.ToFloat() - 0.45f ) < 0.00001 );
	string1.SetFromUTF8( "-120" );
	ASSERT_TRUE( fabs( string1.ToFloat() + 120.0f ) < 0.00001 );
	string1.SetFromUTF8( "-0" );
	ASSERT_TRUE( fabs( string1.ToFloat() ) < 0.00001 );
	string1.SetFromUTF8( "-12.45" );
	ASSERT_TRUE( fabs( string1.ToFloat() + 12.45f ) < 0.00001 );
	string1.SetFromUTF8( "-.45" );
	ASSERT_TRUE( fabs( string1.ToFloat() + 0.45f ) < 0.00001 );
	string1.SetFromUTF8( "120e10" );
	ASSERT_TRUE( fabs( string1.ToFloat() - 1.2e12f ) < 0.00001 );
	string1.SetFromUTF8( "-120e10" );
	ASSERT_TRUE( fabs( string1.ToFloat() + 1.2e12f ) < 0.00001 );
	string1.SetFromUTF8( "120e2" );
	ASSERT_TRUE( fabs( string1.ToFloat() - 1.2e4f ) < 0.00001 );
	string1.SetFromUTF8( "-120e2" );
	ASSERT_TRUE( fabs( string1.ToFloat() + 1.2e4f ) < 0.00001 );
	string1.SetFromUTF8( "120e-2" );
	ASSERT_TRUE( fabs( string1.ToFloat() - 1.2f ) < 0.00001 );
	string1.SetFromUTF8( "-120e-2" );
	ASSERT_TRUE( fabs( string1.ToFloat() + 1.2f ) < 0.00001 );
	string1.SetFromUTF8( "12.45e10" );
	ASSERT_TRUE( fabs( string1.ToFloat() - 1.245e11f ) < 0.00001 );
	string1.SetFromUTF8( "-12.45e10" );
	ASSERT_TRUE( fabs( string1.ToFloat() + 1.245e11f ) < 0.00001 );
	string1.SetFromUTF8( "12.45e2" );
	ASSERT_TRUE( fabs( string1.ToFloat() - 1.245e3f ) < 0.00001 );
	string1.SetFromUTF8( "-12.45e2" );
	ASSERT_TRUE( fabs( string1.ToFloat() + 1.245e3f ) < 0.00001 );
	string1.SetFromUTF8( "12.45e-2" );
	ASSERT_TRUE( fabs( string1.ToFloat() - 1.245e-1f ) < 0.00001 );
	string1.SetFromUTF8( "-12.45e-2" );
	ASSERT_TRUE( fabs( string1.ToFloat() + 1.245e-1f ) < 0.00001 );
	string1.SetFromUTF8( "+120" );
	ASSERT_TRUE( fabs( string1.ToFloat() - 120.0f ) < 0.00001 );
	string1.SetFromUTF8( "+12.45" );
	ASSERT_TRUE( fabs( string1.ToFloat() - 12.45f ) < 0.00001 );
	string1.SetFromUTF8( "+.45" );
	ASSERT_TRUE( fabs( string1.ToFloat() - 0.45f ) < 0.00001 );
	string1.SetFromUTF8( "+120e10" );
	ASSERT_TRUE( fabs( string1.ToFloat() - 1.2e12f ) < 0.00001 );
	string1.SetFromUTF8( "120e+10" );
	ASSERT_TRUE( fabs( string1.ToFloat() - 1.2e+12f ) < 0.00001 );
	string1.SetFromUTF8( "+120e+10" );
	ASSERT_TRUE( fabs( string1.ToFloat() - 1.2e+12f ) < 0.00001 );
	string1.SetFromUTF8( "+12.45e10" );
	ASSERT_TRUE( fabs( string1.ToFloat() - 1.245e11f ) < 0.00001 );
	string1.SetFromUTF8( "12.45e+10" );
	ASSERT_TRUE( fabs( string1.ToFloat() - 1.245e+11f ) < 0.00001 );
	string1.SetFromUTF8( "+12.45e+10" );
	ASSERT_TRUE( fabs( string1.ToFloat() - 1.245e+11f ) < 0.00001 );
	
	// to utf8 string
	string1.SetFromUTF8( "Hallo\xE2\x82\xAC Welt" );
	ASSERT_TRUE( string1.ToUTF8() == "Hallo\xE2\x82\xAC Welt" );
	
	string1.SetFromUTF8( "A\xC3\xA4\xE2\x82\xAC\xF0\x9D\x84\x9E""B" );
	ASSERT_TRUE( string1.ToUTF8() == "A\xC3\xA4\xE2\x82\xAC\xF0\x9D\x84\x9E""B" );
	
	string1.SetFromUTF8( "Hallo Welt" );
	ASSERT_TRUE( string1.ToUTF8() == "Hallo Welt" );
}

void detUnicodeString::TestFormat(){
	SetSubTestNum( 7 );
	
	decUnicodeString string1;
	
	// set using format
	string1.Format( "hello %s, %i all %s? %g", "world", 18456, "okay", 1.234f );
	ASSERT_TRUE( string1 == decUnicodeString::NewFromUTF8( "hello world, 18456 all okay? 1.234" ) );
	
	string1.Format( "hello world, 18456 all okay? 1.234" );
	ASSERT_TRUE( string1 == decUnicodeString::NewFromUTF8( "hello world, 18456 all okay? 1.234" ) );
	
	// append using format
	string1.Format( "hello %s, %i all %s? %g", "world", 18456, "okay", 1.234f );
	ASSERT_TRUE( string1 == decUnicodeString::NewFromUTF8( "hello world, 18456 all okay? 1.234" ) );
	string1.AppendFormat( ". i %s so, %g or %i!", "think", 6.54, 100 );
	ASSERT_TRUE( string1 == decUnicodeString::NewFromUTF8( "hello world, 18456 all okay? 1.234. i think so, 6.54 or 100!" ) );
	
	string1.Format( "hello world, 18456 all okay? 1.234" );
	ASSERT_TRUE( string1 == decUnicodeString::NewFromUTF8( "hello world, 18456 all okay? 1.234" ) );
	string1.AppendFormat( ". i think so, 6.54 or 100!" );
	ASSERT_TRUE( string1 == decUnicodeString::NewFromUTF8( "hello world, 18456 all okay? 1.234. i think so, 6.54 or 100!" ) );
	
	string1.Empty();
	string1.AppendFormat( "%s", "hello" );
	string1.AppendFormat( "%s", " world" );
	string1.AppendFormat( "%i!", 893 );
	ASSERT_TRUE( string1 == decUnicodeString::NewFromUTF8( "hello world893!" ) );
}

void detUnicodeString::TestFind(){
	SetSubTestNum( 8 );
	
	const decUnicodeString string1 = decUnicodeString::NewFromUTF8( "This is:a_test string:,containing_some,stuff.and\n\rothers" );
	const decUnicodeString string2 = decUnicodeString::NewFromUTF8( "This is:a_test string:,containing_some,stuff.and\n\rtest others" );
	const decUnicodeString findString1 = decUnicodeString::NewFromUTF8( "test" );
	const decUnicodeString findString2 = decUnicodeString::NewFromUTF8( "cOnT" );
	const decUnicodeString find3a = decUnicodeString::NewFromUTF8( ":_," );
	const decUnicodeString find3b = decUnicodeString::NewFromUTF8( ",_:" );
	const decUnicodeString find4a = decUnicodeString::NewFromUTF8( "\t@#" );
	const decUnicodeString find4b = decUnicodeString::NewFromUTF8( "#@\t" );
	const int find1 = ':';
	const int find2 = ';';
	
	// int Find( int character, int start, int end ) const;
	ASSERT_EQUAL( string1.Find( find1 ), 7 );
	ASSERT_EQUAL( string1.Find( find1, 3, 10 ), 7 );
	ASSERT_EQUAL( string1.Find( find1, -50, -30 ), 7 );
	ASSERT_EQUAL( string1.Find( find1, 3, -30 ), 7 );
	ASSERT_EQUAL( string1.Find( find1, 9, 30 ), 21 );
	ASSERT_EQUAL( string1.Find( find1, 10, 15 ), -1 );
	ASSERT_EQUAL( string1.Find( find1, -10, -5 ), -1 );
	ASSERT_EQUAL( string1.Find( find1, 7, 7 ), -1 );
	ASSERT_EQUAL( string1.Find( find1, 10, 5 ), -1 );
	ASSERT_EQUAL( string1.Find( find2 ), -1 );
	
	// int Find( const decUnicodeString &characters, int start, int end ) const;
	ASSERT_EQUAL( string1.Find( find3a ), 7 );
	ASSERT_EQUAL( string1.Find( find3a, 3, 10 ), 7 );
	ASSERT_EQUAL( string1.Find( find3a, -50, -30 ), 7 );
	ASSERT_EQUAL( string1.Find( find3a, 3, -30 ), 7 );
	ASSERT_EQUAL( string1.Find( find3a, 10, 30 ), 21 );
	ASSERT_EQUAL( string1.Find( find3a, 10, 15 ), -1 );
	ASSERT_EQUAL( string1.Find( find3a, -10, -5 ), -1 );
	ASSERT_EQUAL( string1.Find( find3a, 7, 7 ), -1 );
	ASSERT_EQUAL( string1.Find( find3a, 10, 5 ), -1 );
	ASSERT_EQUAL( string1.Find( find4a ), -1 );
	
	ASSERT_EQUAL( string1.Find( find3b ), 7 );
	ASSERT_EQUAL( string1.Find( find3b, 3, 10 ), 7 );
	ASSERT_EQUAL( string1.Find( find3b, -50, -30 ), 7 );
	ASSERT_EQUAL( string1.Find( find3b, 3, -30 ), 7 );
	ASSERT_EQUAL( string1.Find( find3b, 10, 30 ), 21 );
	ASSERT_EQUAL( string1.Find( find3b, 10, 15 ), -1 );
	ASSERT_EQUAL( string1.Find( find3b, -10, -5 ), -1 );
	ASSERT_EQUAL( string1.Find( find3b, 7, 7 ), -1 );
	ASSERT_EQUAL( string1.Find( find3b, 10, 5 ), -1 );
	ASSERT_EQUAL( string1.Find( find4b ), -1 );
	
	// int FindReverse( int character, int start, int end ) const;
	ASSERT_EQUAL( string1.FindReverse( find1 ), 21 );
	ASSERT_EQUAL( string1.FindReverse( find1, 3, 10 ), 7 );
	ASSERT_EQUAL( string1.FindReverse( find1, -50, -30 ), 21 );
	ASSERT_EQUAL( string1.FindReverse( find1, 3, -30 ), 21 );
	ASSERT_EQUAL( string1.FindReverse( find1, 9, 30 ), 21 );
	ASSERT_EQUAL( string1.FindReverse( find1, 10, 15 ), -1 );
	ASSERT_EQUAL( string1.FindReverse( find1, -10, -5 ), -1 );
	ASSERT_EQUAL( string1.FindReverse( find1, 7, 7 ), -1 );
	ASSERT_EQUAL( string1.FindReverse( find1, 10, 5 ), -1 );
	ASSERT_EQUAL( string1.FindReverse( find2 ), -1 );
	
	// int FindReverse( const decUnicodeString &characters, int start, int end ) const;
	ASSERT_EQUAL( string1.FindReverse( find3a ), 38 );
	ASSERT_EQUAL( string1.FindReverse( find3a, 3, 10 ), 9 );
	ASSERT_EQUAL( string1.FindReverse( find3a, -50, -30 ), 22 );
	ASSERT_EQUAL( string1.FindReverse( find3a, 3, -30 ), 22 );
	ASSERT_EQUAL( string1.FindReverse( find3a, 10, 30 ), 22 );
	ASSERT_EQUAL( string1.FindReverse( find3a, 10, 15 ), -1 );
	ASSERT_EQUAL( string1.FindReverse( find3a, -10, -5 ), -1 );
	ASSERT_EQUAL( string1.FindReverse( find3a, 7, 7 ), -1 );
	ASSERT_EQUAL( string1.FindReverse( find3a, 10, 5 ), -1 );
	ASSERT_EQUAL( string1.FindReverse( find4a ), -1 );
	
	ASSERT_EQUAL( string1.FindReverse( find3b ), 38 );
	ASSERT_EQUAL( string1.FindReverse( find3b, 3, 10 ), 9 );
	ASSERT_EQUAL( string1.FindReverse( find3b, -50, -30 ), 22 );
	ASSERT_EQUAL( string1.FindReverse( find3b, 3, -30 ), 22 );
	ASSERT_EQUAL( string1.FindReverse( find3b, 10, 30 ), 22 );
	ASSERT_EQUAL( string1.FindReverse( find3b, 10, 15 ), -1 );
	ASSERT_EQUAL( string1.FindReverse( find3b, -10, -5 ), -1 );
	ASSERT_EQUAL( string1.FindReverse( find3b, 7, 7 ), -1 );
	ASSERT_EQUAL( string1.FindReverse( find3b, 10, 5 ), -1 );
	ASSERT_EQUAL( string1.FindReverse( find4b ), -1 );
	
	// int FindString( const decUnicodeString &string, int start, int end ) const;
	ASSERT_EQUAL( string2.FindString( findString1 ), 10 );
	ASSERT_EQUAL( string2.FindString( findString1, 3, 10 ), -1 );
	ASSERT_EQUAL( string2.FindString( findString1, -51, -31 ), 10 );
	ASSERT_EQUAL( string2.FindString( findString1, 3, -30 ), 10 );
	ASSERT_EQUAL( string2.FindString( findString1, 9, 30 ), 10 );
	ASSERT_EQUAL( string2.FindString( findString1, 10, 15 ), 10 );
	ASSERT_EQUAL( string2.FindString( findString1, -11, -6 ), 50 );
	ASSERT_EQUAL( string2.FindString( findString1, 7, 7 ), -1 );
	ASSERT_EQUAL( string2.FindString( findString1, 10, 5 ), -1 );
	ASSERT_EQUAL( string2.FindString( findString2 ), -1 );
	
	// int FindStringReverse( const decUnicodeString &string, int start, int end ) const;
	ASSERT_EQUAL( string2.FindStringReverse( findString1 ), 50 );
	ASSERT_EQUAL( string2.FindStringReverse( findString1, 3, 10 ), -1 );
	ASSERT_EQUAL( string2.FindStringReverse( findString1, -51, -31 ), 10 );
	ASSERT_EQUAL( string2.FindStringReverse( findString1, 3, -30 ), 10 );
	ASSERT_EQUAL( string2.FindStringReverse( findString1, 9, 30 ), 10 );
	ASSERT_EQUAL( string2.FindStringReverse( findString1, 10, 15 ), 10 );
	ASSERT_EQUAL( string2.FindStringReverse( findString1, -11, -6 ), 50 );
	ASSERT_EQUAL( string2.FindStringReverse( findString1, 7, 7 ), -1 );
	ASSERT_EQUAL( string2.FindStringReverse( findString1, 10, 5 ), -1 );
	ASSERT_EQUAL( string2.FindStringReverse( findString2 ), -1 );
	
	
	
	// old tests
	decUnicodeString stringT = decUnicodeString::NewFromUTF8( "Hallo\xE2\x82\xAC_Welt\xC2\xAE!" );
	
	// forward search
	ASSERT_TRUE( stringT.Find( 'l', 0 ) == 2 );
	ASSERT_TRUE( stringT.Find( 'l', 2 ) == 2 );
	ASSERT_TRUE( stringT.Find( 'l', 3 ) == 3 );
	ASSERT_TRUE( stringT.Find( 'l', 4 ) == 9 );
	ASSERT_TRUE( stringT.Find( 'l', 9 ) == 9 );
	ASSERT_TRUE( stringT.Find( 'l', 10 ) == -1 );
	ASSERT_TRUE( stringT.Find( 'Z', 0 ) == -1 );
	ASSERT_TRUE( stringT.Find( 0x20AC, 0 ) == 5 );
	ASSERT_TRUE( stringT.Find( 0x20AC, 5 ) == 5 );
	ASSERT_TRUE( stringT.Find( 0x20AC, 6 ) == -1 );
	
	// backward search
	ASSERT_TRUE( stringT.FindReverse( 'l', 0 ) == 9 );
	ASSERT_TRUE( stringT.FindReverse( 'l', 0, -3 ) == 9 );
	ASSERT_TRUE( stringT.FindReverse( 'l', 0, -4 ) == 3 );
	ASSERT_TRUE( stringT.FindReverse( 'l', 0, -9 ) == 3 );
	ASSERT_TRUE( stringT.FindReverse( 'l', 0, -10 ) == 2 );
	ASSERT_TRUE( stringT.FindReverse( 'l', 0, -11 ) == -1 );
	ASSERT_TRUE( stringT.FindReverse( 'Z', 0 ) == -1 );
	ASSERT_TRUE( stringT.FindReverse( 0x20AC, 0 ) == 5 );
	ASSERT_TRUE( stringT.FindReverse( 0x20AC, 0, -7 ) == 5 );
	ASSERT_TRUE( stringT.FindReverse( 0x20AC, 0, -8 ) == -1 );
}

void detUnicodeString::TestSubString(){
	SetSubTestNum( 9 );
	
	const decUnicodeString string1 = decUnicodeString::NewFromUTF8( "This is a test string from which we grab some parts." );
	
	// decUnicodeString GetLeft( int count ) const;
	ASSERT_EQUAL( string1.GetLeft( 10 ), decUnicodeString::NewFromUTF8( "This is a " ) );
	ASSERT_EQUAL( string1.GetLeft( 100 ), string1 );
	ASSERT_EQUAL( string1.GetLeft( 0 ), decUnicodeString() );
	ASSERT_EQUAL( string1.GetLeft( -10 ), decUnicodeString() );
	
	// decUnicodeString GetRight( int count ) const;
	ASSERT_EQUAL( string1.GetRight( 10 ), decUnicodeString::NewFromUTF8( "ome parts." ) );
	ASSERT_EQUAL( string1.GetRight( 100 ), string1 );
	ASSERT_EQUAL( string1.GetRight( 0 ), decUnicodeString() );
	ASSERT_EQUAL( string1.GetRight( -10 ), decUnicodeString() );
	
	// decUnicodeString GetMiddle( int start, int end ) const;
	ASSERT_EQUAL( string1.GetMiddle( 10 ), decUnicodeString::NewFromUTF8( "test string from which we grab some parts." ) );
	ASSERT_EQUAL( string1.GetMiddle( 10, 20 ), decUnicodeString::NewFromUTF8( "test strin" ) );
	ASSERT_EQUAL( string1.GetMiddle( 10, 10 ), decUnicodeString() );
	ASSERT_EQUAL( string1.GetMiddle( -10 ), decUnicodeString::NewFromUTF8( "ome parts." ) );
	ASSERT_EQUAL( string1.GetMiddle( -100 ), string1 );
	ASSERT_EQUAL( string1.GetMiddle( -10, 10 ), decUnicodeString() );
	ASSERT_EQUAL( string1.GetMiddle( -10, -5 ), decUnicodeString::NewFromUTF8( "ome p" ) );
}

void detUnicodeString::TestReverse(){
	SetSubTestNum( 10 );
	
	decUnicodeString string1 = decUnicodeString::NewFromUTF8( "This is a test string." );
	decUnicodeString string2 = decUnicodeString::NewFromUTF8( ".gnirts tset a si sihT" );
	decUnicodeString string3;
	
	// void Reverse();
	string3 = string1;
	string3.Reverse();
	ASSERT_EQUAL( string3, string2 );
	string3.Reverse();
	ASSERT_EQUAL( string3, string1 );
	
	// decUnicodeString GetReversed() const;
	string3 = string1.GetReversed();
	ASSERT_EQUAL( string3, string2 );
	string3 = string3.GetReversed();
	ASSERT_EQUAL( string3, string1 );
}

void detUnicodeString::TestSplit(){
	SetSubTestNum( 11 );
	
	const decUnicodeString string1 = decUnicodeString::NewFromUTF8( "This is   a test" );
	const decUnicodeString string2 = decUnicodeString::NewFromUTF8( "  This is   a test" );
	const decUnicodeString string3 = decUnicodeString::NewFromUTF8( "This is   a test   " );
	const decUnicodeString string4 = decUnicodeString::NewFromUTF8( "  This is   a test   " );
	const decUnicodeString string5 = decUnicodeString::NewFromUTF8( "This_is_a_test" );
	const decUnicodeString string6 = decUnicodeString::NewFromUTF8( "   This_is_a_test" );
	const decUnicodeString string7 = decUnicodeString::NewFromUTF8( "This_is_a_test   " );
	const decUnicodeString string8 = decUnicodeString::NewFromUTF8( "   This_is_a_test   " );
	const decUnicodeString token1 = decUnicodeString::NewFromUTF8( "This" );
	const decUnicodeString token2 = decUnicodeString::NewFromUTF8( "is" );
	const decUnicodeString token3 = decUnicodeString::NewFromUTF8( "a" );
	const decUnicodeString token4 = decUnicodeString::NewFromUTF8( "test" );
	const decUnicodeString token5 = decUnicodeString::NewFromUTF8( "This_is_a_test" );
	const int split1 = ' ';
	
	decUnicodeStringList list;
	
	// decUnicodeStringList Split( int character ) const;
	list = string1.Split( split1 );
	ASSERT_EQUAL( list.GetCount(), 4 );
	ASSERT_EQUAL( list[ 0 ], token1 );
	ASSERT_EQUAL( list[ 1 ], token2 );
	ASSERT_EQUAL( list[ 2 ], token3 );
	ASSERT_EQUAL( list[ 3 ], token4 );
	
	list = string2.Split( split1 );
	ASSERT_EQUAL( list.GetCount(), 4 );
	ASSERT_EQUAL( list[ 0 ], token1 );
	ASSERT_EQUAL( list[ 1 ], token2 );
	ASSERT_EQUAL( list[ 2 ], token3 );
	ASSERT_EQUAL( list[ 3 ], token4 );
	
	list = string3.Split( split1 );
	ASSERT_EQUAL( list.GetCount(), 4 );
	ASSERT_EQUAL( list[ 0 ], token1 );
	ASSERT_EQUAL( list[ 1 ], token2 );
	ASSERT_EQUAL( list[ 2 ], token3 );
	ASSERT_EQUAL( list[ 3 ], token4 );
	
	list = string4.Split( split1 );
	ASSERT_EQUAL( list.GetCount(), 4 );
	ASSERT_EQUAL( list[ 0 ], token1 );
	ASSERT_EQUAL( list[ 1 ], token2 );
	ASSERT_EQUAL( list[ 2 ], token3 );
	ASSERT_EQUAL( list[ 3 ], token4 );
	
	list = string5.Split( split1 );
	ASSERT_EQUAL( list.GetCount(), 1 );
	ASSERT_EQUAL( list[ 0 ], token5 );
	
	list = string6.Split( split1 );
	ASSERT_EQUAL( list.GetCount(), 1 );
	ASSERT_EQUAL( list[ 0 ], token5 );
	
	list = string7.Split( split1 );
	ASSERT_EQUAL( list.GetCount(), 1 );
	ASSERT_EQUAL( list[ 0 ], token5 );
	
	list = string8.Split( split1 );
	ASSERT_EQUAL( list.GetCount(), 1 );
	ASSERT_EQUAL( list[ 0 ], token5 );
	
	
	
	// decUnicodeStringList Split( const char *characters ) const;
	const decUnicodeString string1b = decUnicodeString::NewFromUTF8( "This is \t \n a\ttest" );
	const decUnicodeString string2b = decUnicodeString::NewFromUTF8( " \t This is \t \n a\ttest" );
	const decUnicodeString string3b = decUnicodeString::NewFromUTF8( "This is \t \n a\ttest \t " );
	const decUnicodeString string4b = decUnicodeString::NewFromUTF8( " \t This is \t \n a\ttest \n " );
	const decUnicodeString string5b = decUnicodeString::NewFromUTF8( "This_is_a_test" );
	const decUnicodeString string6b = decUnicodeString::NewFromUTF8( " \t This_is_a_test" );
	const decUnicodeString string7b = decUnicodeString::NewFromUTF8( "This_is_a_test \t " );
	const decUnicodeString string8b = decUnicodeString::NewFromUTF8( " \t This_is_a_test \t " );
	const decUnicodeString split1b = decUnicodeString::NewFromUTF8( " \t\n\r" );
	
	list = string1b.Split( split1b );
	ASSERT_EQUAL( list.GetCount(), 4 );
	ASSERT_EQUAL( list[ 0 ], token1 );
	ASSERT_EQUAL( list[ 1 ], token2 );
	ASSERT_EQUAL( list[ 2 ], token3 );
	ASSERT_EQUAL( list[ 3 ], token4 );
	
	list = string2b.Split( split1b );
	ASSERT_EQUAL( list.GetCount(), 4 );
	ASSERT_EQUAL( list[ 0 ], token1 );
	ASSERT_EQUAL( list[ 1 ], token2 );
	ASSERT_EQUAL( list[ 2 ], token3 );
	ASSERT_EQUAL( list[ 3 ], token4 );
	
	list = string3b.Split( split1b );
	ASSERT_EQUAL( list.GetCount(), 4 );
	ASSERT_EQUAL( list[ 0 ], token1 );
	ASSERT_EQUAL( list[ 1 ], token2 );
	ASSERT_EQUAL( list[ 2 ], token3 );
	ASSERT_EQUAL( list[ 3 ], token4 );
	
	list = string4b.Split( split1b );
	ASSERT_EQUAL( list.GetCount(), 4 );
	ASSERT_EQUAL( list[ 0 ], token1 );
	ASSERT_EQUAL( list[ 1 ], token2 );
	ASSERT_EQUAL( list[ 2 ], token3 );
	ASSERT_EQUAL( list[ 3 ], token4 );
	
	list = string5b.Split( split1b );
	ASSERT_EQUAL( list.GetCount(), 1 );
	ASSERT_EQUAL( list[ 0 ], token5 );
	
	list = string6b.Split( split1b );
	ASSERT_EQUAL( list.GetCount(), 1 );
	ASSERT_EQUAL( list[ 0 ], token5 );
	
	list = string7b.Split( split1b );
	ASSERT_EQUAL( list.GetCount(), 1 );
	ASSERT_EQUAL( list[ 0 ], token5 );
	
	list = string8b.Split( split1b );
	ASSERT_EQUAL( list.GetCount(), 1 );
	ASSERT_EQUAL( list[ 0 ], token5 );
	
	
	
	// decUnicodeStringList Split( const decUnicodeString &characters ) const;
	list = string1b.Split( split1b );
	ASSERT_EQUAL( list.GetCount(), 4 );
	ASSERT_EQUAL( list[ 0 ], token1 );
	ASSERT_EQUAL( list[ 1 ], token2 );
	ASSERT_EQUAL( list[ 2 ], token3 );
	ASSERT_EQUAL( list[ 3 ], token4 );
	
	list = string2b.Split( split1b );
	ASSERT_EQUAL( list.GetCount(), 4 );
	ASSERT_EQUAL( list[ 0 ], token1 );
	ASSERT_EQUAL( list[ 1 ], token2 );
	ASSERT_EQUAL( list[ 2 ], token3 );
	ASSERT_EQUAL( list[ 3 ], token4 );
	
	list = string3b.Split( split1b );
	ASSERT_EQUAL( list.GetCount(), 4 );
	ASSERT_EQUAL( list[ 0 ], token1 );
	ASSERT_EQUAL( list[ 1 ], token2 );
	ASSERT_EQUAL( list[ 2 ], token3 );
	ASSERT_EQUAL( list[ 3 ], token4 );
	
	list = string4b.Split( split1b );
	ASSERT_EQUAL( list.GetCount(), 4 );
	ASSERT_EQUAL( list[ 0 ], token1 );
	ASSERT_EQUAL( list[ 1 ], token2 );
	ASSERT_EQUAL( list[ 2 ], token3 );
	ASSERT_EQUAL( list[ 3 ], token4 );
	
	list = string5b.Split( split1b );
	ASSERT_EQUAL( list.GetCount(), 1 );
	ASSERT_EQUAL( list[ 0 ], token5 );
	
	list = string6b.Split( split1b );
	ASSERT_EQUAL( list.GetCount(), 1 );
	ASSERT_EQUAL( list[ 0 ], token5 );
	
	list = string7b.Split( split1b );
	ASSERT_EQUAL( list.GetCount(), 1 );
	ASSERT_EQUAL( list[ 0 ], token5 );
	
	list = string8b.Split( split1b );
	ASSERT_EQUAL( list.GetCount(), 1 );
	ASSERT_EQUAL( list[ 0 ], token5 );
}

void detUnicodeString::TestReplace(){
	SetSubTestNum( 12 );
	
	const decUnicodeString string1 = decUnicodeString::NewFromUTF8( "This is a test." );
	const decUnicodeString string2 = decUnicodeString::NewFromUTF8( "Th@s @s a test." );
	const decUnicodeString string3 = decUnicodeString::NewFromUTF8( "Th@@ @@ a @e@@." );
	const decUnicodeString string4 = decUnicodeString::NewFromUTF8( "This is a test string with a some testing test to be replaced." );
	const decUnicodeString string5 = decUnicodeString::NewFromUTF8( "This is a XYZ string with a some XYZing XYZ to be replaced." );
	const decUnicodeString string6 = decUnicodeString::NewFromUTF8( "This is a  string with a some ing  to be replaced." );
	const decUnicodeString string7 = decUnicodeString::NewFromUTF8( "xxx" );
	const decUnicodeString string8 = decUnicodeString::NewFromUTF8( "XYZXYZXYZ" );
	const int replace1 = 'i';
	const int replace2 = 'x';
	const decUnicodeString replace3 = decUnicodeString::NewFromUTF8( "its" );
	const decUnicodeString replace4 = decUnicodeString::NewFromUTF8( "BoZ" );
	const decUnicodeString replace5 = decUnicodeString::NewFromUTF8( "test" );
	const decUnicodeString replace6 = decUnicodeString();
	const decUnicodeString replace7 = decUnicodeString::NewFromUTF8( "x" );
	const int with1 = '@';
	const decUnicodeString with2 = decUnicodeString::NewFromUTF8( "XYZ" );
	const decUnicodeString with3;
	decUnicodeString tstring;
	
	// void Replace( int replaceCharacter, int withCharacter );
	tstring = string1;
	tstring.Replace( replace1, with1 );
	ASSERT_EQUAL( tstring, string2 );
	
	tstring = string1;
	tstring.Replace( replace2, with1 );
	ASSERT_EQUAL( tstring, string1 );
	
	// void Replace( const decUnicodeString &replaceCharacters, int withCharacter );
	tstring = string1;
	tstring.Replace( replace3, with1 );
	ASSERT_EQUAL( tstring, string3 );
	
	tstring = string1;
	tstring.Replace( replace4, with1 );
	ASSERT_EQUAL( tstring, string1 );
	
	// void ReplaceString( const decUnicodeString &replaceString, const decUnicodeString &withString );
	tstring = string4;
	tstring.ReplaceString( replace5, with2 );
	ASSERT_EQUAL( tstring, string5 );
	
	tstring = string4;
	tstring.ReplaceString( replace5, with3 );
	ASSERT_EQUAL( tstring, string6 );
	
	tstring = string4;
	tstring.ReplaceString( replace6, with2 );
	ASSERT_EQUAL( tstring, string4 );
	
	// void ReplaceString( const decUnicodeString &replaceString, const decUnicodeString &withString );
	tstring = string4;
	tstring.ReplaceString( replace5, with2 );
	ASSERT_EQUAL( tstring, string5 );
	
	tstring = string4;
	tstring.ReplaceString( replace5, with3 );
	ASSERT_EQUAL( tstring, string6 );
	
	tstring = string4;
	tstring.ReplaceString( replace6, with2 );
	ASSERT_EQUAL( tstring, string4 );
	
	tstring = string7;
	tstring.ReplaceString( replace7, with2 );
	ASSERT_EQUAL( tstring, string8 );
	tstring.ReplaceString( replace7, with2 );
	ASSERT_EQUAL( tstring, string8 );
	
	// decUnicodeString GetReplaced( int replaceCharacter, int withCharacter ) const;
	ASSERT_EQUAL( string1.GetReplaced( replace1, with1 ), string2 );
	ASSERT_EQUAL( string1.GetReplaced( replace2, with1 ), string1 );
	
	// decUnicodeString GetReplaced( const decUnicodeString &replaceCharacters, int withCharacter ) const;
	ASSERT_EQUAL( string1.GetReplaced( replace3, with1 ), string3 );
	ASSERT_EQUAL( string1.GetReplaced( replace4, with1 ), string1 );
	
	// decUnicodeString GetReplacedString( const decUnicodeString &replaceString, const decUnicodeString &withString ) const;
	ASSERT_EQUAL( string4.GetReplacedString( replace5, with2 ), string5 );
	ASSERT_EQUAL( string4.GetReplacedString( replace5, with3 ), string6 );
	ASSERT_EQUAL( string4.GetReplacedString( replace6, with2 ), string4 );
	ASSERT_EQUAL( string7.GetReplacedString( replace7, with2 ), string8 );
	
	// void ReplaceString( const decUnicodeString &replaceString, const decUnicodeString &withString );
	ASSERT_EQUAL( string4.GetReplacedString( replace5, with2 ), string5 );
	ASSERT_EQUAL( string4.GetReplacedString( replace5, with3 ), string6 );
	ASSERT_EQUAL( string4.GetReplacedString( replace6, with2 ), string4 );
	ASSERT_EQUAL( string7.GetReplacedString( replace7, with2 ), string8 );
}

void detUnicodeString::TestTrim(){
	SetSubTestNum( 13 );
	
	const decUnicodeString string1 = decUnicodeString::NewFromUTF8( "Hallo\xE2\x82\xAC" );
	const decUnicodeString string2 = decUnicodeString::NewFromUTF8( " \t Hallo\xE2\x82\xAC" );
	const decUnicodeString string3 = decUnicodeString::NewFromUTF8( "Hallo\xE2\x82\xAC \t " );
	const decUnicodeString string4 = decUnicodeString::NewFromUTF8( " \t Hallo\xE2\x82\xAC \t " );
	decUnicodeString tstring;
	
	// void TrimLeft();
	tstring = string1;
	tstring.TrimLeft();
	ASSERT_EQUAL( tstring, string1 );
	
	tstring = string2;
	tstring.TrimLeft();
	ASSERT_EQUAL( tstring, string1 );
	
	tstring = string3;
	tstring.TrimLeft();
	ASSERT_EQUAL( tstring, string3 );
	
	tstring = string4;
	tstring.TrimLeft();
	ASSERT_EQUAL( tstring, string3 );
	
	// decString GetTrimmedLeft() const;
	ASSERT_EQUAL( string1.GetTrimmedLeft(), string1 );
	ASSERT_EQUAL( string2.GetTrimmedLeft(), string1 );
	ASSERT_EQUAL( string3.GetTrimmedLeft(), string3 );
	ASSERT_EQUAL( string4.GetTrimmedLeft(), string3 );
	
	// void TrimRight();
	tstring = string1;
	tstring.TrimRight();
	ASSERT_EQUAL( tstring, string1 );
	
	tstring = string2;
	tstring.TrimRight();
	ASSERT_EQUAL( tstring, string2 );
	
	tstring = string3;
	tstring.TrimRight();
	ASSERT_EQUAL( tstring, string1 );
	
	tstring = string4;
	tstring.TrimRight();
	ASSERT_EQUAL( tstring, string2 );
	
	// decString GetTrimmedRight() const;
	ASSERT_EQUAL( string1.GetTrimmedRight(), string1 );
	ASSERT_EQUAL( string2.GetTrimmedRight(), string2 );
	ASSERT_EQUAL( string3.GetTrimmedRight(), string1 );
	ASSERT_EQUAL( string4.GetTrimmedRight(), string2 );
	
	// void Trim();
	tstring = string1;
	tstring.Trim();
	ASSERT_EQUAL( tstring, string1 );
	
	tstring = string2;
	tstring.Trim();
	ASSERT_EQUAL( tstring, string1 );
	
	tstring = string3;
	tstring.Trim();
	ASSERT_EQUAL( tstring, string1 );
	
	tstring = string4;
	tstring.Trim();
	ASSERT_EQUAL( tstring, string1 );
	
	// decString GetTrimmed() const;
	ASSERT_EQUAL( string1.GetTrimmed(), string1 );
	ASSERT_EQUAL( string2.GetTrimmed(), string1 );
	ASSERT_EQUAL( string3.GetTrimmed(), string1 );
	ASSERT_EQUAL( string4.GetTrimmed(), string1 );
}

void detUnicodeString::TestLowerUpper(){
	SetSubTestNum( 14 );
	
	const decUnicodeString string1 = decUnicodeString::NewFromUTF8( "This @Is 2A [Test" );
	const decUnicodeString string2 = decUnicodeString::NewFromUTF8( "this @is 2a [test" );
	const decUnicodeString string3 = decUnicodeString::NewFromUTF8( "THIS @IS 2A [TEST" );
	decUnicodeString tstring;
	
	// void ToLower();
	tstring = string1;
	tstring.ToLower();
	ASSERT_EQUAL( tstring, string2 );
	
	tstring = string2;
	tstring.ToLower();
	ASSERT_EQUAL( tstring, string2 );
	
	// decString GetLower() const;
	ASSERT_EQUAL( string1.GetLower(), string2 );
	ASSERT_EQUAL( string2.GetLower(), string2 );
	
	// void ToUpper();
	tstring = string1;
	tstring.ToUpper();
	ASSERT_EQUAL( tstring, string3 );
	
	tstring = string3;
	tstring.ToUpper();
	ASSERT_EQUAL( tstring, string3 );
	
	// decString GetUpper() const;
	ASSERT_EQUAL( string1.GetUpper(), string3 );
	ASSERT_EQUAL( string3.GetUpper(), string3 );
}
