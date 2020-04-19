/* 
 * Drag[en]gine AI Module
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

#include "dedaiCostTable.h"

#include <dragengine/common/exceptions.h>



// Class dedaiCostTable
/////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiCostTable::dedaiCostTable() :
pChanged( false ){
}

dedaiCostTable::~dedaiCostTable(){
}



// Management
///////////////

void dedaiCostTable::ClearChanged(){
	pChanged = false;
}

int dedaiCostTable::GetTypeCount() const{
	return pTypes.GetCount();
}

int dedaiCostTable::GetTypeAt( int index ) const{
	return pTypes.GetAt( index );
}

int dedaiCostTable::GetTypeWith( int typeNumber ){
	int index = pTypes.IndexOf( typeNumber );
	
	if( index == -1 ){
		index = pTypes.GetCount();
		pTypes.Add( typeNumber );
	}
	
	return index;
}

int dedaiCostTable::GetTypeWith( int typeNumber, int defaultValue ) const{
	const int index = pTypes.IndexOf( typeNumber );
	if( index == -1 ){
		return defaultValue;
	}
	return index;
}

void dedaiCostTable::RemoveAllTypes(){
	pTypes.RemoveAll();
}
