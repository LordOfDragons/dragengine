// include only once
#ifndef _DETHELPERFUNCTIONS_H_
#define _DETHELPERFUNCTIONS_H_

// includes
#include "../detCase.h"

// class detHelperFunctions
class detHelperFunctions : public detCase {
private:
	// Test methods for DEJoin with decTList<decString>
	void TestDEJoinStringListBasic();
	void TestDEJoinStringListEmpty();
	void TestDEJoinStringListSeparator();
	void TestDEJoinStringListEmptyEntries();

	// Test methods for DEFindNamed
	void TestDEFindNamedFound();
	void TestDEFindNamedNotFound();
	
	// Comparators
	void TestComparators();

public:
	detHelperFunctions();
	~detHelperFunctions() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
};

#endif
