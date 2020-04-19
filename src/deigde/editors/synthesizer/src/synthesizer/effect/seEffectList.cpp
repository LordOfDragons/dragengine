/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#include "seEffect.h"
#include "seEffectList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seEffect
///////////////////

// Constructor, destructor
////////////////////////////

seEffectList::seEffectList(){
}

seEffectList::seEffectList( const seEffectList &copy ) :
pEffects( copy.pEffects ){
}

seEffectList::~seEffectList(){
}



// Management
///////////////

int seEffectList::GetCount() const{
	return pEffects.GetCount();
}

seEffect *seEffectList::GetAt( int index ) const{
	return ( seEffect* )pEffects.GetAt( index );
}

int seEffectList::IndexOf( seEffect *effect ) const{
	return pEffects.IndexOf( effect );
}

bool seEffectList::Has( seEffect *effect ) const{
	return pEffects.Has( effect );
}

void seEffectList::Add( seEffect *effect ){
	if( ! effect || Has( effect ) ){
		DETHROW( deeInvalidParam );
	}
	pEffects.Add( effect );
}

void seEffectList::Insert( seEffect *effect, int index ){
	if( ! effect || Has( effect ) ){
		DETHROW( deeInvalidParam );
	}
	pEffects.Insert( effect, index );
}

void seEffectList::Move( seEffect *effect, int index ){
	pEffects.Move( effect, index );
}

void seEffectList::Remove( seEffect *effect ){
	pEffects.Remove( effect );
}

void seEffectList::RemoveAll(){
	pEffects.RemoveAll();
}



// Operators
//////////////

seEffectList &seEffectList::operator=( const seEffectList &list ){
	pEffects = list.pEffects;
	return *this;
}

seEffectList &seEffectList::operator+=( const seEffectList &list ){
	pEffects += list.pEffects;
	return *this;
}
