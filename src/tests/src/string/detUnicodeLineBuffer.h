// include only once
#ifndef _DETUNICODELINEBUFFER_H_
#define _DETUNICODELINEBUFFER_H_

// includes
#include "../detCase.h"

// class detUnicodeLineBuffer
class detUnicodeLineBuffer : public detCase{
private:
	void TestConstructor();
	void TestBufferSize();
	void TestAddAndGet();
	void TestClear();
	void TestMultipleLines();
	void TestFillLines();
	void TestRingBuffer();
	
public:
	detUnicodeLineBuffer();
	~detUnicodeLineBuffer() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
};

#endif
