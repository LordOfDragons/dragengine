/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglDeveloperModeTests.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/deoglSPBParameter.h"
#include "../utils/convexhull/deoglConvexHull2D.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>



// Assert macros
//////////////////

#define ASSERT_TRUE(expr)			if(!(expr)) DETHROW_INFO(deeTestFailed, "AssertTrue failed")
#define ASSERT_FALSE(expr)			if(expr) DETHROW_INFO(deeTestFailed, "AssertFalse failed")
#define ASSERT_EQUAL(expr,val)		if((expr) != (val)) DETHROW_INFO(deeTestFailed, "AssertEqual failed")
#define ASSERT_NEQUAL(expr,val)		if((expr) == (val)) DETHROW_INFO(deeTestFailed, "AssertNotEqual failed")
#define ASSERT_FEQUAL(expr,val)		if( fabs( (expr) - (val) ) >= 0.00001 ) DETHROW_INFO(deeTestFailed, "AssertFEqual failed")
#define ASSERT_NFEQUAL(expr,val)	if( fabs( (expr) - (val) ) < 0.00001 ) DETHROW_INFO(deeTestFailed, "AssertNFEqual failed")
#define ASSERT_NOT_NULL(expr)		if( ! ( expr ) ) DETHROW_INFO(deeTestFailed, "AssertNotNull failed")
#define ASSERT_NULL(expr)			if( expr ) DETHROW_INFO(deeTestFailed, "AssertNull failed")

#define ASSERT_NEW(expr)			if(!(expr)) DETHROW(deeOutOfMemory)

#define ASSERT_DOES_FAIL(expr)		{ bool __failed=false; try{ expr; }catch( const deException & ){ __failed=true; } if( ! __failed ) DETHROW_INFO(deeTestFailed, "AssertDoesFail failed"); }



// Class deoglDeveloperModeTests
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDeveloperModeTests::deoglDeveloperModeTests( deoglRenderThread &renderThread ) :
pRenderThread( renderThread )
{
	(void)pRenderThread; // silence compiler warning
}

deoglDeveloperModeTests::~deoglDeveloperModeTests(){
}



// Management
///////////////

void deoglDeveloperModeTests::Help( decUnicodeString &answer ){
	answer.SetFromUTF8( "dm_tests <mode>\n" );
	answer.AppendFromUTF8( "where <mode> can be:\n" );
	answer.AppendFromUTF8( "shaderParameterBlock => Test deoglSPBlockUBO.\n" );
	answer.AppendFromUTF8( "convexHull2D => Test deoglConvexHull2D.\n" );
}

void deoglDeveloperModeTests::Tests( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() > 1 ){
		if( command.MatchesArgumentAt( 1, "convexHull2D" ) ){
			try{
				TestConvexHull2D( answer );
				AnswerTestPassed( answer );
				
			}catch( const deException &e ){
				AnswerTestFailedWithException( answer, e );
			}
			
		}else{
			Help( answer );
		}
		
	}else{
		Help( answer );
	}
}



void deoglDeveloperModeTests::TestConvexHull2D( decUnicodeString &answer ){
	const decVector2 points[ 10 ] = { decVector2( 10.0f, 7.0f ), decVector2( 4.0f, 3.0f ), decVector2( 6.0f, 9.0f ),
		decVector2( 15.0f, 10.0f ), decVector2( 2.0f, 7.0f ), decVector2( 6.0f, 14.0f ), decVector2( 9.0f, 2.0f ),
		decVector2( 14.0f, 5.0f ), decVector2( 10.0f, 12.0f ), decVector2( 15.0f, 15.0f ) };
	const int order1[ 10 ] = { 4, 5, 9, 3, 7, 6, 1, 0, 2, 8 };
	const int hull[ 7 ] = { 4, 5, 9, 3, 7, 6, 1 };
	const int hull2[ 3 ] = { 1, 2, 0 };
	deoglConvexHull2D convexHull;
	int i;
	
	// test empty
	convexHull.RemoveAllPoints();
	convexHull.CalculateHull();
	ASSERT_EQUAL( convexHull.GetPointCount(), 0 );
	ASSERT_EQUAL( convexHull.GetHullPointCount(), 0 );
	
	// test less than 3 points
	convexHull.RemoveAllPoints();
	convexHull.AddPoint( points[ 0 ] );
	convexHull.AddPoint( points[ 1 ] );
	convexHull.CalculateHull();
	ASSERT_EQUAL( convexHull.GetPointCount(), 2 );
	ASSERT_EQUAL( convexHull.GetHullPointCount(), 0 );
	
	// test exactly 3
	convexHull.RemoveAllPoints();
	for( i=0; i<3; i++ ){
		convexHull.AddPoint( points[ i ] );
	}
	convexHull.CalculateHull();
	ASSERT_EQUAL( convexHull.GetPointCount(), 3 );
	ASSERT_EQUAL( convexHull.GetHullPointCount(), 3 );
	for( i=0; i<3; i++ ){
		ASSERT_EQUAL( convexHull.GetHullPointAt( i ), hull2[ i ] );
		ASSERT_TRUE( convexHull.GetHullPointVectorAt( i ).IsEqualTo( points[ hull2[ i ] ] ) );
	}
	
	// test all points
	convexHull.RemoveAllPoints();
	for( i=0; i<10; i++ ){
		convexHull.AddPoint( points[ i ] );
	}
	convexHull.CalculateHull();
	ASSERT_EQUAL( convexHull.GetPointCount(), 10 );
	ASSERT_EQUAL( convexHull.GetHullPointCount(), 7 );
	for( i=0; i<7; i++ ){
		ASSERT_EQUAL( convexHull.GetHullPointAt( i ), hull[ i ] );
		ASSERT_TRUE( convexHull.GetHullPointVectorAt( i ).IsEqualTo( points[ hull[ i ] ] ) );
	}
	
	// test all points with reverse point order
	convexHull.RemoveAllPoints();
	for( i=9; i>=0; i-- ){
		convexHull.AddPoint( points[ i ] );
	}
	convexHull.CalculateHull();
	ASSERT_EQUAL( convexHull.GetPointCount(), 10 );
	ASSERT_EQUAL( convexHull.GetHullPointCount(), 7 );
	for( i=0; i<7; i++ ){
		ASSERT_EQUAL( convexHull.GetHullPointAt( i ), 9 - hull[ i ] );
		ASSERT_TRUE( convexHull.GetHullPointVectorAt( i ).IsEqualTo( points[ hull[ i ] ] ) );
	}
	
	// test all points with in the correct order
	convexHull.RemoveAllPoints();
	for( i=0; i<10; i++ ){
		convexHull.AddPoint( points[ order1[ i ] ] );
	}
	convexHull.CalculateHull();
	ASSERT_EQUAL( convexHull.GetPointCount(), 10 );
	ASSERT_EQUAL( convexHull.GetHullPointCount(), 7 );
	for( i=0; i<7; i++ ){
		ASSERT_EQUAL( convexHull.GetHullPointAt( i ), i );
		ASSERT_TRUE( convexHull.GetHullPointVectorAt( i ).IsEqualTo( points[ order1[ i ] ] ) );
	}
	
	// test all points with in the reverse order than correct
	convexHull.RemoveAllPoints();
	for( i=9; i>=0; i-- ){
		convexHull.AddPoint( points[ order1[ i ] ] );
	}
	convexHull.CalculateHull();
	ASSERT_EQUAL( convexHull.GetPointCount(), 10 );
	ASSERT_EQUAL( convexHull.GetHullPointCount(), 7 );
	for( i=0; i<7; i++ ){
		ASSERT_EQUAL( convexHull.GetHullPointAt( i ), 9 - i );
		ASSERT_TRUE( convexHull.GetHullPointVectorAt( i ).IsEqualTo( points[ order1[ i ] ] ) );
	}
}



void deoglDeveloperModeTests::AnswerTestPassed( decUnicodeString &answer ){
	answer.AppendFromUTF8( "Test passed\n" );
}

void deoglDeveloperModeTests::AnswerTestFailedWithException( decUnicodeString &answer, const deException &exception ){
	answer.AppendFromUTF8( "Test failed\n" );
	answer.AppendFromUTF8( exception.FormatOutput().Join( "\n" ) );
}
