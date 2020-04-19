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

#include "deoglShaderUnitSourceCode.h"

#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/exceptions.h>



// Class deoglShaderUnitSourceCode
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderUnitSourceCode::deoglShaderUnitSourceCode( const char *filePath, decBaseFileReader &fileReader ){
	if( ! filePath ) DETHROW( deeInvalidParam );
	
	int fileLength = fileReader.GetLength();
	
	pFilePath = NULL;
	pSourceCode = NULL;
	
	try{
		pFilePath = new char[ strlen( filePath ) + 1 ];
		if( ! pFilePath ) DETHROW( deeOutOfMemory );
		strcpy( pFilePath, filePath );
		
		pSourceCode = new char[ fileLength + 1 ];
		if( ! pSourceCode ) DETHROW( deeOutOfMemory );
		fileReader.Read( pSourceCode, fileLength );
		pSourceCode[ fileLength ] = '\0';
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglShaderUnitSourceCode::~deoglShaderUnitSourceCode(){
	pCleanUp();
}



// Private functions
//////////////////////

void deoglShaderUnitSourceCode::pCleanUp(){
	if( pSourceCode ) delete [] pSourceCode;
	if( pFilePath ) delete [] pFilePath;
}
