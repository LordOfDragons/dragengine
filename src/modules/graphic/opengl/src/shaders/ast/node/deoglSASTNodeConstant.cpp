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

#include "deoglSASTNodeConstant.h"
#include "../deoglSASTGenerateCode.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>



// Class deoglSASTNodeConstant
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSASTNodeConstant::deoglSASTNodeConstant() : deoglSASTNode( entConstant ){
	pConstantType = ectFloat;
	pValueFloat = 0.0f;
	pValueInt = 0;
}

deoglSASTNodeConstant::deoglSASTNodeConstant( float value ) : deoglSASTNode( entConstant ){
	pConstantType = ectFloat;
	pValueFloat = value;
	pValueInt = 0;
}

deoglSASTNodeConstant::deoglSASTNodeConstant( int value ) : deoglSASTNode( entConstant ){
	pConstantType = ectInteger;
	pValueFloat = 0.0f;
	pValueInt = value;
}

deoglSASTNodeConstant::deoglSASTNodeConstant( bool value ) : deoglSASTNode( entConstant ){
	if( value ){
		pConstantType = ectTrue;
		
	}else{
		pConstantType = ectFalse;
	}
	
	pValueFloat = 0.0f;
	pValueInt = 0;
}

deoglSASTNodeConstant::~deoglSASTNodeConstant(){
}



// Management
///////////////

void deoglSASTNodeConstant::SetConstantType( int constantType ){
	if( constantType < 0 || constantType >= ECT_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	pConstantType = constantType;
}

void deoglSASTNodeConstant::SetValueFloat( float value ){
	pValueFloat = value;
}

void deoglSASTNodeConstant::SetValueInt( int value ){
	pValueInt = value;
}



void deoglSASTNodeConstant::GenerateSourceCode( deoglSASTGenerateCode &gencode ){
	if( pConstantType == ectFloat ){
		decString buffer;
		
		buffer.AppendFormat( "%g", pValueFloat );
		if( ! strchr( buffer.GetString(), '.' ) && ! strchr( buffer.GetString(), 'e' ) ){
			buffer.Append( ".0" );
		}
		
		gencode.GetSourceCode().Append( buffer );
		
	}else if( pConstantType == ectInteger ){
		gencode.GetSourceCode().AppendFormat( "%i", pValueInt );
		
	}else if( pConstantType == ectTrue ){
		gencode.GetSourceCode().Append( "true" );
		
	}else{ // pConstantType == ectFalse
		gencode.GetSourceCode().Append( "false" );
	}
	
	gencode.SetRequiresEndOfCommand( true );
}
