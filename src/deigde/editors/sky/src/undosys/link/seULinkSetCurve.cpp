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
#include <string.h>
#include <stdlib.h>

#include "seULinkSetCurve.h"
#include "../../sky/link/seLink.h"

#include <dragengine/common/exceptions.h>



// Class seULinkSetCurve
//////////////////////////

// Constructor, destructor
////////////////////////////

seULinkSetCurve::seULinkSetCurve( seLink *link, const decCurveBezier &newCurve ) :
pLink( NULL ),
pNewCurve( newCurve )
{
	if( ! link ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Set Link Curve" );
	
	pOldCurve = link->GetCurve();
	
	pLink = link;
	link->AddReference();
}

seULinkSetCurve::~seULinkSetCurve(){
	if( pLink ){
		pLink->FreeReference();
	}
}



// Management
///////////////

void seULinkSetCurve::SetNewCurve( const decCurveBezier &curve ){
	pNewCurve = curve;
}



void seULinkSetCurve::Undo(){
	pLink->GetCurve() = pOldCurve;
	pLink->NotifyCurveChanged();
}

void seULinkSetCurve::Redo(){
	pLink->GetCurve() = pNewCurve;
	pLink->NotifyCurveChanged();
}
