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

#include "deoglSASTNodeBlock.h"
#include "../deoglShaderAST.h"
#include "../deoglSASTGenerateCode.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>



// Class deoglSASTNodeBlock
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSASTNodeBlock::deoglSASTNodeBlock() : deoglSASTNode( entBlock ){
}

deoglSASTNodeBlock::~deoglSASTNodeBlock(){
	pNodeList.RemoveAll();
}



// Management
///////////////

void deoglSASTNodeBlock::GenerateSourceCode( deoglSASTGenerateCode &gencode ){
	decString &sourceCode = gencode.GetSourceCode();
	const int count = pNodeList.GetCount();
	int i;
	
	sourceCode.Append( "{\n" );
	gencode.IncrementIndentTabs();
	
	for( i=0; i<count; i++ ){
		gencode.AddIndentTabsToSourceCode();
		gencode.SetRequiresParanthesis( false );
		pNodeList.GetAt( i )->GenerateSourceCode( gencode );
		
		if( gencode.GetRequiresEndOfCommand() ){
			sourceCode.Append( ";\n" );
			
		}else{
			sourceCode.Append( "\n" );
		}
	}
	
	gencode.DecrementIndentTabs();
	gencode.AddIndentTabsToSourceCode();
	sourceCode.Append( "}" );
	gencode.SetRequiresEndOfCommand( false );
}
