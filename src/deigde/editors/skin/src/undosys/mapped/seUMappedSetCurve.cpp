/* 
 * Drag[en]gine IGDE Skin Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "seUMappedSetCurve.h"

#include <dragengine/common/exceptions.h>



// Class seUMappedSetCurve
////////////////////////////

// Constructor, destructor
////////////////////////////

seUMappedSetCurve::seUMappedSetCurve( seMapped *mapped, const decCurveBezier &newCurve ) :
pMapped( mapped ),
pOldCurve( mapped ? mapped->GetCurve() : decCurveBezier() ),
pNewCurve( newCurve )
{
	DEASSERT_NOTNULL( pMapped )
	
	SetShortInfo( "Set Mapped Curve" );
}

seUMappedSetCurve::~seUMappedSetCurve(){
}



// Management
///////////////

void seUMappedSetCurve::SetCurve( const decCurveBezier &curve ){
	pNewCurve = curve;
}

void seUMappedSetCurve::Undo(){
	pMapped->SetCurve( pOldCurve );
}

void seUMappedSetCurve::Redo(){
	pMapped->SetCurve( pNewCurve );
}
