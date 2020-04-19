/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeOCEnvMapProbe.h"
#include "gdeOCEnvMapProbeList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeOCEnvMapProbe
///////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCEnvMapProbeList::gdeOCEnvMapProbeList(){
}

gdeOCEnvMapProbeList::gdeOCEnvMapProbeList( const gdeOCEnvMapProbeList &list ) :
pProbes( list.pProbes ){
}

gdeOCEnvMapProbeList::~gdeOCEnvMapProbeList(){
}



// Management
///////////////

int gdeOCEnvMapProbeList::GetCount() const{
	return pProbes.GetCount();
}

gdeOCEnvMapProbe *gdeOCEnvMapProbeList::GetAt( int index ) const{
	return ( gdeOCEnvMapProbe* )pProbes.GetAt( index );
}

int gdeOCEnvMapProbeList::IndexOf( gdeOCEnvMapProbe *probe ) const{
	return pProbes.IndexOf( probe );
}

bool gdeOCEnvMapProbeList::Has( gdeOCEnvMapProbe *probe ) const{
	return pProbes.Has( probe );
}

void gdeOCEnvMapProbeList::Add( gdeOCEnvMapProbe *probe ){
	if( ! probe || Has( probe ) ){
		DETHROW( deeInvalidParam );
	}
	pProbes.Add( probe );
}

void gdeOCEnvMapProbeList::Remove( gdeOCEnvMapProbe *probe ){
	pProbes.Remove( probe );
}

void gdeOCEnvMapProbeList::RemoveAll(){
	pProbes.RemoveAll();
}



gdeOCEnvMapProbeList &gdeOCEnvMapProbeList::operator=( const gdeOCEnvMapProbeList &list ){
	pProbes = list.pProbes;
	return *this;
}
