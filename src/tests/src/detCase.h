// include only once
#ifndef _DETCASE_H_
#define _DETCASE_H_



// equality threshold
#ifdef OS_BEOS
	#define THRESHOLD_EQUAL_FLOAT 2e-5
#else
	#define THRESHOLD_EQUAL_FLOAT 1e-5
#endif


// assert macros
#define ASSERT_TRUE(expr) \
	if(!(expr)) \
		DETHROW_INFO(deeTestFailed, "AssertTrue failed")

#define ASSERT_FALSE(expr) \
	if(expr) \
		DETHROW_INFO(deeTestFailed, "AssertFalse failed")

#define ASSERT_EQUAL(expr,val) \
	if((expr) != (val)) \
		DETHROW_INFO(deeTestFailed, "AssertEqual failed")

#define ASSERT_NEQUAL(expr,val) \
	if((expr) == (val)) \
		DETHROW_INFO(deeTestFailed, "AssertNotEqual failed")

#define ASSERT_FEQUAL(expr,val) \
	if(fabs((expr) - (val)) >= THRESHOLD_EQUAL_FLOAT) \
		DETHROW_INFO(deeTestFailed, "AssertFEqual failed")

#define ASSERT_NFEQUAL(expr,val) \
	if(fabs((expr) - (val)) < THRESHOLD_EQUAL_FLOAT) \
		DETHROW_INFO(deeTestFailed, "AssertNFEqual failed")

#define ASSERT_NOT_NULL(expr) \
	if(!(expr)) \
		DETHROW_INFO(deeTestFailed, "AssertNotNull failed")

#define ASSERT_NULL(expr) \
	if(expr) \
		DETHROW_INFO(deeTestFailed, "AssertNull failed")

#define ASSERT_NEW(expr) \
	if(!(expr)) \
		DETHROW(deeOutOfMemory)

#define ASSERT_DOES_FAIL(expr) \
	{\
		bool failed = false; \
		try{ \
			expr; \
		}catch(const deException &){ \
			failed = true; \
		} \
		if(!failed){ \
			DETHROW_INFO(deeTestFailed, "AssertDoesFail failed"); \
		} \
	}


// class
class detCase{
private:
	int pSubTest;
public:
	detCase();
	inline int GetSubTestNum() const{ return pSubTest; }
	virtual ~detCase(){}
	virtual void Prepare() = 0;
	virtual void Run() = 0;
	virtual void CleanUp() = 0;
	virtual const char *GetTestName() = 0;
protected:
	void SetSubTestNum(int Num);
};

// end of inlcude only once
#endif

