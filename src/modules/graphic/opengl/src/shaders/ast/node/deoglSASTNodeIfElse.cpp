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

#include "deoglSASTNodeIfElse.h"
#include "deoglSASTNodeBlock.h"
#include "../deoglShaderAST.h"
#include "../deoglSASTFunction.h"
#include "../deoglSASTGenerateCode.h"

#include <dragengine/common/exceptions.h>



// Class deoglSASTNodeIfElse
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSASTNodeIfElse::deoglSASTNodeIfElse() : deoglSASTNode( entIfElse ){
	pNodeCondition = NULL;
	pNodeIf = new deoglSASTNodeBlock;
	pNodeElse = new deoglSASTNodeBlock;
}

deoglSASTNodeIfElse::deoglSASTNodeIfElse( deoglSASTNode *nodeCondition ) : deoglSASTNode( entIfElse ){
	pNodeCondition = NULL;
	pNodeIf = new deoglSASTNodeBlock;
	pNodeElse = new deoglSASTNodeBlock;
	
	SetNodeCondition( nodeCondition );
}

deoglSASTNodeIfElse::~deoglSASTNodeIfElse(){
	if( pNodeElse ){
		pNodeElse->FreeReference();
	}
	if( pNodeIf ){
		pNodeIf->FreeReference();
	}
	if( pNodeCondition ){
		pNodeCondition->FreeReference();
	}
}



// Management
///////////////

void deoglSASTNodeIfElse::SetNodeCondition( deoglSASTNode *node ){
	if( node != pNodeCondition ){
		if( pNodeCondition ){
			pNodeCondition->FreeReference();
		}
		
		pNodeCondition = node;
		
		if( node ){
			node->AddReference();
		}
	}
}



void deoglSASTNodeIfElse::GenerateSourceCode( deoglSASTGenerateCode &gencode ){
	decString &sourceCode = gencode.GetSourceCode();
	
	if( pNodeCondition ){
		sourceCode.Append( "if( " );
		gencode.SetRequiresParanthesis( false );
		pNodeCondition->GenerateSourceCode( gencode );
		sourceCode.Append( " )" );
		
		gencode.SetRequiresParanthesis( false );
		pNodeIf->GenerateSourceCode( gencode );
		
		if( pNodeElse->GetNodeList().GetCount() > 0 ){
			sourceCode.Append( "else" );
			gencode.SetRequiresParanthesis( false );
			pNodeElse->GenerateSourceCode( gencode );
		}
		
	}else{
		DETHROW( deeInvalidParam );
	}
}
