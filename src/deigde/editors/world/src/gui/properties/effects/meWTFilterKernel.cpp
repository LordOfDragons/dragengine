/* 
 * Drag[en]gine IGDE World Editor
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

#if 0

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "meWTFilterKernel.h"
#include "dragengine/common/exceptions.h"
#include <dragengine/common/math/decMath.h>



// Class meWTFilterKernel
///////////////////////////

// Constructor, destructor
////////////////////////////

meWTFilterKernel::meWTFilterKernel( const char *name, int rows, int cols, float scale ){
	if( ! name || ! name[ 0 ] || rows < 1 || cols < 1 ) DETHROW( deeInvalidParam );
	if( ( rows % 2 ) == 0 || ( cols % 2 ) == 0 ) DETHROW( deeInvalidParam );
	int i, size;
	
	pName = NULL;
	pKernel = NULL;
	pKernelRows = rows;
	pKernelCols = cols;
	pScale = decMath::max( scale, 0.0f );
	
	try{
		pName = new char[ strlen( name ) + 1 ];
		if( ! pName ) DETHROW( deeOutOfMemory );
		strcpy( pName, name );
		
		pKernel = new float[ rows * cols ];
		if( ! pKernel ) DETHROW( deeOutOfMemory );
		
		size = rows * cols;
		for( i=0; i<size; i++ ) pKernel[ i ] = 0.0f;
		pKernel[ ( ( rows - 1 ) / 2  ) * cols + ( cols - 1 ) / 2 ] = 1.0f;
		
	}catch( const deException & ){
		if( pKernel ) delete [] pKernel;
		if( pName ) delete [] pName;
		throw;
	}
}

meWTFilterKernel::~meWTFilterKernel(){
	if( pKernel ) delete [] pKernel;
	if( pName ) delete [] pName;
}



// Management
///////////////

float meWTFilterKernel::GetKernelValueAt( int row, int col ) const{
	if( row < 0 || row >= pKernelRows ) DETHROW( deeInvalidParam );
	if( col < 0 || col >= pKernelCols ) DETHROW( deeInvalidParam );
	return pKernel[ row * pKernelCols + col ];
}

void meWTFilterKernel::SetKernelValueAt( int row, int col, float value ){
	if( row < 0 || row >= pKernelRows ) DETHROW( deeInvalidParam );
	if( col < 0 || col >= pKernelCols ) DETHROW( deeInvalidParam );
	pKernel[ row * pKernelCols + col ] = value;
}

void meWTFilterKernel::SetScale( float scale ){
	pScale = scale;
}

#endif
