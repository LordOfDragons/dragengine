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

#include "deoglSASTNodeFunctionCall.h"
#include "../deoglShaderAST.h"
#include "../deoglSASTFunction.h"
#include "../deoglSASTGenerateCode.h"

#include <dragengine/common/exceptions.h>



// Class deoglSASTNodeFunctionCall
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSASTNodeFunctionCall::deoglSASTNodeFunctionCall() : deoglSASTNode( entFunctionCall ){
	pSwizzle[ 0 ] = deoglShaderAST::esNone;
	pSwizzle[ 1 ] = deoglShaderAST::esNone;
	pSwizzle[ 2 ] = deoglShaderAST::esNone;
	pSwizzle[ 3 ] = deoglShaderAST::esNone;
}

deoglSASTNodeFunctionCall::deoglSASTNodeFunctionCall( const char *name ) : deoglSASTNode( entFunctionCall ){
	pSwizzle[ 0 ] = deoglShaderAST::esNone;
	pSwizzle[ 1 ] = deoglShaderAST::esNone;
	pSwizzle[ 2 ] = deoglShaderAST::esNone;
	pSwizzle[ 3 ] = deoglShaderAST::esNone;
	
	SetName( name );
}

deoglSASTNodeFunctionCall::deoglSASTNodeFunctionCall( const char *name, deoglSASTNode *nodeParam1 ) : deoglSASTNode( entFunctionCall ){
	pSwizzle[ 0 ] = deoglShaderAST::esNone;
	pSwizzle[ 1 ] = deoglShaderAST::esNone;
	pSwizzle[ 2 ] = deoglShaderAST::esNone;
	pSwizzle[ 3 ] = deoglShaderAST::esNone;
	
	SetName( name );
	pNodeListParams.Add( nodeParam1 );
}

deoglSASTNodeFunctionCall::deoglSASTNodeFunctionCall( const char *name, deoglSASTNode *nodeParam1,
deoglSASTNode *nodeParam2 ) : deoglSASTNode( entFunctionCall ){
	pSwizzle[ 0 ] = deoglShaderAST::esNone;
	pSwizzle[ 1 ] = deoglShaderAST::esNone;
	pSwizzle[ 2 ] = deoglShaderAST::esNone;
	pSwizzle[ 3 ] = deoglShaderAST::esNone;
	
	SetName( name );
	pNodeListParams.Add( nodeParam1 );
	pNodeListParams.Add( nodeParam2 );
}

deoglSASTNodeFunctionCall::deoglSASTNodeFunctionCall( const char *name, deoglSASTNode *nodeParam1,
deoglSASTNode *nodeParam2, deoglSASTNode *nodeParam3 ) : deoglSASTNode( entFunctionCall ){
	pSwizzle[ 0 ] = deoglShaderAST::esNone;
	pSwizzle[ 1 ] = deoglShaderAST::esNone;
	pSwizzle[ 2 ] = deoglShaderAST::esNone;
	pSwizzle[ 3 ] = deoglShaderAST::esNone;
	
	SetName( name );
	pNodeListParams.Add( nodeParam1 );
	pNodeListParams.Add( nodeParam2 );
	pNodeListParams.Add( nodeParam3 );
}

deoglSASTNodeFunctionCall::deoglSASTNodeFunctionCall( const char *name, deoglSASTNode *nodeParam1,
deoglSASTNode *nodeParam2, deoglSASTNode *nodeParam3, deoglSASTNode *nodeParam4 ) : deoglSASTNode( entFunctionCall ){
	pSwizzle[ 0 ] = deoglShaderAST::esNone;
	pSwizzle[ 1 ] = deoglShaderAST::esNone;
	pSwizzle[ 2 ] = deoglShaderAST::esNone;
	pSwizzle[ 3 ] = deoglShaderAST::esNone;
	
	SetName( name );
	pNodeListParams.Add( nodeParam1 );
	pNodeListParams.Add( nodeParam2 );
	pNodeListParams.Add( nodeParam3 );
	pNodeListParams.Add( nodeParam4 );
}

deoglSASTNodeFunctionCall::~deoglSASTNodeFunctionCall(){
	pNodeListParams.RemoveAll();
}



// Management
///////////////

void deoglSASTNodeFunctionCall::SetName( const char *name ){
	pName = name;
}

int deoglSASTNodeFunctionCall::GetSwizzleAt( int position ) const{
	if( position < 0 || position > 3 ){
		DETHROW( deeInvalidParam );
	}
	
	return ( int )pSwizzle[ position ];
}

void deoglSASTNodeFunctionCall::SetSwizzleAt( int position, int swizzle ){
	if( position < 0 || position > 3 || swizzle < 0 || swizzle >= deoglShaderAST::ES_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	pSwizzle[ position ] = ( short )swizzle;
}

void deoglSASTNodeFunctionCall::SetSwizzles( int swizzle1 ){
	if( swizzle1 < 0 || swizzle1 >= deoglShaderAST::ES_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	pSwizzle[ 0 ] = swizzle1;
	pSwizzle[ 1 ] = deoglShaderAST::esNone;
	pSwizzle[ 2 ] = deoglShaderAST::esNone;
	pSwizzle[ 3 ] = deoglShaderAST::esNone;
}

void deoglSASTNodeFunctionCall::SetSwizzles( int swizzle1, int swizzle2 ){
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

void deoglSASTNodeFunctionCall::SetSwizzles( int swizzle1, int swizzle2, int swizzle3 ){
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

void deoglSASTNodeFunctionCall::SetSwizzles( int swizzle1, int swizzle2, int swizzle3, int swizzle4 ){
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



void deoglSASTNodeFunctionCall::GenerateSourceCode( deoglSASTGenerateCode &gencode ){
	decString &sourceCode = gencode.GetSourceCode();
	const int count = pNodeListParams.GetCount();
	int i;
	
	sourceCode.Append( pName.GetString() );
	
	if( count == 0 ){
		sourceCode.Append( "()" );
		
	}else{
		sourceCode.Append( "( " );
		gencode.IncrementIndentTabs();
		
		for( i=0; i<count; i++ ){
			if( i > 0 ){
				sourceCode.Append( ", " );
			}
			gencode.SetRequiresParanthesis( false );
			pNodeListParams.GetAt( i )->GenerateSourceCode( gencode );
		}
		
		gencode.DecrementIndentTabs();
		sourceCode.Append( " )" );
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
