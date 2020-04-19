/* 
 * Drag[en]gine OpenAL Audio Module
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

#include "deoalWOVCollectElements.h"
#include "../../../component/deoalAComponent.h"
#include "../../../model/deoalAModel.h"
#include "../../../world/octree/deoalWorldOctree.h"

#include <dragengine/common/exceptions.h>



// Class deoalWOVCollectElements
//////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalWOVCollectElements::deoalWOVCollectElements()
{
	SetVisitAll( false );
	SetVisitComponents( true );
}

deoalWOVCollectElements::~deoalWOVCollectElements(){
}



// Visiting
/////////////

void deoalWOVCollectElements::SetBox( const decDVector &minimum, const decDVector &maximum ){
	pBoxMin = minimum;
	pBoxMax = maximum;
}

void deoalWOVCollectElements::SetBoxRange( const decDVector &position, double range ){
	const decDVector rangeVector( range, range, range );
	pBoxMin = position - rangeVector;
	pBoxMax = position + rangeVector;
}

void deoalWOVCollectElements::Reset(){
	pComponents.RemoveAll();
}



int deoalWOVCollectElements::GetComponentCount() const{
	return pComponents.GetCount();
}

deoalAComponent *deoalWOVCollectElements::GetComponentAt( int index ) const{
	return ( deoalAComponent* )pComponents.GetAt( index );
}



void deoalWOVCollectElements::VisitComponent( deoalAComponent *component ){
	if( ! component->GetAffectsSound() ){
		return;
	}
	if( component->GetMaxExtend() < pBoxMin || component->GetMinExtend() > pBoxMax ){
		return;
	}
	if( ! component->GetModel() || ! component->GetModel()->GetOctree() ){
		return;
	}
	
	pComponents.Add( component );
}
