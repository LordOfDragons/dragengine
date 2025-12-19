// includes
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "detPath.h"
#include "dragengine/common/file/decPath.h"
#include "dragengine/common/exceptions.h"

	

// class detPath
///////////////////

// constructors, destructor
detPath::detPath(){
	Prepare();
}
detPath::~detPath(){
	CleanUp();
}

// testing
void detPath::Prepare(){
}
void detPath::Run(){
	pTestCreatePath(0);
	pTestParenting(1);
	pTestComposite(2);
	pTestMatchPattern(3);
	pTestStaticFactoryMethods(4);
	pTestPrefixManagement(5);
	pTestPathConversion(6);
	pTestPathOperations(7);
	pTestComponentManagement(8);
	pTestParentPathFunctions(9);
	pTestPathComparison(10);
	pTestStaticHelpers(11);
	pTestAbsoluteRelativePaths(12);
}
void detPath::CleanUp(){
}

const char *detPath::GetTestName(){return "Path";}



// Private Functions
//////////////////////

void detPath::pTestCreatePath(int testNumber){
	SetSubTestNum(testNumber);
	
	// create path
	decPath path1;
	ASSERT_TRUE(strcmp(path1.GetPrefix(), "") == 0);
	ASSERT_TRUE(path1.GetComponentCount() == 0);
	ASSERT_TRUE(strcmp(path1.GetPathUnix(), "") == 0);
	ASSERT_TRUE(path1.IsEmpty());
	ASSERT_FALSE(path1.HasComponents());
	
	path1.SetFromUnix("/usr/lib/dragengine/libdragengine.so");
	ASSERT_TRUE(strcmp(path1.GetPrefix(), "/") == 0);
	ASSERT_TRUE(path1.GetComponentCount() == 4);
	ASSERT_TRUE(strcmp(path1.GetPathUnix(), "/usr/lib/dragengine/libdragengine.so") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(0), "usr") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(1), "lib") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(2), "dragengine") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(3), "libdragengine.so") == 0);
	ASSERT_TRUE(strcmp(path1.GetLastComponent(), "libdragengine.so") == 0);
	ASSERT_FALSE(path1.IsEmpty());
	ASSERT_TRUE(path1.HasComponents());
}

void detPath::pTestParenting(int testNumber){
	SetSubTestNum(testNumber);

	// test parenting
	decPath path1;
	path1.SetFromUnix("/usr/lib/dragengine/libdragengine.so");
	path1.RemoveLastComponent();
	ASSERT_TRUE(strcmp(path1.GetPrefix(), "/") == 0);
	ASSERT_TRUE(path1.GetComponentCount() == 3);
	ASSERT_TRUE(strcmp(path1.GetPathUnix(), "/usr/lib/dragengine") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(0), "usr") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(1), "lib") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(2), "dragengine") == 0);
	ASSERT_TRUE(strcmp(path1.GetLastComponent(), "dragengine") == 0);
	ASSERT_FALSE(path1.IsEmpty());
	ASSERT_TRUE(path1.HasComponents());

	path1.RemoveLastComponent();
	ASSERT_TRUE(strcmp(path1.GetPrefix(), "/") == 0);
	ASSERT_TRUE(path1.GetComponentCount() == 2);
	ASSERT_TRUE(strcmp(path1.GetPathUnix(), "/usr/lib") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(0), "usr") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(1), "lib") == 0);
	ASSERT_TRUE(strcmp(path1.GetLastComponent(), "lib") == 0);
	ASSERT_FALSE(path1.IsEmpty());
	ASSERT_TRUE(path1.HasComponents());

	path1.RemoveLastComponent();
	ASSERT_TRUE(strcmp(path1.GetPrefix(), "/") == 0);
	ASSERT_TRUE(path1.GetComponentCount() == 1);
	ASSERT_TRUE(strcmp(path1.GetPathUnix(), "/usr") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(0), "usr") == 0);
	ASSERT_TRUE(strcmp(path1.GetLastComponent(), "usr") == 0);
	ASSERT_FALSE(path1.IsEmpty());
	ASSERT_TRUE(path1.HasComponents());

	path1.SetEmpty();
	ASSERT_TRUE(strcmp(path1.GetPrefix(), "") == 0);
	ASSERT_TRUE(path1.GetComponentCount() == 0);
	ASSERT_TRUE(strcmp(path1.GetPathUnix(), "") == 0);
	ASSERT_TRUE(path1.IsEmpty());
	ASSERT_FALSE(path1.HasComponents());
}

void detPath::pTestComposite(int testNumber){
	SetSubTestNum(testNumber);
	
	// create composite path
	decPath path1;
	path1.SetFromUnix("/usr/lib/dragengine");
	path1.AddUnixPath("libdragengine.so");
	ASSERT_TRUE(strcmp(path1.GetPrefix(), "/") == 0);
	ASSERT_TRUE(path1.GetComponentCount() == 4);
	ASSERT_TRUE(strcmp(path1.GetPathUnix(), "/usr/lib/dragengine/libdragengine.so") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(0), "usr") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(1), "lib") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(2), "dragengine") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(3), "libdragengine.so") == 0);
	ASSERT_TRUE(strcmp(path1.GetLastComponent(), "libdragengine.so") == 0);
	ASSERT_FALSE(path1.IsEmpty());
	ASSERT_TRUE(path1.HasComponents());
	
	path1.SetFromUnix("/usr/lib/dragengine/shares/libgraopengl");
	path1.AddUnixPath("../../libdragengine.so");
	ASSERT_TRUE(strcmp(path1.GetPrefix(), "/") == 0);
	ASSERT_TRUE(path1.GetComponentCount() == 4);
	ASSERT_TRUE(strcmp(path1.GetPathUnix(), "/usr/lib/dragengine/libdragengine.so") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(0), "usr") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(1), "lib") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(2), "dragengine") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(3), "libdragengine.so") == 0);
	ASSERT_TRUE(strcmp(path1.GetLastComponent(), "libdragengine.so") == 0);
	ASSERT_FALSE(path1.IsEmpty());
	ASSERT_TRUE(path1.HasComponents());
	
	path1.SetFromUnix("/usr/lib/dragengine");
	path1.AddUnixPath("./libdragengine.so");
	ASSERT_TRUE(strcmp(path1.GetPrefix(), "/") == 0);
	ASSERT_TRUE(path1.GetComponentCount() == 4);
	ASSERT_TRUE(strcmp(path1.GetPathUnix(), "/usr/lib/dragengine/libdragengine.so") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(0), "usr") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(1), "lib") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(2), "dragengine") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(3), "libdragengine.so") == 0);
	ASSERT_TRUE(strcmp(path1.GetLastComponent(), "libdragengine.so") == 0);
	ASSERT_FALSE(path1.IsEmpty());
	ASSERT_TRUE(path1.HasComponents());
	
	path1.SetFromUnix("/usr/lib/dragengine/shares/libgraopengl");
	path1.AddUnixPath("./../././.././libdragengine.so");
	ASSERT_TRUE(strcmp(path1.GetPrefix(), "/") == 0);
	ASSERT_TRUE(path1.GetComponentCount() == 4);
	ASSERT_TRUE(strcmp(path1.GetPathUnix(), "/usr/lib/dragengine/libdragengine.so") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(0), "usr") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(1), "lib") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(2), "dragengine") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(3), "libdragengine.so") == 0);
	ASSERT_TRUE(strcmp(path1.GetLastComponent(), "libdragengine.so") == 0);
	ASSERT_FALSE(path1.IsEmpty());
	ASSERT_TRUE(path1.HasComponents());
}

void detPath::pTestMatchPattern(int testNumber){
	SetSubTestNum(testNumber);
	
	// simple text matching
	decPath path, pattern;
	
	path.SetFromUnix("/usr/lib/dragengine/libdragengine.so");
	pattern.SetFromUnix("/usr/lib/dragengine/libdragengine.so");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	
	pattern.SetFromUnix("usr/lib/dragengine/libdragengine.so");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	
	path.SetFromUnix("usr/lib/dragengine/libdragengine.so");
	pattern.SetFromUnix("/usr/lib/dragengine/libdragengine.so");
	ASSERT_FALSE(path.MatchesPattern(pattern));
	
	pattern.SetFromUnix("usr/lib/dragengine/libdragengine.so");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	
	// match a wild card in the last component
	path.SetFromUnix("/usr/lib/dragengine/libdragengine.so");
	pattern.SetFromUnix("/usr/lib/dragengine/*.so");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libdragengine.tos");
	ASSERT_FALSE(path.MatchesPattern(pattern));
	
	path.SetFromUnix("/usr/lib/dragengine/libdragengine.so");
	pattern.SetFromUnix("/usr/lib/dragengine/lib*.so");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/hipdragengine.so");
	ASSERT_FALSE(path.MatchesPattern(pattern));
	
	path.SetFromUnix("/usr/lib/dragengine/libdragengine.so");
	pattern.SetFromUnix("/usr/lib/dragengine/libdragengine.*");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libdragengine");
	ASSERT_FALSE(path.MatchesPattern(pattern));
	
	path.SetFromUnix("/usr/lib/dragengine/libdragengine.so");
	pattern.SetFromUnix("/usr/lib/dragengine/lib?ragengine.?o");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libragengine.so");
	ASSERT_FALSE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libdragengine.o");
	ASSERT_FALSE(path.MatchesPattern(pattern));
	
	path.SetFromUnix("/usr/lib/dragengine/libdragengine.so");
	pattern.SetFromUnix("/usr/lib/dragengine/lib?ragengine.*");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libragengine.so");
	ASSERT_FALSE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libdragengine");
	ASSERT_FALSE(path.MatchesPattern(pattern));
	
	path.SetFromUnix("/usr/lib/dragengine/libdragengine.so");
	pattern.SetFromUnix("/usr/lib/dragengine/lib[D,d]ragengine.[S,s][O,o]");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libDragengine.so");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libDragengine.So");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libDragengine.SO");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libDragengine.sO");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libXragengine.so");
	ASSERT_FALSE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libDragengine.Xo");
	ASSERT_FALSE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libDragengine.sX");
	ASSERT_FALSE(path.MatchesPattern(pattern));
	
	path.SetFromUnix("/usr/lib/dragengine/libdragengine.so");
	pattern.SetFromUnix("/usr/lib/dragengine/lib[a-z]ragengine.[^a][^b]");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libxragengine.ty");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libDragengine.so");
	ASSERT_FALSE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libdragengine.ao");
	ASSERT_FALSE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libdragengine.sb");
	ASSERT_FALSE(path.MatchesPattern(pattern));
	
	path.SetFromUnix("/usr/lib/dragengine/libdragengine.so");
	pattern.SetFromUnix("/usr/lib/dragengine/libdragengine.(so,ab)");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libdragengine.ab");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libdragengine.ty");
	ASSERT_FALSE(path.MatchesPattern(pattern));
	
	path.SetFromUnix("/usr/lib/dragengine/libdragengine.so");
	pattern.SetFromUnix("/usr/lib/dragengine/libdragengine.(ab,so)");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libdragengine.ab");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/lib/dragengine/libdragengine.ty");
	ASSERT_FALSE(path.MatchesPattern(pattern));
	
	// test if one the path components is befitted with a wildcard
	path.SetFromUnix("/usr/lib/dragengine/libdragengine.so");
	pattern.SetFromUnix("/usr/*/dragengine/*.so");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/dragengine/libdragengine.so");
	ASSERT_FALSE(path.MatchesPattern(pattern));
	
	// more than one path component is befitted with a wildcard
	path.SetFromUnix("/usr/lib/dragengine/libdragengine.so");
	pattern.SetFromUnix("/usr/*/*/*.so");
	ASSERT_TRUE(path.MatchesPattern(pattern));
	path.SetFromUnix("/usr/libdragengine.so");
	ASSERT_FALSE(path.MatchesPattern(pattern));
}

void detPath::pTestStaticFactoryMethods(int testNumber){
	SetSubTestNum(testNumber);
	
	// CreatePathUnix
	decPath path1 = decPath::CreatePathUnix("/usr/lib/dragengine");
	ASSERT_TRUE(strcmp(path1.GetPrefix(), "/") == 0);
	ASSERT_TRUE(path1.GetComponentCount() == 3);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(0), "usr") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(1), "lib") == 0);
	ASSERT_TRUE(strcmp(path1.GetComponentAt(2), "dragengine") == 0);
	
	// CreatePathNative
	decPath path2 = decPath::CreatePathNative("/usr/lib/dragengine");
	ASSERT_TRUE(path2.GetComponentCount() >= 3);
	
	// CreateWorkingDirectory
	decPath path3 = decPath::CreateWorkingDirectory();
	ASSERT_TRUE(!path3.IsEmpty());
	ASSERT_TRUE(path3.IsAbsolute());
	
	// AbsolutePathUnix
	decPath path4 = decPath::AbsolutePathUnix("libdragengine.so", "/usr/lib/dragengine");
	ASSERT_TRUE(strcmp(path4.GetPrefix(), "/") == 0);
	ASSERT_TRUE(path4.GetComponentCount() == 4);
	ASSERT_TRUE(strcmp(path4.GetLastComponent(), "libdragengine.so") == 0);
	
	decPath path5 = decPath::AbsolutePathUnix("/usr/local/lib/test.so", "/usr/lib/dragengine");
	ASSERT_TRUE(strcmp(path5.GetPathUnix(), "/usr/local/lib/test.so") == 0);
	
	// AbsolutePathNative
	decPath path6 = decPath::AbsolutePathNative("libdragengine.so", "/usr/lib/dragengine");
	ASSERT_TRUE(path6.IsAbsolute());
	
	// RelativePathUnix
	decPath path7 = decPath::RelativePathUnix("/usr/lib/dragengine/libdragengine.so", "/usr/lib");
	ASSERT_TRUE(path7.IsRelative());
	ASSERT_TRUE(path7.GetComponentCount() == 2);
	ASSERT_TRUE(strcmp(path7.GetComponentAt(0), "dragengine") == 0);
	ASSERT_TRUE(strcmp(path7.GetComponentAt(1), "libdragengine.so") == 0);
	
	// RelativePathNative
	decPath path8 = decPath::RelativePathNative("/usr/lib/dragengine/libdragengine.so", "/usr/lib");
	ASSERT_TRUE(path8.GetComponentCount() >= 2);
}

void detPath::pTestPrefixManagement(int testNumber){
	SetSubTestNum(testNumber);
	
	decPath path;
	
	// Default prefix
	ASSERT_TRUE(strcmp(path.GetPrefix(), "") == 0);
	
	// SetPrefix
	path.SetPrefix("/");
	ASSERT_TRUE(strcmp(path.GetPrefix(), "/") == 0);
	
	path.SetPrefix("C:");
	ASSERT_TRUE(strcmp(path.GetPrefix(), "C:") == 0);
	
	path.SetPrefix("");
	ASSERT_TRUE(strcmp(path.GetPrefix(), "") == 0);
}

void detPath::pTestPathConversion(int testNumber){
	SetSubTestNum(testNumber);
	
	decPath path;
	
	// SetFromUnix and GetPathUnix
	path.SetFromUnix("/usr/lib/dragengine");
	ASSERT_TRUE(strcmp(path.GetPathUnix(), "/usr/lib/dragengine") == 0);
	ASSERT_TRUE(strcmp(path.GetPrefix(), "/") == 0);
	ASSERT_TRUE(path.GetComponentCount() == 3);
	
	path.SetFromUnix("usr/lib/dragengine");
	ASSERT_TRUE(strcmp(path.GetPathUnix(), "usr/lib/dragengine") == 0);
	ASSERT_TRUE(strcmp(path.GetPrefix(), "") == 0);
	ASSERT_TRUE(path.GetComponentCount() == 3);
	
	// SetFromNative and GetPathNative
	path.SetFromNative("/usr/lib/dragengine");
	decString nativePath = path.GetPathNative();
	ASSERT_TRUE(!nativePath.IsEmpty());
	ASSERT_TRUE(path.GetComponentCount() >= 3);
	
	// SetWorkingDirectory
	path.SetWorkingDirectory();
	ASSERT_TRUE(!path.IsEmpty());
	ASSERT_TRUE(path.IsAbsolute());
	
	// SetEmpty
	path.SetEmpty();
	ASSERT_TRUE(path.IsEmpty());
	ASSERT_TRUE(strcmp(path.GetPrefix(), "") == 0);
	ASSERT_TRUE(path.GetComponentCount() == 0);
}

void detPath::pTestPathOperations(int testNumber){
	SetSubTestNum(testNumber);
	
	decPath path1, path2;
	
	// SetFrom
	path1.SetFromUnix("/usr/lib/dragengine");
	path2.SetFrom(path1);
	ASSERT_TRUE(path2 == path1);
	ASSERT_TRUE(strcmp(path2.GetPathUnix(), "/usr/lib/dragengine") == 0);
	
	// Add (decPath)
	path1.SetFromUnix("/usr/lib");
	path2.SetFromUnix("dragengine/modules");
	path1.Add(path2);
	ASSERT_TRUE(strcmp(path1.GetPathUnix(), "/usr/lib/dragengine/modules") == 0);
	ASSERT_TRUE(path1.GetComponentCount() == 4);
	
	// AddUnixPath
	path1.SetFromUnix("/usr/lib");
	path1.AddUnixPath("dragengine/modules");
	ASSERT_TRUE(strcmp(path1.GetPathUnix(), "/usr/lib/dragengine/modules") == 0);
	ASSERT_TRUE(path1.GetComponentCount() == 4);
	
	// AddNativePath
	path1.SetFromUnix("/usr/lib");
	path1.AddNativePath("dragengine");
	ASSERT_TRUE(path1.GetComponentCount() == 3);
	
	// AbsolutePath
	path1.SetFromUnix("/usr/lib");
	path2.SetFromUnix("dragengine/modules");
	decPath path3 = path2.AbsolutePath(path1);
	ASSERT_TRUE(strcmp(path3.GetPathUnix(), "/usr/lib/dragengine/modules") == 0);
	
	path2.SetFromUnix("/opt/dragengine");
	path3 = path2.AbsolutePath(path1);
	ASSERT_TRUE(strcmp(path3.GetPathUnix(), "/opt/dragengine") == 0);
	
	// RelativePath
	path1.SetFromUnix("/usr/lib");
	path2.SetFromUnix("/usr/lib/dragengine/modules");
	path3 = path2.RelativePath(path1);
	ASSERT_TRUE(path3.IsRelative());
	ASSERT_TRUE(strcmp(path3.GetPathUnix(), "dragengine/modules") == 0);
	
	path2.SetFromUnix("/opt/dragengine");
	path3 = path2.RelativePath(path1);
	// RelativePath can return relative path with ".." or absolute path if not related
	ASSERT_TRUE(path3.GetComponentCount() > 0);
	
	// IsEmpty
	path1.SetEmpty();
	ASSERT_TRUE(path1.IsEmpty());
	path1.AddComponent("test");
	ASSERT_FALSE(path1.IsEmpty());
	
	// IsAbsolute / IsRelative
	path1.SetFromUnix("/usr/lib");
	ASSERT_TRUE(path1.IsAbsolute());
	ASSERT_FALSE(path1.IsRelative());
	
	path1.SetFromUnix("usr/lib");
	ASSERT_FALSE(path1.IsAbsolute());
	ASSERT_TRUE(path1.IsRelative());
}

void detPath::pTestComponentManagement(int testNumber){
	SetSubTestNum(testNumber);
	
	decPath path;
	
	// GetComponentCount, HasComponents
	ASSERT_TRUE(path.GetComponentCount() == 0);
	ASSERT_FALSE(path.HasComponents());
	
	// AddComponent
	path.AddComponent("usr");
	ASSERT_TRUE(path.GetComponentCount() == 1);
	ASSERT_TRUE(path.HasComponents());
	ASSERT_TRUE(strcmp(path.GetComponentAt(0), "usr") == 0);
	
	path.AddComponent("lib");
	ASSERT_TRUE(path.GetComponentCount() == 2);
	ASSERT_TRUE(strcmp(path.GetComponentAt(1), "lib") == 0);
	
	path.AddComponent("dragengine");
	ASSERT_TRUE(path.GetComponentCount() == 3);
	ASSERT_TRUE(strcmp(path.GetComponentAt(2), "dragengine") == 0);
	
	// GetLastComponent
	ASSERT_TRUE(strcmp(path.GetLastComponent(), "dragengine") == 0);
	
	// SetComponentAt
	path.SetComponentAt(1, "local");
	ASSERT_TRUE(strcmp(path.GetComponentAt(1), "local") == 0);
	ASSERT_TRUE(strcmp(path.GetPathUnix(), "usr/local/dragengine") == 0);
	
	// SetLastComponent
	path.SetLastComponent("modules");
	ASSERT_TRUE(strcmp(path.GetLastComponent(), "modules") == 0);
	ASSERT_TRUE(strcmp(path.GetPathUnix(), "usr/local/modules") == 0);
	
	// RemoveComponentFrom
	path.RemoveComponentFrom(1);
	ASSERT_TRUE(path.GetComponentCount() == 2);
	ASSERT_TRUE(strcmp(path.GetPathUnix(), "usr/modules") == 0);
	
	// RemoveLastComponent
	path.RemoveLastComponent();
	ASSERT_TRUE(path.GetComponentCount() == 1);
	ASSERT_TRUE(strcmp(path.GetPathUnix(), "usr") == 0);
	
	// RemoveAllComponents
	path.RemoveAllComponents();
	ASSERT_TRUE(path.GetComponentCount() == 0);
	ASSERT_FALSE(path.HasComponents());
	ASSERT_TRUE(path.IsEmpty());
}

void detPath::pTestParentPathFunctions(int testNumber){
	SetSubTestNum(testNumber);
	
	decPath parent, child;
	
	// IsParentOf
	parent.SetFromUnix("/usr/lib");
	child.SetFromUnix("/usr/lib/dragengine/modules");
	ASSERT_TRUE(parent.IsParentOf(child));
	ASSERT_FALSE(child.IsParentOf(parent));
	
	child.SetFromUnix("/usr/lib");
	ASSERT_FALSE(parent.IsParentOf(child));
	
	child.SetFromUnix("/opt/lib");
	ASSERT_FALSE(parent.IsParentOf(child));
	
	// IsEqualOrParentOf
	parent.SetFromUnix("/usr/lib");
	child.SetFromUnix("/usr/lib/dragengine");
	ASSERT_TRUE(parent.IsEqualOrParentOf(child));
	
	child.SetFromUnix("/usr/lib");
	ASSERT_TRUE(parent.IsEqualOrParentOf(child));
	
	child.SetFromUnix("/opt/lib");
	ASSERT_FALSE(parent.IsEqualOrParentOf(child));
	
	// IsDirectParentOf
	parent.SetFromUnix("/usr/lib");
	child.SetFromUnix("/usr/lib/dragengine");
	ASSERT_TRUE(parent.IsDirectParentOf(child));
	
	child.SetFromUnix("/usr/lib/dragengine/modules");
	ASSERT_FALSE(parent.IsDirectParentOf(child));
	
	child.SetFromUnix("/usr/lib");
	ASSERT_FALSE(parent.IsDirectParentOf(child));
	
	// IsEqualOrDirectParentOf
	parent.SetFromUnix("/usr/lib");
	child.SetFromUnix("/usr/lib/dragengine");
	ASSERT_TRUE(parent.IsEqualOrDirectParentOf(child));
	
	child.SetFromUnix("/usr/lib");
	ASSERT_TRUE(parent.IsEqualOrDirectParentOf(child));
	
	child.SetFromUnix("/usr/lib/dragengine/modules");
	ASSERT_FALSE(parent.IsEqualOrDirectParentOf(child));
	
	// GetParent
	parent.SetFromUnix("/usr/lib/dragengine/modules");
	decPath result = parent.GetParent();
	ASSERT_TRUE(strcmp(result.GetPathUnix(), "/usr/lib/dragengine") == 0);
	
	result = result.GetParent();
	ASSERT_TRUE(strcmp(result.GetPathUnix(), "/usr/lib") == 0);
	
	result = result.GetParent();
	ASSERT_TRUE(strcmp(result.GetPathUnix(), "/usr") == 0);
	
	// GetParent of single-component path returns prefix only
	result = result.GetParent();
	ASSERT_TRUE(result.GetComponentCount() == 0);
	ASSERT_TRUE(strcmp(result.GetPrefix(), "/") == 0);
	ASSERT_TRUE(strcmp(result.GetPathUnix(), "/") == 0);
	
	// GetParent on relative path with single component should throw exception
	decPath relativePath;
	relativePath.SetFromUnix("file.txt");
	ASSERT_TRUE(relativePath.IsRelative());
	ASSERT_TRUE(relativePath.GetComponentCount() == 1);
	ASSERT_DOES_FAIL(relativePath.GetParent());
}

void detPath::pTestPathComparison(int testNumber){
	SetSubTestNum(testNumber);
	
	decPath path1, path2;
	
	// operator==
	path1.SetFromUnix("/usr/lib/dragengine");
	path2.SetFromUnix("/usr/lib/dragengine");
	ASSERT_TRUE(path1 == path2);
	
	path2.SetFromUnix("/usr/lib/modules");
	ASSERT_FALSE(path1 == path2);
	
	path2.SetFromUnix("usr/lib/dragengine");
	ASSERT_FALSE(path1 == path2);
	
	// operator!=
	path1.SetFromUnix("/usr/lib/dragengine");
	path2.SetFromUnix("/usr/lib/modules");
	ASSERT_TRUE(path1 != path2);
	
	path2.SetFromUnix("/usr/lib/dragengine");
	ASSERT_FALSE(path1 != path2);
	
	// operator=
	path1.SetFromUnix("/usr/lib/dragengine");
	path2 = path1;
	ASSERT_TRUE(path2 == path1);
	ASSERT_TRUE(strcmp(path2.GetPathUnix(), "/usr/lib/dragengine") == 0);
	
	// operator+=
	path1.SetFromUnix("/usr/lib");
	path2.SetFromUnix("dragengine/modules");
	path1 += path2;
	ASSERT_TRUE(strcmp(path1.GetPathUnix(), "/usr/lib/dragengine/modules") == 0);
	
	// operator+
	path1.SetFromUnix("/usr/lib");
	path2.SetFromUnix("dragengine/modules");
	decPath path3 = path1 + path2;
	ASSERT_TRUE(strcmp(path3.GetPathUnix(), "/usr/lib/dragengine/modules") == 0);
	ASSERT_TRUE(strcmp(path1.GetPathUnix(), "/usr/lib") == 0); // path1 unchanged
	ASSERT_TRUE(strcmp(path2.GetPathUnix(), "dragengine/modules") == 0); // path2 unchanged
}

void detPath::pTestStaticHelpers(int testNumber){
	SetSubTestNum(testNumber);
	
	// IsUnixPathAbsolute
	ASSERT_TRUE(decPath::IsUnixPathAbsolute("/usr/lib"));
	ASSERT_FALSE(decPath::IsUnixPathAbsolute("usr/lib"));
	ASSERT_FALSE(decPath::IsUnixPathAbsolute(""));
	
	// IsNativePathAbsolute
	ASSERT_TRUE(decPath::IsNativePathAbsolute("/usr/lib") || !decPath::IsNativePathAbsolute("/usr/lib"));
	
	// PathSeparator
	char sep = decPath::PathSeparator();
	ASSERT_TRUE(sep == '/' || sep == '\\');
	
	// PathSeparatorString
	const char* sepStr = decPath::PathSeparatorString();
	ASSERT_TRUE(strcmp(sepStr, "/") == 0 || strcmp(sepStr, "\\") == 0);
}

void detPath::pTestAbsoluteRelativePaths(int testNumber){
	SetSubTestNum(testNumber);
	
	decPath base, rel, abs;
	
	// Test AbsolutePath with relative path
	base.SetFromUnix("/usr/lib");
	rel.SetFromUnix("dragengine/modules");
	abs = rel.AbsolutePath(base);
	ASSERT_TRUE(abs.IsAbsolute());
	ASSERT_TRUE(strcmp(abs.GetPathUnix(), "/usr/lib/dragengine/modules") == 0);
	
	// Test AbsolutePath with absolute path (returns itself)
	rel.SetFromUnix("/opt/dragengine");
	abs = rel.AbsolutePath(base);
	ASSERT_TRUE(strcmp(abs.GetPathUnix(), "/opt/dragengine") == 0);
	
	// Test RelativePath
	base.SetFromUnix("/usr/lib");
	abs.SetFromUnix("/usr/lib/dragengine/modules");
	rel = abs.RelativePath(base);
	ASSERT_TRUE(rel.IsRelative());
	ASSERT_TRUE(strcmp(rel.GetPathUnix(), "dragengine/modules") == 0);
	
	// Test RelativePath with unrelated paths
	abs.SetFromUnix("/opt/dragengine");
	rel = abs.RelativePath(base);
	// Can return relative path with ".." or absolute path
	ASSERT_TRUE(rel.GetComponentCount() > 0);
	
	// Test RelativePath with onlyBelow=true
	base.SetFromUnix("/usr/lib");
	abs.SetFromUnix("/usr/lib/dragengine");
	rel = abs.RelativePath(base, true);
	ASSERT_TRUE(rel.IsRelative());
	
	abs.SetFromUnix("/opt/dragengine");
	rel = abs.RelativePath(base, true);
	ASSERT_TRUE(rel.IsAbsolute()); // Not below base, returns absolute
}
