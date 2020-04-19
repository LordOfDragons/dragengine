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
#include <stdlib.h>

#include "reBaseUndoScale.h"



// class reBaseUndoScale
//////////////////////////

// Constructor, destructor
////////////////////////////

reBaseUndoScale::reBaseUndoScale(){
	pModifyPosition = true;
	pModifySize = true;
	pFactors.Set( 1.0f, 1.0f, 1.0f );
	Update();
}

reBaseUndoScale::~reBaseUndoScale(){
}



// Management
///////////////

void reBaseUndoScale::SetModifyPosition( bool modifyPosition ){
	pModifyPosition = modifyPosition;
}

void reBaseUndoScale::SetModifySize( bool modifySize ){
	pModifySize = modifySize;
}

void reBaseUndoScale::SetFactors( const decVector &factors ){
	pFactors = factors;
	Update();
}

void reBaseUndoScale::SetCenter( const decVector &center ){
	pCenter = center;
	Update();
}

void reBaseUndoScale::Update(){
	// matrix... TODO
	/*
	return decMatrix::CreateTranslation( -p_center )
		* decMatrix::CreateRotationZ( -p_viewRot.z )
		* decMatrix::CreateRotationY( -p_viewRot.y )
		* decMatrix::CreateRotationX( -p_viewRot.x )
		* decMatrix::CreateScale( pScaleX, pScaleY, 1 )
		* decMatrix::CreateRotationX( p_viewRot.x )
		* decMatrix::CreateRotationY( p_viewRot.y )
		* decMatrix::CreateRotationZ( p_viewRot.z )
		* decMatrix::CreateTranslation( p_center );
	*/
	
	// set informations
	decString info;
	
	info.Format( "factors(%g,%g,%g) center(%g,%g,%g)", pFactors.x, pFactors.y, pFactors.z, pCenter.x, pCenter.y, pCenter.z );
	SetLongInfo( info.GetString() );
}



// Undo and Redo actions
//////////////////////////

void reBaseUndoScale::ProgressiveRedo(){
	Undo();
	Redo();
}
