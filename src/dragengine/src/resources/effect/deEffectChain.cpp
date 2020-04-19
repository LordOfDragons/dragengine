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
#include "deEffect.h"
#include "deEffectChain.h"
#include "deEffectVisitor.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// class deEffectChain
////////////////////////

// constructor, destructor
////////////////////////////

deEffectChain::deEffectChain(){
	pEffects = NULL;
	pEffectCount = 0;
	pEffectSize = 0;
}

deEffectChain::~deEffectChain(){
	RemoveAllEffects();
	if( pEffects ) delete [] pEffects;
}



// Effect management
//////////////////////

deEffect *deEffectChain::GetEffectAt( int index ) const{
	if( index < 0 || index >= pEffectCount ) DETHROW( deeInvalidParam );
	return pEffects[ index ];
}

bool deEffectChain::HasEffect( deEffect *effect ) const{
	return IndexOfEffect( effect ) != -1;
}

int deEffectChain::IndexOfEffect( deEffect *effect ) const{
	if( ! effect ) DETHROW( deeInvalidParam );
	int i;
	for( i=0; i<pEffectCount; i++ ){
		if( pEffects[ i ] == effect ) return i;
	}
	return -1;
}

void deEffectChain::AddEffect( deEffect *effect ){
	InsertEffect( effect, pEffectCount );
}

void deEffectChain::InsertEffect( deEffect *effect, int position ){
	if( HasEffect( effect ) || position < 0 || position > pEffectCount ) DETHROW( deeInvalidParam );
	int i;
	if( pEffectCount == pEffectSize ){
		int newSize = pEffectSize * 3 / 2 + 1;
		deEffect **newArray = new deEffect*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pEffects ){
			for( i=0; i<pEffectSize; i++ ){
				newArray[ i ] = pEffects[ i ];
			}
			delete [] pEffects;
		}
		pEffects = newArray;
		pEffectSize = newSize;
	}
	for( i=pEffectCount; i>position; i-- ){
		pEffects[ i ] = pEffects[ i - 1 ];
	}
	pEffects[ position ] = effect;
	pEffectCount++;
	effect->AddReference();
}

void deEffectChain::RemoveEffect( deEffect *effect ){
	int i, index = IndexOfEffect( effect );
	if( index == -1 ) DETHROW( deeInvalidParam );
	for( i=index; i<pEffectCount-1; i++ ){
		pEffects[ i ] = pEffects[ i + 1 ];
	}
	pEffects[ pEffectCount - 1 ] = NULL;
	pEffectCount--;
	effect->FreeReference();
}

void deEffectChain::RemoveAllEffects(){
	while( pEffectCount > 0 ){
		pEffects[ pEffectCount - 1 ]->FreeReference();
		pEffects[ pEffectCount - 1 ] = NULL;
		pEffectCount--;
	}
}

void deEffectChain::MoveEffect( deEffect *effect, int newPosition ){
	DETHROW( deeInvalidParam );
}



// Visiting
/////////////

void deEffectChain::VisitEffects( deEffectVisitor &visitor ){
	int i;
	for( i=0; i<pEffectCount; i++ ){
		pEffects[ i ]->Visit( visitor );
	}
}
