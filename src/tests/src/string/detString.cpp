#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "detString.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/exceptions.h>



// Class detString
////////////////////

// Constructors, destructor
/////////////////////////////

detString::detString(){
	Prepare();
}

detString::~detString(){
	CleanUp();
}



// Testing
////////////

void detString::Prepare(){
}

void detString::Run(){
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

void detString::CleanUp(){
}

const char *detString::GetTestName(){
	return "String";
}



// Tests
//////////

void detString::TestConstructor(){
	SetSubTestNum(0);
	
	// empty string
	decString string1;
	ASSERT_TRUE(string1.GetLength() == 0);
	ASSERT_TRUE(string1.IsEmpty());
	
	// copy of a 0-terminated string pointer
	const char *orgstr1 = "test string 1";
	decString string2(orgstr1);
	ASSERT_TRUE(string2.GetLength() == (int)strlen(orgstr1));
	ASSERT_FALSE(string2.IsEmpty());
	ASSERT_TRUE(strcmp(string2.GetString(), orgstr1) == 0);
	
	// copy of another string
	decString string3(string2);
	ASSERT_TRUE(string3.GetLength() == (int)strlen(orgstr1));
	ASSERT_FALSE(string3.IsEmpty());
	ASSERT_TRUE(strcmp(string3.GetString(), orgstr1) == 0);
	
	// concatenation of two strings
	const char *orgstr2 = " hello world";
	decString string4(orgstr2);
	const char *orgstr3 = "test string 1 hello world";
	decString string5(string2, string4);
	ASSERT_TRUE(string5.GetLength() == (int)strlen(orgstr3));
	ASSERT_FALSE(string5.IsEmpty());
	ASSERT_TRUE(strcmp(string5.GetString(), orgstr3) == 0);
	
	decString string6(string2, orgstr2);
	ASSERT_TRUE(string6.GetLength() == (int)strlen(orgstr3));
	ASSERT_FALSE(string6.IsEmpty());
	ASSERT_TRUE(strcmp(string6.GetString(), orgstr3) == 0);
}

void detString::TestAccessors(){
	int i, len;
	
	SetSubTestNum(1);
	
	const char *orgstr1 = "test string 1";
	len = strlen(orgstr1);
	decString string(orgstr1);
	
	ASSERT_EQUAL(strcmp(string.GetString(), orgstr1), 0);
	for(i=0; i<len; i++){
		ASSERT_EQUAL(string.GetAt(i), (int)orgstr1[i]);
	}
	ASSERT_TRUE(string.GetString()[len] == '\0');
	
	for(i=0; i<len; i++){
		ASSERT_EQUAL(string[i], orgstr1[i]);
	}
}

void detString::TestMutators(){
	int i, len;
	
	SetSubTestNum(2);
	
	const char *orgstr1 = "test string 1";
	const char *orgstr2 = "teAt string 1";
	const char *orgstr3 = "teAt striXg 1";
	const char *orgstr4 = "6eAt striXg 1";
	const char *orgstr5 = "6eAt striXg #";
	const char *orgstr6 = "6eAt striX@ #";
	len = strlen(orgstr1);
	decString string1 = orgstr1;
	
	ASSERT_TRUE(strcmp(string1.GetString(), orgstr1) == 0);
	ASSERT_FALSE(string1.IsEmpty());
	for(i=0; i<len; i++){
		ASSERT_TRUE(string1.GetAt(i) == (int)orgstr1[i]);
	}
	ASSERT_TRUE(string1.GetString()[len] == '\0');
	
	// change a character next to the beginning
	string1.SetAt(2, 'A');
	ASSERT_TRUE(strcmp(string1.GetString(), orgstr2) == 0);
	ASSERT_FALSE(string1.IsEmpty());
	for(i=0; i<len; i++){
		ASSERT_TRUE(string1.GetAt(i) == (int)orgstr2[i]);
	}
	ASSERT_TRUE(string1.GetString()[len] == '\0');
	
	// change a character next to the end
	string1.SetAt(9, 'X');
	ASSERT_TRUE(strcmp(string1.GetString(), orgstr3) == 0);
	ASSERT_FALSE(string1.IsEmpty());
	for(i=0; i<len; i++){
		ASSERT_TRUE(string1.GetAt(i) == (int)orgstr3[i]);
	}
	ASSERT_TRUE(string1.GetString()[len] == '\0');
	
	// change a character at the beginning
	string1.SetAt(0, '6');
	ASSERT_TRUE(strcmp(string1.GetString(), orgstr4) == 0);
	ASSERT_FALSE(string1.IsEmpty());
	for(i=0; i<len; i++){
		ASSERT_TRUE(string1.GetAt(i) == (int)orgstr4[i]);
	}
	ASSERT_TRUE(string1.GetString()[len] == '\0');
	
	// change a character at the end
	string1.SetAt(len - 1, '#');
	ASSERT_TRUE(strcmp(string1.GetString(), orgstr5) == 0);
	ASSERT_FALSE(string1.IsEmpty());
	for(i=0; i<len; i++){
		ASSERT_TRUE(string1.GetAt(i) == (int)orgstr5[i]);
	}
	ASSERT_TRUE(string1.GetString()[len] == '\0');
	
	// change a character using negative index
	string1.SetAt(-3, '@');
	ASSERT_TRUE(strcmp(string1.GetString(), orgstr6) == 0);
	ASSERT_FALSE(string1.IsEmpty());
	for(i=0; i<len; i++){
		ASSERT_TRUE(string1.GetAt(i) == (int)orgstr6[i]);
	}
	ASSERT_TRUE(string1.GetString()[len] == '\0');
	
	
	// char &operator[]( int position );
	string1 = orgstr1;
	ASSERT_EQUAL(string1, orgstr1);
	string1[2] = 'A';
	ASSERT_EQUAL(string1, orgstr2);
	string1[9] = 'X';
	ASSERT_EQUAL(string1, orgstr3);
	string1[0] = '6';
	ASSERT_EQUAL(string1, orgstr4);
	string1[len - 1] = '#';
	ASSERT_EQUAL(string1, orgstr5);
	string1[-3] = '@';
	ASSERT_EQUAL(string1, orgstr6);
	
	
	
	// set string to another string, same length
	decString string2 = "g5fd 66hgj8 a";
	string1.Set(string2);
	ASSERT_EQUAL(string1, string2);
	
	// set string to another string, shorter
	decString string3 = "fhh994 sf4";
	string1.Set(string3);
	ASSERT_EQUAL(string1, string3);
	
	// set string to another string, longer
	decString string4 = "sh8373495 s883 5fhf743";
	string1.Set(string4);
	ASSERT_EQUAL(string1, string4);
	
	// set string to another string, shorter
	decString string5 = "aa4";
	string1.Set(string5);
	ASSERT_EQUAL(string1, string5);
	
	// set string to another string, empty
	decString string6;
	string1.Set(string6);
	ASSERT_EQUAL(string1, string6);
	
	
	
	// set string to a string pointer, same length
	string1.Set(orgstr5);
	string1.Set("g5fd 66hgj8 a");
	ASSERT_TRUE(strcmp(string1.GetString(), string2.GetString()) == 0);
	ASSERT_FALSE(string1.IsEmpty());
	
	// set string to another string, shorter
	string1.Set("fhh994 sf4");
	ASSERT_TRUE(strcmp(string1.GetString(), string3.GetString()) == 0);
	ASSERT_FALSE(string1.IsEmpty());
	
	// set string to another string, longer
	string1.Set("sh8373495 s883 5fhf743");
	ASSERT_TRUE(strcmp(string1.GetString(), string4.GetString()) == 0);
	ASSERT_FALSE(string1.IsEmpty());
	
	// set string to another string, shorter
	string1.Set("aa4");
	ASSERT_TRUE(strcmp(string1.GetString(), string5.GetString()) == 0);
	ASSERT_FALSE(string1.IsEmpty());
	
	// set string to another string, empty
	string1.Set("");
	ASSERT_TRUE(strcmp(string1.GetString(), string6.GetString()) == 0);
	ASSERT_TRUE(string1.IsEmpty());
	
	
	
	// set string to empty
	string1.Set(orgstr5);
	string1.Empty();
	ASSERT_TRUE(strcmp(string1.GetString(), "") == 0);
	ASSERT_TRUE(string1.IsEmpty());
	
	
	
	// set string to a run of characters
	const char *orgstr7 = "hhhhhhhhhh";
	string1.Set('h', 10);
	ASSERT_TRUE(strcmp(string1.GetString(), orgstr7) == 0);
	ASSERT_FALSE(string1.IsEmpty());
	
	// set string to a run of no characters
	string1.Set('h', 0);
	ASSERT_TRUE(strcmp(string1.GetString(), "") == 0);
	ASSERT_TRUE(string1.IsEmpty());
}

void detString::TestOperators(){
	//int i, len;
	
	SetSubTestNum(3);
	
	const char *orgstr1 = "test string 1";
	const char *orgstr2 = "teAt string 1";
	const char *orgstr3 = "test striXg 1";
	const char *orgstr4 = "6est string 1";
	const char *orgstr5 = "test string #";
	const char *orgstr6 = "test stzing 1";
	decString string1;
	decString string2(orgstr1);
	decString string3(orgstr2);
	decString string4(orgstr3);
	decString string5(orgstr4);
	decString string6(orgstr5);
	decString string7(orgstr6);
	
	// operator ! , is empty
	ASSERT_TRUE(string1.IsEmpty());
	ASSERT_TRUE(! string1);
	
	string1.Set(orgstr1);
	ASSERT_FALSE(string1.IsEmpty());
	ASSERT_FALSE(! string1);
	
	// const char *operator , get character pointer
	ASSERT_TRUE(strcmp((const char *)string1, orgstr1) == 0);
	
	// operator == , equality
	string1.Set(orgstr1);
	ASSERT_TRUE(string1 == string1);
	ASSERT_TRUE(string1 == string2);
	ASSERT_FALSE(string1 == string3);
	ASSERT_FALSE(string1 == string4);
	ASSERT_FALSE(string1 == string5);
	ASSERT_FALSE(string1 == string6);
	
	ASSERT_TRUE(string1 == orgstr1);
	ASSERT_FALSE(string1 == orgstr2);
	ASSERT_FALSE(string1 == orgstr3);
	ASSERT_FALSE(string1 == orgstr4);
	ASSERT_FALSE(string1 == orgstr5);
	
	// operator != , non-equality
	string1.Set(orgstr1);
	ASSERT_FALSE(string1 != string1);
	ASSERT_FALSE(string1 != string2);
	ASSERT_TRUE(string1 != string3);
	ASSERT_TRUE(string1 != string4);
	ASSERT_TRUE(string1 != string5);
	ASSERT_TRUE(string1 != string6);
	
	ASSERT_FALSE(string1 != orgstr1);
	ASSERT_TRUE(string1 != orgstr2);
	ASSERT_TRUE(string1 != orgstr3);
	ASSERT_TRUE(string1 != orgstr4);
	ASSERT_TRUE(string1 != orgstr5);
	
	// operator < , lexicographic less
	string1.Set(orgstr1);
	ASSERT_FALSE(string1 < string1);
	ASSERT_FALSE(string1 < string2);
	ASSERT_FALSE(string1 < string3);
	ASSERT_FALSE(string1 < string4);
	ASSERT_TRUE(string1 < string7);
	ASSERT_TRUE(string3 < string1);
	ASSERT_TRUE(string4 < string1);
	
	ASSERT_FALSE(string1 < orgstr1);
	ASSERT_FALSE(string1 < orgstr2);
	ASSERT_FALSE(string1 < orgstr3);
	ASSERT_TRUE(string1 < orgstr6);
	
	// operator <= , lexicographic less or equal
	string1.Set(orgstr1);
	ASSERT_TRUE(string1 <= string1);
	ASSERT_TRUE(string1 <= string2);
	ASSERT_FALSE(string1 < string3);
	ASSERT_FALSE(string1 < string4);
	ASSERT_TRUE(string1 < string7);
	ASSERT_TRUE(string3 < string1);
	ASSERT_TRUE(string4 < string1);
	
	ASSERT_TRUE(string1 <= orgstr1);
	ASSERT_FALSE(string1 < orgstr2);
	ASSERT_FALSE(string1 < orgstr3);
	ASSERT_TRUE(string1 < orgstr6);
	
	// operator > , lexicographic larger
	string1.Set(orgstr1);
	ASSERT_FALSE(string1 > string1);
	ASSERT_FALSE(string1 > string2);
	ASSERT_TRUE(string1 > string3);
	ASSERT_TRUE(string1 > string4);
	ASSERT_FALSE(string1 > string7);
	ASSERT_FALSE(string3 > string1);
	ASSERT_FALSE(string4 > string1);
	
	ASSERT_FALSE(string1 > orgstr1);
	ASSERT_TRUE(string1 > orgstr2);
	ASSERT_TRUE(string1 > orgstr3);
	ASSERT_FALSE(string1 > orgstr6);
	
	// operator >= , lexicographic larger or equal
	string1.Set(orgstr1);
	ASSERT_TRUE(string1 >= string1);
	ASSERT_TRUE(string1 >= string2);
	ASSERT_TRUE(string1 >= string3);
	ASSERT_TRUE(string1 >= string4);
	ASSERT_FALSE(string1 >= string7);
	ASSERT_FALSE(string3 >= string1);
	ASSERT_FALSE(string4 >= string1);
	
	ASSERT_TRUE(string1 >= orgstr1);
	ASSERT_TRUE(string1 >= orgstr2);
	ASSERT_TRUE(string1 >= orgstr3);
	ASSERT_FALSE(string1 >= orgstr6);
	
	// operator + , concatenate strings
	const char *orgstr7 =  " hello world";
	const char *orgstr8 = "test string 1 hello world";
	decString string8(orgstr7);
	decString string9(orgstr8);
	
	string1.Set(orgstr1);
	ASSERT_TRUE(string1 + string8 == string9);
	ASSERT_TRUE(string1 == string2);
	
	string1.Set(orgstr1);
	ASSERT_TRUE(string1 + orgstr7 == string9);
	ASSERT_TRUE(string1 == string2);
	
	string1.Set(orgstr1);
	ASSERT_TRUE(string1 + decString() == string2);
	ASSERT_TRUE(string1 == string2);
	
	string1.Set(orgstr1);
	ASSERT_TRUE(string1 + "" == string2);
	ASSERT_TRUE(string1 == string2);
	
	// operator = , assign string
	string1.Set(orgstr1);
	ASSERT_TRUE(string1 == string2);
	string1 = string9;
	ASSERT_TRUE(string1 == string9);
	
	string1.Set(orgstr1);
	ASSERT_TRUE(string1 == string2);
	string1 = orgstr8;
	ASSERT_TRUE(string1 == string9);
	
	string1.Set(orgstr1);
	ASSERT_TRUE(string1 == string2);
	string1 = decString();
	ASSERT_TRUE(string1.IsEmpty());
	
	string1.Set(orgstr1);
	ASSERT_TRUE(string1 == string2);
	string1 = "";
	ASSERT_TRUE(string1.IsEmpty());
	
	// operator += , append string
	string1.Set(orgstr1);
	string1 += string8;
	ASSERT_TRUE(string1 == string9);
	
	string1.Set(orgstr1);
	string1 += orgstr7;
	ASSERT_TRUE(string1 == string9);
	
	string1.Set(orgstr1);
	string1 += decString();
	ASSERT_TRUE(string1 == string2);
	
	string1.Set(orgstr1);
	string1 += "";
	ASSERT_TRUE(string1 == string2);
}

void detString::TestAppend(){
	SetSubTestNum(4);
	
	const char *orgstr1 = "hallo";
	const char *orgstr2 = " welt";
	const char *orgstr3 = "hallo welt";
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
	decString string1;
	decString string2(orgstr1);
	decString string3(orgstr2);
	
	// append string
	string1.Set(string2);
	string1.Append(string3);
	ASSERT_TRUE(string1 == orgstr3);
	
	string1.Set(string2);
	string1.Append(orgstr2);
	ASSERT_TRUE(string1 == orgstr3);
	
	// append a character
	string1.Set(string2);
	string1.AppendCharacter((unsigned char)' ');
	string1.AppendCharacter((signed char)'w');
	string1.AppendCharacter((int)'e');
	string1.AppendCharacter((unsigned char)'l');
	string1.AppendCharacter((signed char)'t');
	ASSERT_TRUE(string1 == orgstr3);
	
	// append value
	string1.Set(string2);
	string1.AppendValue(valueC1);
	ASSERT_TRUE(string1 == "hallo108");
	
	string1.Set(string2);
	string1.AppendValue(valueC2);
	ASSERT_TRUE(string1 == "hallo-108");
	
	string1.Set(string2);
	string1.AppendValue(valueUC);
	ASSERT_TRUE(string1 == "hallo239");
	
	string1.Set(string2);
	string1.AppendValue(valueS1);
	ASSERT_TRUE(string1 == "hallo12564");
	
	string1.Set(string2);
	string1.AppendValue(valueS2);
	ASSERT_TRUE(string1 == "hallo-12564");
	
	string1.Set(string2);
	string1.AppendValue(valueUS);
	ASSERT_TRUE(string1 == "hallo63139");
	
	string1.Set(string2);
	string1.AppendValue(valueI1);
	ASSERT_TRUE(string1 == "hallo2147281608");
	
	string1.Set(string2);
	string1.AppendValue(valueI2);
	ASSERT_TRUE(string1 == "hallo-2147281608");
	
	string1.Set(string2);
	string1.AppendValue(valueUI);
	ASSERT_TRUE(string1 == "hallo4192965065");
	
	string1.Set(string2);
	string1.AppendValue(valueF);
	ASSERT_TRUE(string1 == "hallo5.234");
	
	string1.Set(string2);
	string1.AppendValue(valueD);
	ASSERT_TRUE(string1 == "hallo1.6654");
	
}

void detString::TestConversion(){
	SetSubTestNum(5);
	
	ASSERT_TRUE(decString("2147281608").ToInt() == 2147281608);
	ASSERT_TRUE(decString("-2147281608").ToInt() == -2147281608);
	ASSERT_TRUE(fabsf(decString("5.234").ToFloat() - 5.234f) < 1e-6f);
	ASSERT_TRUE(fabs(decString("1.6654").ToDouble() - 1.6654) < 1e-6);
}

void detString::TestFormat(){
	SetSubTestNum(6);
	
	decString string1;
	
	// set using format
	string1.Format("hello %s, %i all %s? %g", "world", 18456, "okay", 1.234f);
	ASSERT_TRUE(string1 == "hello world, 18456 all okay? 1.234");
	
	string1.Format("hello world, 18456 all okay? 1.234");
	ASSERT_TRUE(string1 == "hello world, 18456 all okay? 1.234");
	
	// append using format
	string1.Format("hello %s, %i all %s? %g", "world", 18456, "okay", 1.234f);
	ASSERT_TRUE(string1 == "hello world, 18456 all okay? 1.234");
	string1.AppendFormat(". i %s so, %g or %i!", "think", 6.54, 100);
	ASSERT_TRUE(string1 == "hello world, 18456 all okay? 1.234. i think so, 6.54 or 100!");
	
	string1.Format("hello world, 18456 all okay? 1.234");
	ASSERT_TRUE(string1 == "hello world, 18456 all okay? 1.234");
	string1.AppendFormat(". i think so, 6.54 or 100!");
	ASSERT_TRUE(string1 == "hello world, 18456 all okay? 1.234. i think so, 6.54 or 100!");
	
	string1.Empty();
	string1.AppendFormat("%s", "hello");
	string1.AppendFormat("%s", " world");
	string1.AppendFormat("%i!", 893);
	ASSERT_TRUE(string1 == "hello world893!");
}

void detString::TestFind(){
	SetSubTestNum(7);
	
	const decString string1 = "This is:a_test string:,containing_some,stuff.and\n\rothers";
	const decString string2 = "This is:a_test string:,containing_some,stuff.and\n\rtest others";
	const decString findString1 = "test";
	const decString findString2 = "cOnT";
	const decString find3a = ":_,";
	const decString find3b = ",_:";
	const decString find4a = "\t@#";
	const decString find4b = "#@\t";
	const int find1 = ':';
	const int find2 = ';';
	
	// int Find( int character, int start, int end ) const;
	ASSERT_EQUAL(string1.Find(find1), 7);
	ASSERT_EQUAL(string1.Find(find1, 3, 10), 7);
	ASSERT_EQUAL(string1.Find(find1, -50, -30), 7);
	ASSERT_EQUAL(string1.Find(find1, 3, -30), 7);
	ASSERT_EQUAL(string1.Find(find1, 9, 30), 21);
	ASSERT_EQUAL(string1.Find(find1, 10, 15), -1);
	ASSERT_EQUAL(string1.Find(find1, -10, -5), -1);
	ASSERT_EQUAL(string1.Find(find1, 7, 7), -1);
	ASSERT_EQUAL(string1.Find(find1, 10, 5), -1);
	ASSERT_EQUAL(string1.Find(find2), -1);
	
	// int Find( const char *characters, int start, int end ) const;
	ASSERT_EQUAL(string1.Find(find3a.GetString()), 7);
	ASSERT_EQUAL(string1.Find(find3a.GetString(), 3, 10), 7);
	ASSERT_EQUAL(string1.Find(find3a.GetString(), -50, -30), 7);
	ASSERT_EQUAL(string1.Find(find3a.GetString(), 3, -30), 7);
	ASSERT_EQUAL(string1.Find(find3a.GetString(), 10, 30), 21);
	ASSERT_EQUAL(string1.Find(find3a.GetString(), 10, 15), -1);
	ASSERT_EQUAL(string1.Find(find3a.GetString(), -10, -5), -1);
	ASSERT_EQUAL(string1.Find(find3a.GetString(), 7, 7), -1);
	ASSERT_EQUAL(string1.Find(find3a.GetString(), 10, 5), -1);
	ASSERT_EQUAL(string1.Find(find4a.GetString()), -1);
	
	ASSERT_EQUAL(string1.Find(find3b.GetString()), 7);
	ASSERT_EQUAL(string1.Find(find3b.GetString(), 3, 10), 7);
	ASSERT_EQUAL(string1.Find(find3b.GetString(), -50, -30), 7);
	ASSERT_EQUAL(string1.Find(find3b.GetString(), 3, -30), 7);
	ASSERT_EQUAL(string1.Find(find3b.GetString(), 10, 30), 21);
	ASSERT_EQUAL(string1.Find(find3b.GetString(), 10, 15), -1);
	ASSERT_EQUAL(string1.Find(find3b.GetString(), -10, -5), -1);
	ASSERT_EQUAL(string1.Find(find3b.GetString(), 7, 7), -1);
	ASSERT_EQUAL(string1.Find(find3b.GetString(), 10, 5), -1);
	ASSERT_EQUAL(string1.Find(find4b.GetString()), -1);
	
	// int Find( const decString &characters, int start, int end ) const;
	ASSERT_EQUAL(string1.Find(find3a), 7);
	ASSERT_EQUAL(string1.Find(find3a, 3, 10), 7);
	ASSERT_EQUAL(string1.Find(find3a, -50, -30), 7);
	ASSERT_EQUAL(string1.Find(find3a, 3, -30), 7);
	ASSERT_EQUAL(string1.Find(find3a, 10, 30), 21);
	ASSERT_EQUAL(string1.Find(find3a, 10, 15), -1);
	ASSERT_EQUAL(string1.Find(find3a, -10, -5), -1);
	ASSERT_EQUAL(string1.Find(find3a, 7, 7), -1);
	ASSERT_EQUAL(string1.Find(find3a, 10, 5), -1);
	ASSERT_EQUAL(string1.Find(find4a), -1);
	
	ASSERT_EQUAL(string1.Find(find3b), 7);
	ASSERT_EQUAL(string1.Find(find3b, 3, 10), 7);
	ASSERT_EQUAL(string1.Find(find3b, -50, -30), 7);
	ASSERT_EQUAL(string1.Find(find3b, 3, -30), 7);
	ASSERT_EQUAL(string1.Find(find3b, 10, 30), 21);
	ASSERT_EQUAL(string1.Find(find3b, 10, 15), -1);
	ASSERT_EQUAL(string1.Find(find3b, -10, -5), -1);
	ASSERT_EQUAL(string1.Find(find3b, 7, 7), -1);
	ASSERT_EQUAL(string1.Find(find3b, 10, 5), -1);
	ASSERT_EQUAL(string1.Find(find4b), -1);
	
	// int FindReverse( int character, int start, int end ) const;
	ASSERT_EQUAL(string1.FindReverse(find1), 21);
	ASSERT_EQUAL(string1.FindReverse(find1, 3, 10), 7);
	ASSERT_EQUAL(string1.FindReverse(find1, -50, -30), 21);
	ASSERT_EQUAL(string1.FindReverse(find1, 3, -30), 21);
	ASSERT_EQUAL(string1.FindReverse(find1, 9, 30), 21);
	ASSERT_EQUAL(string1.FindReverse(find1, 10, 15), -1);
	ASSERT_EQUAL(string1.FindReverse(find1, -10, -5), -1);
	ASSERT_EQUAL(string1.FindReverse(find1, 7, 7), -1);
	ASSERT_EQUAL(string1.FindReverse(find1, 10, 5), -1);
	ASSERT_EQUAL(string1.FindReverse(find2), -1);
	
	// int FindReverse( const char *characters, int start, int end ) const;
	ASSERT_EQUAL(string1.FindReverse(find3a.GetString()), 38);
	ASSERT_EQUAL(string1.FindReverse(find3a.GetString(), 3, 10), 9);
	ASSERT_EQUAL(string1.FindReverse(find3a.GetString(), -50, -30), 22);
	ASSERT_EQUAL(string1.FindReverse(find3a.GetString(), 3, -30), 22);
	ASSERT_EQUAL(string1.FindReverse(find3a.GetString(), 10, 30), 22);
	ASSERT_EQUAL(string1.FindReverse(find3a.GetString(), 10, 15), -1);
	ASSERT_EQUAL(string1.FindReverse(find3a.GetString(), -10, -5), -1);
	ASSERT_EQUAL(string1.FindReverse(find3a.GetString(), 7, 7), -1);
	ASSERT_EQUAL(string1.FindReverse(find3a.GetString(), 10, 5), -1);
	ASSERT_EQUAL(string1.FindReverse(find4a.GetString()), -1);
	
	ASSERT_EQUAL(string1.FindReverse(find3b.GetString()), 38);
	ASSERT_EQUAL(string1.FindReverse(find3b.GetString(), 3, 10), 9);
	ASSERT_EQUAL(string1.FindReverse(find3b.GetString(), -50, -30), 22);
	ASSERT_EQUAL(string1.FindReverse(find3b.GetString(), 3, -30), 22);
	ASSERT_EQUAL(string1.FindReverse(find3b.GetString(), 10, 30), 22);
	ASSERT_EQUAL(string1.FindReverse(find3b.GetString(), 10, 15), -1);
	ASSERT_EQUAL(string1.FindReverse(find3b.GetString(), -10, -5), -1);
	ASSERT_EQUAL(string1.FindReverse(find3b.GetString(), 7, 7), -1);
	ASSERT_EQUAL(string1.FindReverse(find3b.GetString(), 10, 5), -1);
	ASSERT_EQUAL(string1.FindReverse(find4b.GetString()), -1);
	
	// int FindReverse( const decString &characters, int start, int end ) const;
	ASSERT_EQUAL(string1.FindReverse(find3a), 38);
	ASSERT_EQUAL(string1.FindReverse(find3a, 3, 10), 9);
	ASSERT_EQUAL(string1.FindReverse(find3a, -50, -30), 22);
	ASSERT_EQUAL(string1.FindReverse(find3a, 3, -30), 22);
	ASSERT_EQUAL(string1.FindReverse(find3a, 10, 30), 22);
	ASSERT_EQUAL(string1.FindReverse(find3a, 10, 15), -1);
	ASSERT_EQUAL(string1.FindReverse(find3a, -10, -5), -1);
	ASSERT_EQUAL(string1.FindReverse(find3a, 7, 7), -1);
	ASSERT_EQUAL(string1.FindReverse(find3a, 10, 5), -1);
	ASSERT_EQUAL(string1.FindReverse(find4a), -1);
	
	ASSERT_EQUAL(string1.FindReverse(find3b), 38);
	ASSERT_EQUAL(string1.FindReverse(find3b, 3, 10), 9);
	ASSERT_EQUAL(string1.FindReverse(find3b, -50, -30), 22);
	ASSERT_EQUAL(string1.FindReverse(find3b, 3, -30), 22);
	ASSERT_EQUAL(string1.FindReverse(find3b, 10, 30), 22);
	ASSERT_EQUAL(string1.FindReverse(find3b, 10, 15), -1);
	ASSERT_EQUAL(string1.FindReverse(find3b, -10, -5), -1);
	ASSERT_EQUAL(string1.FindReverse(find3b, 7, 7), -1);
	ASSERT_EQUAL(string1.FindReverse(find3b, 10, 5), -1);
	ASSERT_EQUAL(string1.FindReverse(find4b), -1);
	
	// int FindString( const char *string, int start, int end ) const;
	ASSERT_EQUAL(string2.FindString(findString1.GetString()), 10);
	ASSERT_EQUAL(string2.FindString(findString1.GetString(), 3, 10), -1);
	ASSERT_EQUAL(string2.FindString(findString1.GetString(), -51, -31), 10);
	ASSERT_EQUAL(string2.FindString(findString1.GetString(), 3, -30), 10);
	ASSERT_EQUAL(string2.FindString(findString1.GetString(), 9, 30), 10);
	ASSERT_EQUAL(string2.FindString(findString1.GetString(), 10, 15), 10);
	ASSERT_EQUAL(string2.FindString(findString1.GetString(), -11, -6), 50);
	ASSERT_EQUAL(string2.FindString(findString1.GetString(), 7, 7), -1);
	ASSERT_EQUAL(string2.FindString(findString1.GetString(), 10, 5), -1);
	ASSERT_EQUAL(string2.FindString(findString2.GetString()), -1);
	
	// int FindString( const decString &string, int start, int end ) const;
	ASSERT_EQUAL(string2.FindString(findString1), 10);
	ASSERT_EQUAL(string2.FindString(findString1, 3, 10), -1);
	ASSERT_EQUAL(string2.FindString(findString1, -51, -31), 10);
	ASSERT_EQUAL(string2.FindString(findString1, 3, -30), 10);
	ASSERT_EQUAL(string2.FindString(findString1, 9, 30), 10);
	ASSERT_EQUAL(string2.FindString(findString1, 10, 15), 10);
	ASSERT_EQUAL(string2.FindString(findString1, -11, -6), 50);
	ASSERT_EQUAL(string2.FindString(findString1, 7, 7), -1);
	ASSERT_EQUAL(string2.FindString(findString1, 10, 5), -1);
	ASSERT_EQUAL(string2.FindString(findString2), -1);
	
	// int FindStringReverse( const char *string, int start, int end ) const;
	ASSERT_EQUAL(string2.FindStringReverse(findString1.GetString()), 50);
	ASSERT_EQUAL(string2.FindStringReverse(findString1.GetString(), 3, 10), -1);
	ASSERT_EQUAL(string2.FindStringReverse(findString1.GetString(), -51, -31), 10);
	ASSERT_EQUAL(string2.FindStringReverse(findString1.GetString(), 3, -30), 10);
	ASSERT_EQUAL(string2.FindStringReverse(findString1.GetString(), 9, 30), 10);
	ASSERT_EQUAL(string2.FindStringReverse(findString1.GetString(), 10, 15), 10);
	ASSERT_EQUAL(string2.FindStringReverse(findString1.GetString(), -11, -6), 50);
	ASSERT_EQUAL(string2.FindStringReverse(findString1.GetString(), 7, 7), -1);
	ASSERT_EQUAL(string2.FindStringReverse(findString1.GetString(), 10, 5), -1);
	ASSERT_EQUAL(string2.FindStringReverse(findString2.GetString()), -1);
	
	// int FindStringReverse( const decString &string, int start, int end ) const;
	ASSERT_EQUAL(string2.FindStringReverse(findString1), 50);
	ASSERT_EQUAL(string2.FindStringReverse(findString1, 3, 10), -1);
	ASSERT_EQUAL(string2.FindStringReverse(findString1, -51, -31), 10);
	ASSERT_EQUAL(string2.FindStringReverse(findString1, 3, -30), 10);
	ASSERT_EQUAL(string2.FindStringReverse(findString1, 9, 30), 10);
	ASSERT_EQUAL(string2.FindStringReverse(findString1, 10, 15), 10);
	ASSERT_EQUAL(string2.FindStringReverse(findString1, -11, -6), 50);
	ASSERT_EQUAL(string2.FindStringReverse(findString1, 7, 7), -1);
	ASSERT_EQUAL(string2.FindStringReverse(findString1, 10, 5), -1);
	ASSERT_EQUAL(string2.FindStringReverse(findString2), -1);
}

void detString::TestSubString(){
	SetSubTestNum(8);
	
	const decString string1 = "This is a test string from which we grab some parts.";
	
	// decString GetLeft( int count ) const;
	ASSERT_EQUAL(string1.GetLeft(10), "This is a ");
	ASSERT_EQUAL(string1.GetLeft(100), string1);
	ASSERT_EQUAL(string1.GetLeft(0), "");
	ASSERT_EQUAL(string1.GetLeft(-10), "");
	
	// decString GetRight( int count ) const;
	ASSERT_EQUAL(string1.GetRight(10), "ome parts.");
	ASSERT_EQUAL(string1.GetRight(100), string1);
	ASSERT_EQUAL(string1.GetRight(0), "");
	ASSERT_EQUAL(string1.GetRight(-10), "");
	
	// decString GetMiddle( int start, int end ) const;
	ASSERT_EQUAL(string1.GetMiddle(10), "test string from which we grab some parts.");
	ASSERT_EQUAL(string1.GetMiddle(10, 20), "test strin");
	ASSERT_EQUAL(string1.GetMiddle(10, 10), "");
	ASSERT_EQUAL(string1.GetMiddle(-10), "ome parts.");
	ASSERT_EQUAL(string1.GetMiddle(-100), string1);
	ASSERT_EQUAL(string1.GetMiddle(-10, 10), "");
	ASSERT_EQUAL(string1.GetMiddle(-10, -5), "ome p");
}

void detString::TestReverse(){
	SetSubTestNum(9);
	
	decString string1 = "This is a test string.";
	decString string2 = ".gnirts tset a si sihT";
	decString string3;
	
	// void Reverse();
	string3 = string1;
	string3.Reverse();
	ASSERT_EQUAL(string3, string2);
	string3.Reverse();
	ASSERT_EQUAL(string3, string1);
	
	// decString GetReversed() const;
	string3 = string1.GetReversed();
	ASSERT_EQUAL(string3, string2);
	string3 = string3.GetReversed();
	ASSERT_EQUAL(string3, string1);
}

void detString::TestSplit(){
	SetSubTestNum(10);
	
	const decString string1 = "This is   a test";
	const decString string2 = "  This is   a test";
	const decString string3 = "This is   a test   ";
	const decString string4 = "  This is   a test   ";
	const decString string5 = "This_is_a_test";
	const decString string6 = "   This_is_a_test";
	const decString string7 = "This_is_a_test   ";
	const decString string8 = "   This_is_a_test   ";
	const decString token1 = "This";
	const decString token2 = "is";
	const decString token3 = "a";
	const decString token4 = "test";
	const decString token5 = "This_is_a_test";
	const int split1 = ' ';
	
	decStringList list;
	
	// decStringList Split( int character ) const;
	list = string1.Split(split1);
	ASSERT_EQUAL(list.GetCount(), 4);
	ASSERT_EQUAL(list[0], token1);
	ASSERT_EQUAL(list[1], token2);
	ASSERT_EQUAL(list[2], token3);
	ASSERT_EQUAL(list[3], token4);
	
	list = string2.Split(split1);
	ASSERT_EQUAL(list.GetCount(), 4);
	ASSERT_EQUAL(list[0], token1);
	ASSERT_EQUAL(list[1], token2);
	ASSERT_EQUAL(list[2], token3);
	ASSERT_EQUAL(list[3], token4);
	
	list = string3.Split(split1);
	ASSERT_EQUAL(list.GetCount(), 4);
	ASSERT_EQUAL(list[0], token1);
	ASSERT_EQUAL(list[1], token2);
	ASSERT_EQUAL(list[2], token3);
	ASSERT_EQUAL(list[3], token4);
	
	list = string4.Split(split1);
	ASSERT_EQUAL(list.GetCount(), 4);
	ASSERT_EQUAL(list[0], token1);
	ASSERT_EQUAL(list[1], token2);
	ASSERT_EQUAL(list[2], token3);
	ASSERT_EQUAL(list[3], token4);
	
	list = string5.Split(split1);
	ASSERT_EQUAL(list.GetCount(), 1);
	ASSERT_EQUAL(list[0], token5);
	
	list = string6.Split(split1);
	ASSERT_EQUAL(list.GetCount(), 1);
	ASSERT_EQUAL(list[0], token5);
	
	list = string7.Split(split1);
	ASSERT_EQUAL(list.GetCount(), 1);
	ASSERT_EQUAL(list[0], token5);
	
	list = string8.Split(split1);
	ASSERT_EQUAL(list.GetCount(), 1);
	ASSERT_EQUAL(list[0], token5);
	
	
	
	// decStringList Split( const char *characters ) const;
	const decString string1b = "This is \t \n a\ttest";
	const decString string2b = " \t This is \t \n a\ttest";
	const decString string3b = "This is \t \n a\ttest \t ";
	const decString string4b = " \t This is \t \n a\ttest \n ";
	const decString string5b = "This_is_a_test";
	const decString string6b = " \t This_is_a_test";
	const decString string7b = "This_is_a_test \t ";
	const decString string8b = " \t This_is_a_test \t ";
	const decString split1b = " \t\n\r";
	
	list = string1b.Split(split1b.GetString());
	ASSERT_EQUAL(list.GetCount(), 4);
	ASSERT_EQUAL(list[0], token1);
	ASSERT_EQUAL(list[1], token2);
	ASSERT_EQUAL(list[2], token3);
	ASSERT_EQUAL(list[3], token4);
	
	list = string2b.Split(split1b.GetString());
	ASSERT_EQUAL(list.GetCount(), 4);
	ASSERT_EQUAL(list[0], token1);
	ASSERT_EQUAL(list[1], token2);
	ASSERT_EQUAL(list[2], token3);
	ASSERT_EQUAL(list[3], token4);
	
	list = string3b.Split(split1b.GetString());
	ASSERT_EQUAL(list.GetCount(), 4);
	ASSERT_EQUAL(list[0], token1);
	ASSERT_EQUAL(list[1], token2);
	ASSERT_EQUAL(list[2], token3);
	ASSERT_EQUAL(list[3], token4);
	
	list = string4b.Split(split1b.GetString());
	ASSERT_EQUAL(list.GetCount(), 4);
	ASSERT_EQUAL(list[0], token1);
	ASSERT_EQUAL(list[1], token2);
	ASSERT_EQUAL(list[2], token3);
	ASSERT_EQUAL(list[3], token4);
	
	list = string5b.Split(split1b.GetString());
	ASSERT_EQUAL(list.GetCount(), 1);
	ASSERT_EQUAL(list[0], token5);
	
	list = string6b.Split(split1b.GetString());
	ASSERT_EQUAL(list.GetCount(), 1);
	ASSERT_EQUAL(list[0], token5);
	
	list = string7b.Split(split1b.GetString());
	ASSERT_EQUAL(list.GetCount(), 1);
	ASSERT_EQUAL(list[0], token5);
	
	list = string8b.Split(split1b.GetString());
	ASSERT_EQUAL(list.GetCount(), 1);
	ASSERT_EQUAL(list[0], token5);
	
	
	
	// decStringList Split( const decString &characters ) const;
	list = string1b.Split(split1b);
	ASSERT_EQUAL(list.GetCount(), 4);
	ASSERT_EQUAL(list[0], token1);
	ASSERT_EQUAL(list[1], token2);
	ASSERT_EQUAL(list[2], token3);
	ASSERT_EQUAL(list[3], token4);
	
	list = string2b.Split(split1b);
	ASSERT_EQUAL(list.GetCount(), 4);
	ASSERT_EQUAL(list[0], token1);
	ASSERT_EQUAL(list[1], token2);
	ASSERT_EQUAL(list[2], token3);
	ASSERT_EQUAL(list[3], token4);
	
	list = string3b.Split(split1b);
	ASSERT_EQUAL(list.GetCount(), 4);
	ASSERT_EQUAL(list[0], token1);
	ASSERT_EQUAL(list[1], token2);
	ASSERT_EQUAL(list[2], token3);
	ASSERT_EQUAL(list[3], token4);
	
	list = string4b.Split(split1b);
	ASSERT_EQUAL(list.GetCount(), 4);
	ASSERT_EQUAL(list[0], token1);
	ASSERT_EQUAL(list[1], token2);
	ASSERT_EQUAL(list[2], token3);
	ASSERT_EQUAL(list[3], token4);
	
	list = string5b.Split(split1b);
	ASSERT_EQUAL(list.GetCount(), 1);
	ASSERT_EQUAL(list[0], token5);
	
	list = string6b.Split(split1b);
	ASSERT_EQUAL(list.GetCount(), 1);
	ASSERT_EQUAL(list[0], token5);
	
	list = string7b.Split(split1b);
	ASSERT_EQUAL(list.GetCount(), 1);
	ASSERT_EQUAL(list[0], token5);
	
	list = string8b.Split(split1b);
	ASSERT_EQUAL(list.GetCount(), 1);
	ASSERT_EQUAL(list[0], token5);
}

void detString::TestReplace(){
	SetSubTestNum(11);
	
	const decString string1 = "This is a test.";
	const decString string2 = "Th@s @s a test.";
	const decString string3 = "Th@@ @@ a @e@@.";
	const decString string4 = "This is a test string with a some testing test to be replaced.";
	const decString string5 = "This is a XYZ string with a some XYZing XYZ to be replaced.";
	const decString string6 = "This is a  string with a some ing  to be replaced.";
	const decString string7 = "xxx";
	const decString string8 = "XYZXYZXYZ";
	const int replace1 = 'i';
	const int replace2 = 'x';
	const decString replace3 = "its";
	const decString replace4 = "BoZ";
	const decString replace5 = "test";
	const decString replace6 = "";
	const decString replace7 = "x";
	const int with1 = '@';
	const decString with2 = "XYZ";
	const decString with3 = "";
	decString tstring;
	
	// void Replace( int replaceCharacter, int withCharacter );
	tstring = string1;
	tstring.Replace(replace1, with1);
	ASSERT_EQUAL(tstring, string2);
	
	tstring = string1;
	tstring.Replace(replace2, with1);
	ASSERT_EQUAL(tstring, string1);
	
	// void Replace( const char *replaceCharacters, int withCharacter );
	tstring = string1;
	tstring.Replace(replace3.GetString(), with1);
	ASSERT_EQUAL(tstring, string3);
	
	tstring = string1;
	tstring.Replace(replace4.GetString(), with1);
	ASSERT_EQUAL(tstring, string1);
	
	// void Replace( const decString &replaceCharacters, int withCharacter );
	tstring = string1;
	tstring.Replace(replace3, with1);
	ASSERT_EQUAL(tstring, string3);
	
	tstring = string1;
	tstring.Replace(replace4, with1);
	ASSERT_EQUAL(tstring, string1);
	
	// void ReplaceString( const char *replaceString, const char *withString );
	tstring = string4;
	tstring.ReplaceString(replace5.GetString(), with2.GetString());
	ASSERT_EQUAL(tstring, string5);
	
	tstring = string4;
	tstring.ReplaceString(replace5.GetString(), with3.GetString());
	ASSERT_EQUAL(tstring, string6);
	
	tstring = string4;
	tstring.ReplaceString(replace6.GetString(), with2.GetString());
	ASSERT_EQUAL(tstring, string4);
	
	// void ReplaceString( const char *replaceString, const char *withString );
	tstring = string4;
	tstring.ReplaceString(replace5, with2);
	ASSERT_EQUAL(tstring, string5);
	
	tstring = string4;
	tstring.ReplaceString(replace5, with3);
	ASSERT_EQUAL(tstring, string6);
	
	tstring = string4;
	tstring.ReplaceString(replace6, with2);
	ASSERT_EQUAL(tstring, string4);
	
	tstring = string7;
	tstring.ReplaceString(replace7, with2);
	ASSERT_EQUAL(tstring, string8);
	tstring.ReplaceString(replace7.GetString(), with2.GetString());
	ASSERT_EQUAL(tstring, string8);
	
	// decString GetReplaced( int replaceCharacter, int withCharacter ) const;
	ASSERT_EQUAL(string1.GetReplaced(replace1, with1), string2);
	ASSERT_EQUAL(string1.GetReplaced(replace2, with1), string1);
	
	// decString GetReplaced( const char *replaceCharacters, int withCharacter ) const;
	ASSERT_EQUAL(string1.GetReplaced(replace3.GetString(), with1), string3);
	ASSERT_EQUAL(string1.GetReplaced(replace4.GetString(), with1), string1);
	
	// decString GetReplaced( const decString &replaceCharacters, int withCharacter ) const;
	ASSERT_EQUAL(string1.GetReplaced(replace3, with1), string3);
	ASSERT_EQUAL(string1.GetReplaced(replace4, with1), string1);
	
	// decString GetReplacedString( const char *replaceString, const char *withString ) const;
	ASSERT_EQUAL(string4.GetReplacedString(replace5.GetString(), with2.GetString()), string5);
	ASSERT_EQUAL(string4.GetReplacedString(replace5.GetString(), with3.GetString()), string6);
	ASSERT_EQUAL(string4.GetReplacedString(replace6.GetString(), with2.GetString()), string4);
	ASSERT_EQUAL(string7.GetReplacedString(replace7.GetString(), with2.GetString()), string8);
	
	// void ReplaceString( const char *replaceString, const char *withString );
	ASSERT_EQUAL(string4.GetReplacedString(replace5, with2), string5);
	ASSERT_EQUAL(string4.GetReplacedString(replace5, with3), string6);
	ASSERT_EQUAL(string4.GetReplacedString(replace6, with2), string4);
	ASSERT_EQUAL(string7.GetReplacedString(replace7, with2), string8);
}

void detString::TestTrim(){
	SetSubTestNum(12);
	
	const decString string1 = "This is \t \n a\ttest";
	const decString string2 = " \t This is \t \n a\ttest";
	const decString string3 = "This is \t \n a\ttest \t ";
	const decString string4 = " \t This is \t \n a\ttest \t ";
	decString tstring;
	
	// void TrimLeft();
	tstring = string1;
	tstring.TrimLeft();
	ASSERT_EQUAL(tstring, string1);
	
	tstring = string2;
	tstring.TrimLeft();
	ASSERT_EQUAL(tstring, string1);
	
	tstring = string3;
	tstring.TrimLeft();
	ASSERT_EQUAL(tstring, string3);
	
	tstring = string4;
	tstring.TrimLeft();
	ASSERT_EQUAL(tstring, string3);
	
	// decString GetTrimmedLeft() const;
	ASSERT_EQUAL(string1.GetTrimmedLeft(), string1);
	ASSERT_EQUAL(string2.GetTrimmedLeft(), string1);
	ASSERT_EQUAL(string3.GetTrimmedLeft(), string3);
	ASSERT_EQUAL(string4.GetTrimmedLeft(), string3);
	
	// void TrimRight();
	tstring = string1;
	tstring.TrimRight();
	ASSERT_EQUAL(tstring, string1);
	
	tstring = string2;
	tstring.TrimRight();
	ASSERT_EQUAL(tstring, string2);
	
	tstring = string3;
	tstring.TrimRight();
	ASSERT_EQUAL(tstring, string1);
	
	tstring = string4;
	tstring.TrimRight();
	ASSERT_EQUAL(tstring, string2);
	
	// decString GetTrimmedRight() const;
	ASSERT_EQUAL(string1.GetTrimmedRight(), string1);
	ASSERT_EQUAL(string2.GetTrimmedRight(), string2);
	ASSERT_EQUAL(string3.GetTrimmedRight(), string1);
	ASSERT_EQUAL(string4.GetTrimmedRight(), string2);
	
	// void Trim();
	tstring = string1;
	tstring.Trim();
	ASSERT_EQUAL(tstring, string1);
	
	tstring = string2;
	tstring.Trim();
	ASSERT_EQUAL(tstring, string1);
	
	tstring = string3;
	tstring.Trim();
	ASSERT_EQUAL(tstring, string1);
	
	tstring = string4;
	tstring.Trim();
	ASSERT_EQUAL(tstring, string1);
	
	// decString GetTrimmed() const;
	ASSERT_EQUAL(string1.GetTrimmed(), string1);
	ASSERT_EQUAL(string2.GetTrimmed(), string1);
	ASSERT_EQUAL(string3.GetTrimmed(), string1);
	ASSERT_EQUAL(string4.GetTrimmed(), string1);
}

void detString::TestLowerUpper(){
	SetSubTestNum(13);
	
	const decString string1 = "This @Is 2A [Test";
	const decString string2 = "this @is 2a [test";
	const decString string3 = "THIS @IS 2A [TEST";
	decString tstring;
	
	// void ToLower();
	tstring = string1;
	tstring.ToLower();
	ASSERT_EQUAL(tstring, string2);
	
	tstring = string2;
	tstring.ToLower();
	ASSERT_EQUAL(tstring, string2);
	
	// decString GetLower() const;
	ASSERT_EQUAL(string1.GetLower(), string2);
	ASSERT_EQUAL(string2.GetLower(), string2);
	
	// void ToUpper();
	tstring = string1;
	tstring.ToUpper();
	ASSERT_EQUAL(tstring, string3);
	
	tstring = string3;
	tstring.ToUpper();
	ASSERT_EQUAL(tstring, string3);
	
	// decString GetUpper() const;
	ASSERT_EQUAL(string1.GetUpper(), string3);
	ASSERT_EQUAL(string3.GetUpper(), string3);
}
