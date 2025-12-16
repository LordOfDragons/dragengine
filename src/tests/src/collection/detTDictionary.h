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

	// Test methods for new functions in commit 1d5552384445f75f7cf67ed21a59f55c7127cae5
	void TestGetAtOrDefault();
	void TestVisit();
	void TestFind();
	void TestFindDefault();
	void TestCollect();
	void TestRemoveIf();
	void TestConstIterator();

public:
	detTDictionary();
	~detTDictionary() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
};

#endif
