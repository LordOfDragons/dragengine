/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglSASTNodeVariable.h"
#include "deoglSASTNodeConstant.h"
#include "../deoglSASTDeclaration.h"
#include "../deoglSASTGenerateCode.h"
#include "../deoglShaderAST.h"
#include "../deoglShaderASTHelper.h"

#include <dragengine/common/exceptions.h>



/** Variable types. */
enum eVariableTypes{
	evtUnknown,
	evtUniform,
	evtSampler,
	evtInput,
	evtOutput,
	evtVariable,
	evtParameter,
	EVT_COUNT
};



// Class deoglSASTNodeVariable
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSASTNodeVariable::deoglSASTNodeVariable() : deoglSASTNode( entVariable ){
	pSwizzle[ 0 ] = deoglShaderAST::esNone;
	pSwizzle[ 1 ] = deoglShaderAST::esNone;
	pSwizzle[ 2 ] = deoglShaderAST::esNone;
	pSwizzle[ 3 ] = deoglShaderAST::esNone;
}

deoglSASTNodeVariable::deoglSASTNodeVariable( const char *name ) : deoglSASTNode( entVariable ){
	pSwizzle[ 0 ] = deoglShaderAST::esNone;
	pSwizzle[ 1 ] = deoglShaderAST::esNone;
	pSwizzle[ 2 ] = deoglShaderAST::esNone;
	pSwizzle[ 3 ] = deoglShaderAST::esNone;
	
	SetName( name );
}

deoglSASTNodeVariable::deoglSASTNodeVariable( const char *name, int swizzle1 ) : deoglSASTNode( entVariable ){
	pSwizzle[ 0 ] = deoglShaderAST::esNone;
	pSwizzle[ 1 ] = deoglShaderAST::esNone;
	pSwizzle[ 2 ] = deoglShaderAST::esNone;
	pSwizzle[ 3 ] = deoglShaderAST::esNone;
	
	SetName( name );
	SetSwizzleAt( 0, swizzle1 );
}

deoglSASTNodeVariable::deoglSASTNodeVariable( const char *name, int swizzle1, int swizzle2 ) : deoglSASTNode( entVariable ){
	pSwizzle[ 0 ] = deoglShaderAST::esNone;
	pSwizzle[ 1 ] = deoglShaderAST::esNone;
	pSwizzle[ 2 ] = deoglShaderAST::esNone;
	pSwizzle[ 3 ] = deoglShaderAST::esNone;
	
	SetName( name );
	SetSwizzleAt( 0, swizzle1 );
	SetSwizzleAt( 1, swizzle2 );
}

deoglSASTNodeVariable::deoglSASTNodeVariable( const char *name, int swizzle1, int swizzle2, int swizzle3 ) : deoglSASTNode( entVariable ){
	pSwizzle[ 0 ] = deoglShaderAST::esNone;
	pSwizzle[ 1 ] = deoglShaderAST::esNone;
	pSwizzle[ 2 ] = deoglShaderAST::esNone;
	pSwizzle[ 3 ] = deoglShaderAST::esNone;
	
	SetName( name );
	SetSwizzleAt( 0, swizzle1 );
	SetSwizzleAt( 1, swizzle2 );
	SetSwizzleAt( 2, swizzle3 );
}

deoglSASTNodeVariable::deoglSASTNodeVariable( const char *name, int swizzle1, int swizzle2, int swizzle3, int swizzle4 ) : deoglSASTNode( entVariable ){
	pSwizzle[ 0 ] = deoglShaderAST::esNone;
	pSwizzle[ 1 ] = deoglShaderAST::esNone;
	pSwizzle[ 2 ] = deoglShaderAST::esNone;
	pSwizzle[ 3 ] = deoglShaderAST::esNone;
	
	SetName( name );
	SetSwizzleAt( 0, swizzle1 );
	SetSwizzleAt( 1, swizzle2 );
	SetSwizzleAt( 2, swizzle3 );
	SetSwizzleAt( 3, swizzle4 );
}

deoglSASTNodeVariable::~deoglSASTNodeVariable(){
	pNodeArrayList.RemoveAll();
}



// Management
///////////////

void deoglSASTNodeVariable::SetName( const char *name ){
	pName = name;
}

int deoglSASTNodeVariable::GetSwizzleAt( int position ) const{
	if( position < 0 || position > 3 ){
		DETHROW( deeInvalidParam );
	}
	
	return ( int )pSwizzle[ position ];
}

void deoglSASTNodeVariable::SetSwizzleAt( int position, int swizzle ){
	if( position < 0 || position > 3 || swizzle < 0 || swizzle >= deoglShaderAST::ES_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	pSwizzle[ position ] = ( short )swizzle;
}

void deoglSASTNodeVariable::SetSwizzles( int swizzle1 ){
	if( swizzle1 < 0 || swizzle1 >= deoglShaderAST::ES_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	pSwizzle[ 0 ] = swizzle1;
	pSwizzle[ 1 ] = deoglShaderAST::esNone;
	pSwizzle[ 2 ] = deoglShaderAST::esNone;
	pSwizzle[ 3 ] = deoglShaderAST::esNone;
}

void deoglSASTNodeVariable::SetSwizzles( int swizzle1, int swizzle2 ){
	if( swizzle1 < 0 || swizzle1 >= deoglShaderAST::ES_COUNT ){
		DETHROW( deeInvalidParam );
	}
	if( swizzle2 < 0 || swizzle2 >= deoglShaderAST::ES_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	pSwizzle[ 0 ] = swizzle1;
	pSwizzle[ 1 ] = swizzle2;
	pSwizzle[ 2 ] = deoglShaderAST::esNone;
	pSwizzle[ 3 ] = deoglShaderAST::esNone;
}

void deoglSASTNodeVariable::SetSwizzles( int swizzle1, int swizzle2, int swizzle3 ){
	if( swizzle1 < 0 || swizzle1 >= deoglShaderAST::ES_COUNT ){
		DETHROW( deeInvalidParam );
	}
	if( swizzle2 < 0 || swizzle2 >= deoglShaderAST::ES_COUNT ){
		DETHROW( deeInvalidParam );
	}
	if( swizzle3 < 0 || swizzle3 >= deoglShaderAST::ES_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	pSwizzle[ 0 ] = swizzle1;
	pSwizzle[ 1 ] = swizzle2;
	pSwizzle[ 2 ] = swizzle3;
	pSwizzle[ 3 ] = deoglShaderAST::esNone;
}

void deoglSASTNodeVariable::SetSwizzles( int swizzle1, int swizzle2, int swizzle3, int swizzle4 ){
	if( swizzle1 < 0 || swizzle1 >= deoglShaderAST::ES_COUNT ){
		DETHROW( deeInvalidParam );
	}
	if( swizzle2 < 0 || swizzle2 >= deoglShaderAST::ES_COUNT ){
		DETHROW( deeInvalidParam );
	}
	if( swizzle3 < 0 || swizzle3 >= deoglShaderAST::ES_COUNT ){
		DETHROW( deeInvalidParam );
	}
	if( swizzle4 < 0 || swizzle4 >= deoglShaderAST::ES_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	pSwizzle[ 0 ] = swizzle1;
	pSwizzle[ 1 ] = swizzle2;
	pSwizzle[ 2 ] = swizzle3;
	pSwizzle[ 3 ] = swizzle4;
}

void deoglSASTNodeVariable::AddArrayConstant( int index ){
	deoglSASTNodeConstant *nodeConst = NULL;
	
	try{
		nodeConst = new deoglSASTNodeConstant( index );
		LIST_ADD_NODE( pNodeArrayList, nodeConst );
		
	}catch( const deException & ){
		NODE_CATCH_CLEANUP( nodeConst );
		throw;
	}
}



void deoglSASTNodeVariable::GenerateSourceCode( deoglSASTGenerateCode &gencode ){
	decString &sourceCode = gencode.GetSourceCode();
	const int arrayCount = pNodeArrayList.GetCount();
	int i;
	
	sourceCode.Append( pName.GetString() );
	
	for( i=0; i<arrayCount; i++ ){
		sourceCode.Append( "[ " );
		gencode.SetRequiresParanthesis( false );
		pNodeArrayList.GetAt( i )->GenerateSourceCode( gencode );
		sourceCode.Append( " ]" );
	}
	
	if( pSwizzle[ 0 ] != deoglShaderAST::esNone ){
		sourceCode.AppendCharacter( '.' );
		
		for( i=0; i<4; i++ ){
			if( pSwizzle[ i ] == deoglShaderAST::esNone ){
				break;
				
			}else{
				sourceCode.Append( deoglShaderAST::GetSwizzleString( pSwizzle[ i ] ) );
			}
		}
	}
	
	gencode.SetRequiresEndOfCommand( true );
}
