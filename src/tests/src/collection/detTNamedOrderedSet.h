// include only once
#ifndef _DETTNAMEDORDEREDSET_H_
#define _DETTNAMEDORDEREDSET_H_

// includes
#include "../detCase.h"

// predeclare classes
class deObject;

// class detTNamedOrderedSet
class detTNamedOrderedSet : public detCase {
private:
	void TestGetNamed();
	void TestIndexOfNamed();
	void TestHasNamed();
	void TestSetAt();
	void TestAdd();
	void TestAddIfAbsent();
	void TestInsert();
	
public:
	detTNamedOrderedSet();
	~detTNamedOrderedSet() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
};

// end of include only once
#endif
