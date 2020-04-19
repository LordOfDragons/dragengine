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

#include "deoglShaderAST.h"
#include "deoglSASTDeclaration.h"
#include "node/deoglSASTNode.h"

#include <dragengine/common/exceptions.h>



// Class deoglSASTDeclaration
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSASTDeclaration::deoglSASTDeclaration( const char *name ){
	pName = name;
	pDataType = deoglShaderAST::edtFloat;
	pNodeInit = NULL;
	pDirectionQualifier = edqNone;
}

deoglSASTDeclaration::deoglSASTDeclaration( const char *name, int dataType ){
	pName = name;
	pDataType = deoglShaderAST::edtFloat;
	pNodeInit = NULL;
	pDirectionQualifier = edqNone;
	
	SetDataType( dataType );
}

deoglSASTDeclaration::deoglSASTDeclaration( const char *name, int dataType, deoglSASTNode *nodeInit ){
	pName = name;
	pDataType = deoglShaderAST::edtFloat;
	pNodeInit = NULL;
	pDirectionQualifier = edqNone;
	
	SetDataType( dataType );
	SetNodeInit( nodeInit );
}

deoglSASTDeclaration::deoglSASTDeclaration( const char *name, int dataType, int directionQualifitier ){
	pName = name;
	pDataType = deoglShaderAST::edtFloat;
	pNodeInit = NULL;
	pDirectionQualifier = edqNone;
	
	SetDataType( dataType );
	SetDirectionQualifier( directionQualifitier );
}

deoglSASTDeclaration::~deoglSASTDeclaration(){
	pCleanUp();
}



// Management
///////////////

void deoglSASTDeclaration::SetDataType( int dataType ){
	if( dataType < 0 || dataType >= deoglShaderAST::EDT_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	pDataType = dataType;
}

void deoglSASTDeclaration::SetNodeInit( deoglSASTNode *node ){
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

void deoglSASTDeclaration::SetDirectionQualifier( int qualifier ){
	if( qualifier < 0 || qualifier >= EDQ_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	pDirectionQualifier = qualifier;
}



// Private functions
//////////////////////

void deoglSASTDeclaration::pCleanUp(){
	if( pNodeInit ){
		pNodeInit->FreeReference();
	}
}
