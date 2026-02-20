#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "detUnicodeLineBuffer.h"

#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeLineBuffer.h>
#include <dragengine/common/exceptions.h>



// Class detUnicodeLineBuffer
////////////////////////////////

// Constructors, destructor
////////////////////////////

detUnicodeLineBuffer::detUnicodeLineBuffer(){
	Prepare();
}

detUnicodeLineBuffer::~detUnicodeLineBuffer(){
	CleanUp();
}



// Testing
////////////

void detUnicodeLineBuffer::Prepare(){
}

void detUnicodeLineBuffer::Run(){
	TestConstructor();
	TestBufferSize();
	TestAddAndGet();
	TestClear();
	TestMultipleLines();
	TestFillLines();
	TestRingBuffer();
}

void detUnicodeLineBuffer::CleanUp(){
}

const char *detUnicodeLineBuffer::GetTestName(){
	return "UnicodeLineBuffer";
}



// Tests
//////////

void detUnicodeLineBuffer::TestConstructor(){
	SetSubTestNum(0);
	
	// decUnicodeLineBuffer(int initialSize)
	decUnicodeLineBuffer buffer1(5);
	ASSERT_EQUAL(buffer1.GetLineCount(), 0);
	ASSERT_EQUAL(buffer1.GetBufferSize(), 5);
	
	decUnicodeLineBuffer buffer2(10);
	ASSERT_EQUAL(buffer2.GetLineCount(), 0);
	ASSERT_EQUAL(buffer2.GetBufferSize(), 10);
}

void detUnicodeLineBuffer::TestBufferSize(){
	SetSubTestNum(1);
	
	decUnicodeLineBuffer buffer(5);
	
	// SetBufferSize / GetBufferSize
	buffer.SetBufferSize(10);
	ASSERT_EQUAL(buffer.GetBufferSize(), 10);
	ASSERT_EQUAL(buffer.GetLineCount(), 0);
	
	// Add some lines
	buffer.AddLine(decUnicodeString::NewFromUTF8("Line1"));
	buffer.AddLine(decUnicodeString::NewFromUTF8("Line2"));
	buffer.AddLine(decUnicodeString::NewFromUTF8("Line3"));
	ASSERT_EQUAL(buffer.GetLineCount(), 3);
	
	// Reduce buffer size (should keep newest lines based on comment)
	buffer.SetBufferSize(2);
	ASSERT_EQUAL(buffer.GetBufferSize(), 2);
	ASSERT_EQUAL(buffer.GetLineCount(), 2);
	ASSERT_EQUAL(buffer.GetLineAt(0), decUnicodeString::NewFromUTF8("Line2"));
	ASSERT_EQUAL(buffer.GetLineAt(1), decUnicodeString::NewFromUTF8("Line3"));
	
	// Increase buffer size
	buffer.SetBufferSize(5);
	ASSERT_EQUAL(buffer.GetBufferSize(), 5);
	ASSERT_EQUAL(buffer.GetLineCount(), 2);
	ASSERT_EQUAL(buffer.GetLineAt(0), decUnicodeString::NewFromUTF8("Line2"));
	ASSERT_EQUAL(buffer.GetLineAt(1), decUnicodeString::NewFromUTF8("Line3"));
}

void detUnicodeLineBuffer::TestAddAndGet(){
	SetSubTestNum(2);
	
	decUnicodeLineBuffer buffer(5);
	
	// AddLine / GetLineAt / GetLineCount
	buffer.AddLine(decUnicodeString::NewFromUTF8("First"));
	ASSERT_EQUAL(buffer.GetLineCount(), 1);
	ASSERT_EQUAL(buffer.GetLineAt(0), decUnicodeString::NewFromUTF8("First"));
	
	buffer.AddLine(decUnicodeString::NewFromUTF8("Second"));
	ASSERT_EQUAL(buffer.GetLineCount(), 2);
	ASSERT_EQUAL(buffer.GetLineAt(0), decUnicodeString::NewFromUTF8("First"));
	ASSERT_EQUAL(buffer.GetLineAt(1), decUnicodeString::NewFromUTF8("Second"));
	
	buffer.AddLine(decUnicodeString::NewFromUTF8("Third"));
	ASSERT_EQUAL(buffer.GetLineCount(), 3);
	ASSERT_EQUAL(buffer.GetLineAt(0), decUnicodeString::NewFromUTF8("First"));
	ASSERT_EQUAL(buffer.GetLineAt(1), decUnicodeString::NewFromUTF8("Second"));
	ASSERT_EQUAL(buffer.GetLineAt(2), decUnicodeString::NewFromUTF8("Third"));
}

void detUnicodeLineBuffer::TestClear(){
	SetSubTestNum(3);
	
	decUnicodeLineBuffer buffer(5);
	buffer.AddLine(decUnicodeString::NewFromUTF8("Line1"));
	buffer.AddLine(decUnicodeString::NewFromUTF8("Line2"));
	ASSERT_EQUAL(buffer.GetLineCount(), 2);
	
	// Clear
	buffer.Clear();
	ASSERT_EQUAL(buffer.GetLineCount(), 0);
	ASSERT_EQUAL(buffer.GetBufferSize(), 5);
	
	// Can add lines after clear
	buffer.AddLine(decUnicodeString::NewFromUTF8("NewLine"));
	ASSERT_EQUAL(buffer.GetLineCount(), 1);
	ASSERT_EQUAL(buffer.GetLineAt(0), decUnicodeString::NewFromUTF8("NewLine"));
}

void detUnicodeLineBuffer::TestMultipleLines(){
	SetSubTestNum(4);
	
	decUnicodeLineBuffer buffer(10);
	
	// AddMultipleLines with single line
	buffer.AddMultipleLines(decUnicodeString::NewFromUTF8("Single"));
	ASSERT_EQUAL(buffer.GetLineCount(), 1);
	ASSERT_EQUAL(buffer.GetLineAt(0), decUnicodeString::NewFromUTF8("Single"));
	
	buffer.Clear();
	
	// AddMultipleLines with multiple lines
	buffer.AddMultipleLines(decUnicodeString::NewFromUTF8("Line1\nLine2\nLine3"));
	ASSERT_EQUAL(buffer.GetLineCount(), 3);
	ASSERT_EQUAL(buffer.GetLineAt(0), decUnicodeString::NewFromUTF8("Line1"));
	ASSERT_EQUAL(buffer.GetLineAt(1), decUnicodeString::NewFromUTF8("Line2"));
	ASSERT_EQUAL(buffer.GetLineAt(2), decUnicodeString::NewFromUTF8("Line3"));
	
	buffer.Clear();
	
	// AddMultipleLines with trailing newline
	buffer.AddMultipleLines(decUnicodeString::NewFromUTF8("Line1\nLine2\n"));
	ASSERT_EQUAL(buffer.GetLineCount(), 3);
	ASSERT_EQUAL(buffer.GetLineAt(0), decUnicodeString::NewFromUTF8("Line1"));
	ASSERT_EQUAL(buffer.GetLineAt(1), decUnicodeString::NewFromUTF8("Line2"));
	ASSERT_EQUAL(buffer.GetLineAt(2), decUnicodeString());
	
	buffer.Clear();
	
	// AddMultipleLines with empty string
	buffer.AddMultipleLines(decUnicodeString());
	ASSERT_EQUAL(buffer.GetLineCount(), 0);
}

void detUnicodeLineBuffer::TestFillLines(){
	SetSubTestNum(5);
	
	decUnicodeLineBuffer buffer(10);
	decUnicodeString result;
	
	// FillLinesInto with empty buffer
	buffer.FillLinesInto(result);
	ASSERT_TRUE(result.IsEmpty());
	
	// FillLinesInto with single line
	buffer.AddLine(decUnicodeString::NewFromUTF8("Only"));
	buffer.FillLinesInto(result);
	ASSERT_EQUAL(result, decUnicodeString::NewFromUTF8("Only"));
	
	// FillLinesInto with multiple lines
	buffer.AddLine(decUnicodeString::NewFromUTF8("Second"));
	buffer.AddLine(decUnicodeString::NewFromUTF8("Third"));
	buffer.FillLinesInto(result);
	ASSERT_EQUAL(result, decUnicodeString::NewFromUTF8("Only\nSecond\nThird"));
}

void detUnicodeLineBuffer::TestRingBuffer(){
	SetSubTestNum(6);
	
	// Test ring buffer behavior when exceeding capacity
	decUnicodeLineBuffer buffer(3);
	
	buffer.AddLine(decUnicodeString::NewFromUTF8("Line1"));
	buffer.AddLine(decUnicodeString::NewFromUTF8("Line2"));
	buffer.AddLine(decUnicodeString::NewFromUTF8("Line3"));
	ASSERT_EQUAL(buffer.GetLineCount(), 3);
	ASSERT_EQUAL(buffer.GetLineAt(0), decUnicodeString::NewFromUTF8("Line1"));
	ASSERT_EQUAL(buffer.GetLineAt(1), decUnicodeString::NewFromUTF8("Line2"));
	ASSERT_EQUAL(buffer.GetLineAt(2), decUnicodeString::NewFromUTF8("Line3"));
	
	// Add one more (should discard oldest)
	buffer.AddLine(decUnicodeString::NewFromUTF8("Line4"));
	ASSERT_EQUAL(buffer.GetLineCount(), 3);
	ASSERT_EQUAL(buffer.GetLineAt(0), decUnicodeString::NewFromUTF8("Line2"));
	ASSERT_EQUAL(buffer.GetLineAt(1), decUnicodeString::NewFromUTF8("Line3"));
	ASSERT_EQUAL(buffer.GetLineAt(2), decUnicodeString::NewFromUTF8("Line4"));
	
	// Add more lines
	buffer.AddLine(decUnicodeString::NewFromUTF8("Line5"));
	buffer.AddLine(decUnicodeString::NewFromUTF8("Line6"));
	ASSERT_EQUAL(buffer.GetLineCount(), 3);
	ASSERT_EQUAL(buffer.GetLineAt(0), decUnicodeString::NewFromUTF8("Line4"));
	ASSERT_EQUAL(buffer.GetLineAt(1), decUnicodeString::NewFromUTF8("Line5"));
	ASSERT_EQUAL(buffer.GetLineAt(2), decUnicodeString::NewFromUTF8("Line6"));
}
