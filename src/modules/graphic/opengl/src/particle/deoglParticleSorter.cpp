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

#include "deoglParticleSorter.h"
#include "deoglRParticleEmitterInstance.h"
#include "../rendering/task/deoglAddToRenderTaskParticles.h"

#include <dragengine/common/exceptions.h>



// Class deoglParticleSorter
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglParticleSorter::deoglParticleSorter(){
	pParticles = NULL;
	pParticleCount = 0;
	pParticleSize = 0;
}

deoglParticleSorter::~deoglParticleSorter(){
	if( pParticles ){
		while( pParticleSize > 0 ){
			pParticleSize--;
			if( pParticles[ pParticleSize ] ){
				delete pParticles[ pParticleSize ];
			}
		}
		
		delete [] pParticles;
	}
}



// Management
///////////////

const deoglParticleSorter::sParticle &deoglParticleSorter::GetParticleAt( int index ) const{
	if( index < 0 || index >= pParticleCount ){
		DETHROW( deeInvalidParam );
	}
	
	return *pParticles[ index ];
}

void deoglParticleSorter::AddToRenderTask( deoglAddToRenderTaskParticles &renderTask ){
	deoglRParticleEmitterInstance *instance;
	int p;
	
	for( p=0; p<pParticleCount; p++ ){
		instance = pParticles[ p ]->instance;
		
		renderTask.AddParticle( *instance, instance->GetParticles() + pParticles[ p ]->particle );
	}
}

void deoglParticleSorter::Clear(){
	pParticleCount = 0;
}

void deoglParticleSorter::AddParticle( deoglRParticleEmitterInstance *instance, int particle, float distance ){
	if( pParticleCount == pParticleSize ){
		int newSize = pParticleSize + 10;
		sParticle **newArray = new sParticle*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pParticles ){
			memcpy( newArray, pParticles, sizeof( sParticle* ) * pParticleSize );
			delete [] pParticles;
		}
		pParticles = newArray;
		for( ; pParticleSize<newSize; pParticleSize++ ){
			pParticles[ pParticleSize ] = new sParticle;
			if( ! pParticles[ pParticleSize ] ) DETHROW( deeOutOfMemory );
		}
	}
	
	pParticles[ pParticleCount ]->instance = instance;
	pParticles[ pParticleCount ]->particle = particle;
	pParticles[ pParticleCount ]->distance = distance;
	pParticleCount++;
}

void deoglParticleSorter::Sort(){
	if( pParticleCount > 0 ){
		pSortParticle( 0, pParticleCount - 1 );
	}
}



// Private Functions
//////////////////////

void deoglParticleSorter::pSortParticle( int left, int right ){
	sParticle *pivotParticle = pParticles[ left ];
	float pivot = pivotParticle->distance;
	int r_hold = right;
	int l_hold = left;
	
	while( left < right ){
		while( ( pParticles[ right ]->distance <= pivot ) && ( left < right ) ){
			right--;
		}
		if( left != right ){
			pParticles[ left ] = pParticles[ right ];
			left++;
		}
		while( ( pParticles[ left ]->distance >= pivot ) && ( left < right ) ){
			left++;
		}
		if( left != right ){
			pParticles[ right ] = pParticles[ left ];
			right--;
		}
	}
	
	pParticles[ left ] = pivotParticle;
	if( l_hold < left ){
		pSortParticle( l_hold, left - 1 );
	}
	if( r_hold > left ){
		pSortParticle( left + 1, r_hold );
	}
}
