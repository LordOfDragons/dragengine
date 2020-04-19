/* 
 * Drag[en]gine IGDE Rig Editor
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

#include "reBaseUndoRotate.h"



// class reBaseUndoRotate
///////////////////////////

// Constructor, destructor
////////////////////////////

reBaseUndoRotate::reBaseUndoRotate(){
	pModifyPosition = true;
	pModifyOrientation = true;
	SetShortInfo( "Rotate ?" );
	pAxis = decVector( 0.0f, 0.0f, 1.0f );
}

reBaseUndoRotate::~reBaseUndoRotate(){
}



// Management
///////////////

void reBaseUndoRotate::SetAngle( float angle ){
	pAngle = angle;
	Update();
}

void reBaseUndoRotate::SetCenterSector( const decPoint3 &sector ){
	pCenterSector = sector;
	Update();
}

void reBaseUndoRotate::SetCenterPosition( const decVector &position ){
	pCenterPosition = position;
	Update();
}

void reBaseUndoRotate::SetAxis( const decVector &axis ){
	pAxis = axis;
	pAxis.Normalize();
	Update();
}

void reBaseUndoRotate::SetModifyPosition( bool modifyPosition ){
	pModifyPosition = modifyPosition;
}

void reBaseUndoRotate::SetModifyOrientation( bool modifyOrientation ){
	pModifyOrientation = modifyOrientation;
}

void reBaseUndoRotate::Update(){
	pMatrix = decMatrix::CreateTranslation( -pCenterPosition )
		* decMatrix::CreateRotationAxis( pAxis, pAngle )
		* decMatrix::CreateTranslation( pCenterPosition );
	
	decString info;
	info.Format( "axis(%g,%g,%g) center(%i,%i,%i - %g,%g,%g) angle=%g°",
		pAxis.x, pAxis.y, pAxis.z, pCenterSector.x, pCenterSector.y, pCenterSector.z,
		pCenterPosition.x, pCenterPosition.y, pCenterPosition.z, pAngle / DEG2RAD );
	SetLongInfo( info.GetString() );
}



// Undo and Redo actions
//////////////////////////

void reBaseUndoRotate::ProgressiveRedo(){
	Undo();
	Redo();
}
