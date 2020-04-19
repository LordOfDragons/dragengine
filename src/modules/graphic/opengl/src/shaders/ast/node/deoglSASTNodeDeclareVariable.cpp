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

#include "deoglSASTNodeDeclareVariable.h"
#include "../deoglSASTDeclaration.h"
#include "../deoglSASTGenerateCode.h"
#include "../deoglShaderAST.h"

#include <dragengine/common/exceptions.h>



// Class deoglSASTNodeDeclareVariable
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSASTNodeDeclareVariable::deoglSASTNodeDeclareVariable() : deoglSASTNode( entDeclareCariable ){
	pDataType = deoglShaderAST::edtFloat;
	pNodeInit = NULL;
}

deoglSASTNodeDeclareVariable::deoglSASTNodeDeclareVariable( const char *name ) : deoglSASTNode( entVariable ){
	pDataType = deoglShaderAST::edtFloat;
	pNodeInit = NULL;
	
	SetName( name );
}

deoglSASTNodeDeclareVariable::deoglSASTNodeDeclareVariable( const char *name, int dataType ) : deoglSASTNode( entVariable ){
	pDataType = deoglShaderAST::edtFloat;
	pNodeInit = NULL;
	
	SetName( name );
	SetDataType( dataType );
}

deoglSASTNodeDeclareVariable::deoglSASTNodeDeclareVariable( const char *name, int dataType, deoglSASTNode *nodeInit ) : deoglSASTNode( entVariable ){
	pDataType = deoglShaderAST::edtFloat;
	pNodeInit = NULL;
	
	SetName( name );
	SetDataType( dataType );
	SetNodeInit( nodeInit );
}

deoglSASTNodeDeclareVariable::~deoglSASTNodeDeclareVariable(){
	if( pNodeInit ){
		pNodeInit->FreeReference();
	}
}



// Management
///////////////

void deoglSASTNodeDeclareVariable::SetName( const char *name ){
	pName = name;
}

void deoglSASTNodeDeclareVariable::SetDataType( int dataType ){
	if( dataType < 0 || dataType >= deoglShaderAST::EDT_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	pDataType = dataType;
}

void deoglSASTNodeDeclareVariable::SetNodeInit( deoglSASTNode *node ){
	if( node != pNodeInit ){
		if( pNodeInit ){
			pNodeInit->FreeReference();
		}
		
		pNodeInit = node;
		
		if( node ){
			node->AddReference();
		}
	}
}



void deoglSASTNodeDeclareVariable::GenerateSourceCode( deoglSASTGenerateCode &gencode ){
	decString &sourceCode = gencode.GetSourceCode();
	
	sourceCode.Append( deoglShaderAST::GetDataTypeString( pDataType ) );
	sourceCode.AppendCharacter( ' ' );
	sourceCode.Append( pName.GetString() );
	
	if( pNodeInit ){
		sourceCode.Append( " = " );
		gencode.SetRequiresParanthesis( false );
		pNodeInit->GenerateSourceCode( gencode );
	}
	
	gencode.SetRequiresEndOfCommand( true );
}
