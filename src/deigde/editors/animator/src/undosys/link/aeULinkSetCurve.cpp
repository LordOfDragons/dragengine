/* 
 * Drag[en]gine IGDE Animator Editor
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

#include "aeULinkSetCurve.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/link/aeLink.h"

#include <dragengine/common/exceptions.h>



// Class aeULinkSetCurve
//////////////////////////

// Constructor, destructor
////////////////////////////

aeULinkSetCurve::aeULinkSetCurve( aeLink *link, const decCurveBezier &newCurve ) :
pLink( link ),
pOldCurve( ( ( aeLink& )( deObject& )pLink ).GetCurve() ),
pNewCurve( newCurve )
{
	SetShortInfo( "Set Link Curve" );
}

aeULinkSetCurve::~aeULinkSetCurve(){
}



// Management
///////////////

void aeULinkSetCurve::SetNewCurve( const decCurveBezier &curve ){
	pNewCurve = curve;
}



void aeULinkSetCurve::Undo(){
	( ( aeLink& )( deObject& )pLink ).SetCurve( pOldCurve );
}

void aeULinkSetCurve::Redo(){
	( ( aeLink& )( deObject& )pLink ).SetCurve( pNewCurve );
}

void aeULinkSetCurve::ProgressiveRedo(){
	Redo();
}
