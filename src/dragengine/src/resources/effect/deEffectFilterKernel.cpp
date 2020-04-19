/* 
 * Drag[en]gine Game Engine
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

// includes
#include <stdio.h>
#include <stdlib.h>
#include "deEffectFilterKernel.h"
#include "deEffectVisitor.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/graphic/deBaseGraphicEffect.h"



// class deEffectFilterKernel
///////////////////////////////

// constructor, destructor
////////////////////////////

deEffectFilterKernel::deEffectFilterKernel( deEffectManager *manager ) :
deEffect( manager ),
pKernel( NULL ),
pKernelRows( 0 ),
pKernelCols( 0 ),
pScale( 1.0f )
{
	SetKernelSize( 1, 1 );
}

deEffectFilterKernel::~deEffectFilterKernel(){
	if( pKernel ){
		delete [] pKernel;
	}
}



// Management
///////////////

void deEffectFilterKernel::SetKernelSize( int rows, int cols ){
	if( rows < 1 || cols < 1 ){
		DETHROW( deeInvalidParam );
	}
	if( ( rows % 2 ) == 0 || ( cols % 2 ) == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( rows == pKernelRows && cols == pKernelCols ){
		return;
	}
	
	int i, count = rows * cols;
	float *newKernel = new float[ count ];
	if( ! newKernel ) DETHROW( deeOutOfMemory );
	if( pKernel ) delete [] pKernel;
	pKernel = newKernel;
	pKernelRows = rows;
	pKernelCols = cols;
	for( i=0; i<count; i++ ){
		pKernel[ i ] = 0.0f;
	}
	pKernel[ ( ( rows - 1 ) / 2  ) * cols + ( cols - 1 ) / 2 ] = 1.0f;
	
	deBaseGraphicEffect * const graEffect = GetPeerGraphic();
	if( graEffect ){
		graEffect->FilterKernelChanged();
	}
}

float deEffectFilterKernel::GetKernelValueAt( int row, int col ) const{
	if( row < 0 || row >= pKernelRows ){
		DETHROW( deeInvalidParam );
	}
	if( col < 0 || col >= pKernelCols ){
		DETHROW( deeInvalidParam );
	}
	
	return pKernel[ row * pKernelCols + col ];
}

void deEffectFilterKernel::SetKernelValueAt( int row, int col, float value ){
	if( row < 0 || row >= pKernelRows ){
		DETHROW( deeInvalidParam );
	}
	if( col < 0 || col >= pKernelCols ){
		DETHROW( deeInvalidParam );
	}
	
	pKernel[ row * pKernelCols + col ] = value;
	
	deBaseGraphicEffect * const graEffect = GetPeerGraphic();
	if( graEffect ){
		graEffect->FilterKernelChanged();
	}
}

void deEffectFilterKernel::SetScale( float scale ){
	scale = decMath::max( scale, 0.0f );
	if( fabsf( scale - pScale ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pScale = scale;
	
	deBaseGraphicEffect * const graEffect = GetPeerGraphic();
	if( graEffect ){
		graEffect->FilterKernelChanged();
	}
}



// visiting
/////////////

void deEffectFilterKernel::Visit( deEffectVisitor &visitor ){
	visitor.VisitFilterKernel( *this );
}
