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
#include <string.h>
#include <stddef.h>

#include "deoglShaderProgram.h"
#include "deoglShaderProgramUsage.h"

#include <dragengine/common/exceptions.h>



// Class deoglShaderProgramUsage
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderProgramUsage::deoglShaderProgramUsage() :
pProgram( NULL ){
}

deoglShaderProgramUsage::deoglShaderProgramUsage( deoglShaderProgram *program ) :
pProgram( program )
{
	if( pProgram ){
		pProgram->AddUsage();
	}
}

deoglShaderProgramUsage::deoglShaderProgramUsage( const deoglShaderProgramUsage &usage ) :
pProgram( usage.pProgram )
{
	if( pProgram ){
		pProgram->AddUsage();
	}
}

deoglShaderProgramUsage::~deoglShaderProgramUsage(){
	if( pProgram ){
		pProgram->RemoveUsage();
	}
}

deoglShaderProgramUsage deoglShaderProgramUsage::New( deoglShaderProgram *program ){
	deoglShaderProgramUsage usage;
	usage.pProgram = program;
	return usage;
}



// Management
///////////////

bool deoglShaderProgramUsage::operator!() const{
	return pProgram == NULL;
}

deoglShaderProgramUsage::operator bool() const{
	return pProgram != NULL;
}

deoglShaderProgramUsage::operator deoglShaderProgram &() const{
	if( ! pProgram ){
		DETHROW( deeNullPointer );
	}
	return *pProgram;
}

deoglShaderProgram *deoglShaderProgramUsage::operator->() const{
	if( ! pProgram ){
		DETHROW( deeNullPointer );
	}
	return pProgram;
}

deoglShaderProgramUsage & deoglShaderProgramUsage::operator=( const deoglShaderProgramUsage &usage ){
	if( usage.pProgram == pProgram ){
		return *this;
	}
	
	if( pProgram ){
		pProgram->RemoveUsage();
	}
	
	pProgram = usage.pProgram;
	
	if( usage.pProgram ){
		usage.pProgram->AddUsage();
	}
	
	return *this;
}
