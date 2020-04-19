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

#include "deoglShaderGenerator.h"
#include "deoglSGeneratorUnit.h"
#include "../deoglShaderProgram.h"
#include "../deoglShaderUnitSourceCode.h"
#include "../deoglShaderSources.h"

#include <dragengine/common/exceptions.h>



// Class deoglShaderGenerator
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderGenerator::deoglShaderGenerator( const char *name ){
	pName = name;
	pUnitVertex = NULL;
	pUnitGeometry = NULL;
	pUnitFragment = NULL;
	
	pSources = NULL;
	pShader = NULL;
}

deoglShaderGenerator::~deoglShaderGenerator(){
	pCleanUp();
}



// Management
///////////////

void deoglShaderGenerator::SetUnitVertex( deoglSGeneratorUnit *unit ){
	if( unit != pUnitVertex ){
		if( pUnitVertex ){
			pUnitVertex->FreeReference();
		}
		
		pUnitVertex = unit;
		
		if( unit ){
			unit->AddReference();
		}
	}
}

void deoglShaderGenerator::SetUnitGeometry( deoglSGeneratorUnit *unit ){
	if( unit != pUnitGeometry ){
		if( pUnitGeometry ){
			pUnitGeometry->FreeReference();
		}
		
		pUnitGeometry = unit;
		
		if( unit ){
			unit->AddReference();
		}
	}
}

void deoglShaderGenerator::SetUnitFragment( deoglSGeneratorUnit *unit ){
	if( unit != pUnitFragment ){
		if( pUnitFragment ){
			pUnitFragment->FreeReference();
		}
		
		pUnitFragment = unit;
		
		if( unit ){
			unit->AddReference();
		}
	}
}



void deoglShaderGenerator::GenerateShader(){
}

void deoglShaderGenerator::FreeShader(){
	if( pShader ){
		delete pShader;
		pShader = NULL;
	}
	if( pSources ){
		delete pSources;
		pSources = NULL;
	}
}



// Private functions
//////////////////////

void deoglShaderGenerator::pCleanUp(){
	FreeShader();
}
