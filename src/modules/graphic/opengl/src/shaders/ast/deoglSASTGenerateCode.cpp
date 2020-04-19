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
#include "deoglSASTGenerateCode.h"
#include "node/deoglSASTNode.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>



// Class deoglSASTGenerateCode
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSASTGenerateCode::deoglSASTGenerateCode( deoglShaderAST *ast, decString *sourceCode ){
	if( ! ast || ! sourceCode ){
		pAST = ast;
	}
	pSourceCode = sourceCode;
	
	pIndentTabs = 0;
	pRequiresParanthesis = false;
	pRequiresEndOfCommand = true;
}

deoglSASTGenerateCode::~deoglSASTGenerateCode(){
}



// Management
///////////////

void deoglSASTGenerateCode::SetIndentTabs( int tabs ){
	if( tabs < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pIndentTabs = tabs;
}

void deoglSASTGenerateCode::SetRequiresParanthesis( bool requiresParanthesis ){
	pRequiresParanthesis = requiresParanthesis;
}

void deoglSASTGenerateCode::SetRequiresEndOfCommand( bool requiresEndOfCommand ){
	pRequiresEndOfCommand = requiresEndOfCommand;
}



void deoglSASTGenerateCode::AddIndentTabsToSourceCode(){
	int i;
	
	for( i=0; i<pIndentTabs; i++ ){
		pSourceCode->AppendCharacter( '\t' );
	}
}
