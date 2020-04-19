/* 
 * Drag[en]gine IGDE Sky Editor
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

#include "seLayer.h"
#include "seLayerList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seLayer
/////////////////////

// Constructor, destructor
////////////////////////////

seLayerList::seLayerList(){
}

seLayerList::~seLayerList(){
}



// Management
///////////////

int seLayerList::GetCount() const{
	return pLayers.GetCount();
}

seLayer *seLayerList::GetAt( int index ) const{
	return ( seLayer* )pLayers.GetAt( index );
}

int seLayerList::IndexOf( seLayer *layer ) const{
	return pLayers.IndexOf( layer );
}

bool seLayerList::Has( seLayer *layer ) const{
	return pLayers.Has( layer );
}

void seLayerList::Add( seLayer *layer ){
	if( ! layer ){
		DETHROW( deeInvalidParam );
	}
	pLayers.Add( layer );
}

void seLayerList::InsertAt( seLayer *layer, int index ){
	if( ! layer ){
		DETHROW( deeInvalidParam );
	}
	pLayers.Insert( layer, index );
}

void seLayerList::MoveTo( seLayer *layer, int index ){
	pLayers.Move( layer, index );
}

void seLayerList::Remove( seLayer *layer ){
	pLayers.Remove( layer );
}

void seLayerList::RemoveAll(){
	pLayers.RemoveAll();
}



seLayerList &seLayerList::operator=( const seLayerList &list ){
	pLayers = list.pLayers;
	return *this;
}
