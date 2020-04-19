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
	pTestCreatePath( 0 );
	pTestParenting( 1 );
	pTestComposite( 2 );
	pTestMatchPattern( 3 );
}
void detPath::CleanUp(){
}

const char *detPath::GetTestName(){ return "Path"; }



// Private Functions
//////////////////////

void detPath::pTestCreatePath( int testNumber ){
	SetSubTestNum( testNumber );
	
	// create path
	decPath path1;
	ASSERT_TRUE( strcmp( path1.GetPrefix(), "" ) == 0 );
	ASSERT_TRUE( path1.GetComponentCount() == 0 );
	ASSERT_TRUE( strcmp( path1.GetPathUnix(), "" ) == 0 );
	ASSERT_TRUE( path1.IsEmpty() );
	ASSERT_FALSE( path1.HasComponents() );
	
	path1.SetFromUnix( "/usr/lib/dragengine/libdragengine.so" );
	ASSERT_TRUE( strcmp( path1.GetPrefix(), "/" ) == 0 );
	ASSERT_TRUE( path1.GetComponentCount() == 4 );
	ASSERT_TRUE( strcmp( path1.GetPathUnix(), "/usr/lib/dragengine/libdragengine.so" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 0 ), "usr" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 1 ), "lib" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 2 ), "dragengine" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 3 ), "libdragengine.so" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetLastComponent(), "libdragengine.so" ) == 0 );
	ASSERT_FALSE( path1.IsEmpty() );
	ASSERT_TRUE( path1.HasComponents() );
}

void detPath::pTestParenting( int testNumber ){
	SetSubTestNum( testNumber );

	// test parenting
	decPath path1;
	path1.SetFromUnix( "/usr/lib/dragengine/libdragengine.so" );
	path1.RemoveLastComponent();
	ASSERT_TRUE( strcmp( path1.GetPrefix(), "/" ) == 0 );
	ASSERT_TRUE( path1.GetComponentCount() == 3 );
	ASSERT_TRUE( strcmp( path1.GetPathUnix(), "/usr/lib/dragengine" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 0 ), "usr" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 1 ), "lib" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 2 ), "dragengine" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetLastComponent(), "dragengine" ) == 0 );
	ASSERT_FALSE( path1.IsEmpty() );
	ASSERT_TRUE( path1.HasComponents() );

	path1.RemoveLastComponent();
	ASSERT_TRUE( strcmp( path1.GetPrefix(), "/" ) == 0 );
	ASSERT_TRUE( path1.GetComponentCount() == 2 );
	ASSERT_TRUE( strcmp( path1.GetPathUnix(), "/usr/lib" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 0 ), "usr" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 1 ), "lib" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetLastComponent(), "lib" ) == 0 );
	ASSERT_FALSE( path1.IsEmpty() );
	ASSERT_TRUE( path1.HasComponents() );

	path1.RemoveLastComponent();
	ASSERT_TRUE( strcmp( path1.GetPrefix(), "/" ) == 0 );
	ASSERT_TRUE( path1.GetComponentCount() == 1 );
	ASSERT_TRUE( strcmp( path1.GetPathUnix(), "/usr" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 0 ), "usr" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetLastComponent(), "usr" ) == 0 );
	ASSERT_FALSE( path1.IsEmpty() );
	ASSERT_TRUE( path1.HasComponents() );

	path1.SetEmpty();
	ASSERT_TRUE( strcmp( path1.GetPrefix(), "" ) == 0 );
	ASSERT_TRUE( path1.GetComponentCount() == 0 );
	ASSERT_TRUE( strcmp( path1.GetPathUnix(), "" ) == 0 );
	ASSERT_TRUE( path1.IsEmpty() );
	ASSERT_FALSE( path1.HasComponents() );
}

void detPath::pTestComposite( int testNumber ){
	SetSubTestNum( testNumber );
	
	// create composite path
	decPath path1;
	path1.SetFromUnix( "/usr/lib/dragengine" );
	path1.AddUnixPath( "libdragengine.so" );
	ASSERT_TRUE( strcmp( path1.GetPrefix(), "/" ) == 0 );
	ASSERT_TRUE( path1.GetComponentCount() == 4 );
	ASSERT_TRUE( strcmp( path1.GetPathUnix(), "/usr/lib/dragengine/libdragengine.so" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 0 ), "usr" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 1 ), "lib" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 2 ), "dragengine" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 3 ), "libdragengine.so" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetLastComponent(), "libdragengine.so" ) == 0 );
	ASSERT_FALSE( path1.IsEmpty() );
	ASSERT_TRUE( path1.HasComponents() );
	
	path1.SetFromUnix( "/usr/lib/dragengine/shares/libgraopengl" );
	path1.AddUnixPath( "../../libdragengine.so" );
	ASSERT_TRUE( strcmp( path1.GetPrefix(), "/" ) == 0 );
	ASSERT_TRUE( path1.GetComponentCount() == 4 );
	ASSERT_TRUE( strcmp( path1.GetPathUnix(), "/usr/lib/dragengine/libdragengine.so" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 0 ), "usr" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 1 ), "lib" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 2 ), "dragengine" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 3 ), "libdragengine.so" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetLastComponent(), "libdragengine.so" ) == 0 );
	ASSERT_FALSE( path1.IsEmpty() );
	ASSERT_TRUE( path1.HasComponents() );
	
	path1.SetFromUnix( "/usr/lib/dragengine" );
	path1.AddUnixPath( "./libdragengine.so" );
	ASSERT_TRUE( strcmp( path1.GetPrefix(), "/" ) == 0 );
	ASSERT_TRUE( path1.GetComponentCount() == 4 );
	ASSERT_TRUE( strcmp( path1.GetPathUnix(), "/usr/lib/dragengine/libdragengine.so" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 0 ), "usr" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 1 ), "lib" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 2 ), "dragengine" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 3 ), "libdragengine.so" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetLastComponent(), "libdragengine.so" ) == 0 );
	ASSERT_FALSE( path1.IsEmpty() );
	ASSERT_TRUE( path1.HasComponents() );
	
	path1.SetFromUnix( "/usr/lib/dragengine/shares/libgraopengl" );
	path1.AddUnixPath( "./../././.././libdragengine.so" );
	ASSERT_TRUE( strcmp( path1.GetPrefix(), "/" ) == 0 );
	ASSERT_TRUE( path1.GetComponentCount() == 4 );
	ASSERT_TRUE( strcmp( path1.GetPathUnix(), "/usr/lib/dragengine/libdragengine.so" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 0 ), "usr" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 1 ), "lib" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 2 ), "dragengine" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetComponentAt( 3 ), "libdragengine.so" ) == 0 );
	ASSERT_TRUE( strcmp( path1.GetLastComponent(), "libdragengine.so" ) == 0 );
	ASSERT_FALSE( path1.IsEmpty() );
	ASSERT_TRUE( path1.HasComponents() );
}

void detPath::pTestMatchPattern( int testNumber ){
	SetSubTestNum( testNumber );
	
	// simple text matching
	decPath path, pattern;
	
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine.so" );
	pattern.SetFromUnix( "/usr/lib/dragengine/libdragengine.so" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	
	pattern.SetFromUnix( "usr/lib/dragengine/libdragengine.so" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	
	path.SetFromUnix( "usr/lib/dragengine/libdragengine.so" );
	pattern.SetFromUnix( "/usr/lib/dragengine/libdragengine.so" );
	ASSERT_FALSE( path.MatchesPattern( pattern ) );
	
	pattern.SetFromUnix( "usr/lib/dragengine/libdragengine.so" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	
	// match a wild card in the last component
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine.so" );
	pattern.SetFromUnix( "/usr/lib/dragengine/*.so" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine.tos" );
	ASSERT_FALSE( path.MatchesPattern( pattern ) );
	
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine.so" );
	pattern.SetFromUnix( "/usr/lib/dragengine/lib*.so" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/hipdragengine.so" );
	ASSERT_FALSE( path.MatchesPattern( pattern ) );
	
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine.so" );
	pattern.SetFromUnix( "/usr/lib/dragengine/libdragengine.*" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine" );
	ASSERT_FALSE( path.MatchesPattern( pattern ) );
	
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine.so" );
	pattern.SetFromUnix( "/usr/lib/dragengine/lib?ragengine.?o" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libragengine.so" );
	ASSERT_FALSE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine.o" );
	ASSERT_FALSE( path.MatchesPattern( pattern ) );
	
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine.so" );
	pattern.SetFromUnix( "/usr/lib/dragengine/lib?ragengine.*" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libragengine.so" );
	ASSERT_FALSE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine" );
	ASSERT_FALSE( path.MatchesPattern( pattern ) );
	
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine.so" );
	pattern.SetFromUnix( "/usr/lib/dragengine/lib[D,d]ragengine.[S,s][O,o]" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libDragengine.so" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libDragengine.So" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libDragengine.SO" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libDragengine.sO" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libXragengine.so" );
	ASSERT_FALSE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libDragengine.Xo" );
	ASSERT_FALSE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libDragengine.sX" );
	ASSERT_FALSE( path.MatchesPattern( pattern ) );
	
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine.so" );
	pattern.SetFromUnix( "/usr/lib/dragengine/lib[a-z]ragengine.[^a][^b]" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libxragengine.ty" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libDragengine.so" );
	ASSERT_FALSE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine.ao" );
	ASSERT_FALSE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine.sb" );
	ASSERT_FALSE( path.MatchesPattern( pattern ) );
	
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine.so" );
	pattern.SetFromUnix( "/usr/lib/dragengine/libdragengine.(so,ab)" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine.ab" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine.ty" );
	ASSERT_FALSE( path.MatchesPattern( pattern ) );
	
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine.so" );
	pattern.SetFromUnix( "/usr/lib/dragengine/libdragengine.(ab,so)" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine.ab" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine.ty" );
	ASSERT_FALSE( path.MatchesPattern( pattern ) );
	
	// test if one the path components is befitted with a wildcard
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine.so" );
	pattern.SetFromUnix( "/usr/*/dragengine/*.so" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/dragengine/libdragengine.so" );
	ASSERT_FALSE( path.MatchesPattern( pattern ) );
	
	// more than one path component is befitted with a wildcard
	path.SetFromUnix( "/usr/lib/dragengine/libdragengine.so" );
	pattern.SetFromUnix( "/usr/*/*/*.so" );
	ASSERT_TRUE( path.MatchesPattern( pattern ) );
	path.SetFromUnix( "/usr/libdragengine.so" );
	ASSERT_FALSE( path.MatchesPattern( pattern ) );
}
