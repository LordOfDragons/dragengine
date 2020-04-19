/* 
 * Drag[en]gine IGDE
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

#include "igdeGDCEnvMapProbe.h"
#include "igdeGDCEnvMapProbeList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCEnvMapProbe
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCEnvMapProbeList::igdeGDCEnvMapProbeList(){
}

igdeGDCEnvMapProbeList::igdeGDCEnvMapProbeList( const igdeGDCEnvMapProbeList &list ){
	pProbes = list.pProbes;
}

igdeGDCEnvMapProbeList::~igdeGDCEnvMapProbeList(){
}



// Management
///////////////

int igdeGDCEnvMapProbeList::GetCount() const{
	return pProbes.GetCount();
}

igdeGDCEnvMapProbe *igdeGDCEnvMapProbeList::GetAt( int index ) const{
	return ( igdeGDCEnvMapProbe* )pProbes.GetAt( index );
}

int igdeGDCEnvMapProbeList::IndexOf( igdeGDCEnvMapProbe *probe ) const{
	return pProbes.IndexOf( probe );
}

bool igdeGDCEnvMapProbeList::Has( igdeGDCEnvMapProbe *probe ) const{
	return pProbes.Has( probe );
}

void igdeGDCEnvMapProbeList::Add( igdeGDCEnvMapProbe *probe ){
	if( ! probe ){
		DETHROW( deeInvalidParam );
	}
	
	pProbes.Add( probe );
}

void igdeGDCEnvMapProbeList::InsertAt( igdeGDCEnvMapProbe *probe, int index ){
	if( ! probe ){
		DETHROW( deeInvalidParam );
	}
	
	pProbes.Insert( probe, index );
}

void igdeGDCEnvMapProbeList::MoveTo( igdeGDCEnvMapProbe *probe, int index ){
	pProbes.Move( probe, index );
}

void igdeGDCEnvMapProbeList::Remove( igdeGDCEnvMapProbe *probe ){
	pProbes.Remove( probe );
}

void igdeGDCEnvMapProbeList::RemoveAll(){
	pProbes.RemoveAll();
}



void igdeGDCEnvMapProbeList::SetToDeepCopyFrom( const igdeGDCEnvMapProbeList &list ){
	const int count = list.GetCount();
	igdeGDCEnvMapProbe *probe = NULL;
	
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			probe = new igdeGDCEnvMapProbe( *list.GetAt( i ) );
			Add( probe );
			probe->FreeReference();
			probe = NULL;
		}
		
	}catch( const deException & ){
		if( probe ){
			probe->FreeReference();
		}
		throw;
	}
}



// Operators
//////////////

igdeGDCEnvMapProbeList &igdeGDCEnvMapProbeList::operator=( const igdeGDCEnvMapProbeList &list ){
	pProbes = list.pProbes;
	return *this;
}
