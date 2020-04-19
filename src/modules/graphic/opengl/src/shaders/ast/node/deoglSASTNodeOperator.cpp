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

#include "deoglSASTNodeOperator.h"
#include "../deoglSASTGenerateCode.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>



// Operator Strings
/////////////////////

static const char *vOperatorStrings[ deoglSASTNodeOperator::EO_COUNT ] = {
	"-", // eoNegate
	"! ", // eoNot
	" = ", // eoAssign
	" + ", // eoAdd
	" - ", // eoSubtract
	" * ", // eoMultiply
	" / ", // eoDivide
	" < ", // eoLessThan
	" > ", // eoGreaterThan
	" <= ", // eoLessThanOrEqual
	" >= ", // eoGreaterThanOrEqual
	" += ", // eoAssignAdd
	" -= ", // eoAssignSubtract
	" *= ", // eoAssignMultiply
	" /= ", // eoAssignDivide
};



// Class deoglSASTNodeOperator
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSASTNodeOperator::deoglSASTNodeOperator() : deoglSASTNode( entOperator ){
	pOperator = eoAssign;
	pNodeOperand1 = NULL;
	pNodeOperand2 = NULL;
}

deoglSASTNodeOperator::deoglSASTNodeOperator( int op ) : deoglSASTNode( entOperator ){
	pOperator = eoAssign;
	pNodeOperand1 = NULL;
	pNodeOperand2 = NULL;
	
	SetOperator( op );
}

deoglSASTNodeOperator::deoglSASTNodeOperator( int op, deoglSASTNode *nodeOperand1 ) : deoglSASTNode( entOperator ){
	pOperator = eoAssign;
	pNodeOperand1 = NULL;
	pNodeOperand2 = NULL;
	
	SetOperator( op );
	SetNodeOperand1( nodeOperand1 );
}

deoglSASTNodeOperator::deoglSASTNodeOperator( int op, deoglSASTNode *nodeOperand1, deoglSASTNode *nodeOperand2 ) : deoglSASTNode( entOperator ){
	pOperator = eoAssign;
	pNodeOperand1 = NULL;
	pNodeOperand2 = NULL;
	
	SetOperator( op );
	SetNodeOperand1( nodeOperand1 );
	SetNodeOperand2( nodeOperand2 );
}

deoglSASTNodeOperator::~deoglSASTNodeOperator(){
	if( pNodeOperand2 ){
		pNodeOperand2->FreeReference();
	}
	if( pNodeOperand1 ){
		pNodeOperand1->FreeReference();
	}
}



// Management
///////////////

void deoglSASTNodeOperator::SetOperator( int op ){
	if( op < 0 || op >= EO_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	pOperator = op;
}

void deoglSASTNodeOperator::SetNodeOperand1( deoglSASTNode *node ){
	if( node != pNodeOperand1 ){
		if( pNodeOperand1 ){
			pNodeOperand1->FreeReference();
		}
		
		pNodeOperand1 = node;
		
		if( node ){
			node->AddReference();
		}
	}
}

void deoglSASTNodeOperator::SetNodeOperand2( deoglSASTNode *node ){
	if( node != pNodeOperand2 ){
		if( pNodeOperand2 ){
			pNodeOperand2->FreeReference();
		}
		
		pNodeOperand2 = node;
		
		if( node ){
			node->AddReference();
		}
	}
}



void deoglSASTNodeOperator::GenerateSourceCode( deoglSASTGenerateCode &gencode ){
	const bool requiresParanthesis = gencode.GetRequiresParanthesis();
	decString &sourceCode = gencode.GetSourceCode();
	
	if( pOperator < eoAssign ){
		if( ! pNodeOperand1 || pNodeOperand2 ){
			DETHROW( deeInvalidParam );
		}
		
		sourceCode.Append( vOperatorStrings[ pOperator ] );
		if( requiresParanthesis ){
			sourceCode.Append( "( " );
		}
		gencode.SetRequiresParanthesis( true );
		pNodeOperand1->GenerateSourceCode( gencode );
		if( requiresParanthesis ){
			sourceCode.Append( " )" );
		}
		
	}else{
		if( ! pNodeOperand1 || ! pNodeOperand2 ){
			DETHROW( deeInvalidParam );
		}
		
		const bool isAssignOp = ( pOperator == eoAssign || pOperator == eoAssignAdd || pOperator == eoAssignSubtract
			|| pOperator == eoAssignMultiply || pOperator == eoAssignDivide );
		
		if( requiresParanthesis ){
			sourceCode.Append( "( " );
		}
		gencode.SetRequiresParanthesis( ! isAssignOp );
		pNodeOperand1->GenerateSourceCode( gencode );
		sourceCode.Append( vOperatorStrings[ pOperator ] );
		gencode.SetRequiresParanthesis( ! isAssignOp );
		pNodeOperand2->GenerateSourceCode( gencode );
		if( requiresParanthesis ){
			sourceCode.Append( " )" );
		}
	}
	
	gencode.SetRequiresEndOfCommand( true );
}
