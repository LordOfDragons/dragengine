// include only once
#ifndef _DETTDICTIONARY_H_
#define _DETTDICTIONARY_H_

// includes
#include "../detCase.h"

// class detTDictionary
class detTDictionary : public detCase {
private:
	// Test methods for K=decString, V=int
	void TestStringIntBasic();
	void TestStringIntOperators();
	void TestStringIntKeysValues();

	// Test methods for K=decString, V=decXmlElementTag::Ref, VP=decXmlElementTag*
	void TestStringObjectRefBasic();
	void TestStringObjectRefOperations();

public:
	detTDictionary();
	~detTDictionary() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
};

#endif
