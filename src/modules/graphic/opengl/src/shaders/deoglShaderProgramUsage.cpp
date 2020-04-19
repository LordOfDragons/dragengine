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



// Management
///////////////

void deoglShaderProgramUsage::TakeOver( deoglShaderProgram *program ){
	if( program == pProgram ){
		if( program ){
			// this is required since we are asked to take over the usage. since we
			// have the same usage already we refuse to take over the usage and
			// thus without decreasing it this usage would be dangling
			program->RemoveUsage();
		}
		return;
	}
	
	if( pProgram ){
		pProgram->RemoveUsage();
	}
	pProgram = program;
}

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

deoglShaderProgramUsage &deoglShaderProgramUsage::operator=( deoglShaderProgram *program ){
	if( program == pProgram ){
		return *this;
	}
	
	if( pProgram ){
		pProgram->RemoveUsage();
	}
	
	pProgram = program;
	
	if( program ){
		program->AddUsage();
	}
	
	return *this;
}

deoglShaderProgramUsage & deoglShaderProgramUsage::operator=( const deoglShaderProgramUsage &usage ){
	return operator=( usage.pProgram );
}
