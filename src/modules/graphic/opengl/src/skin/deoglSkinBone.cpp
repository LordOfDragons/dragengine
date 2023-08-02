/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglSkinBone.h"
#include "state/deoglSkinState.h"

#include <dragengine/common/exceptions.h>


// Class deoglSkinBone
////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinBone::deoglSkinBone( const decString &name ) :
pName( name ),
pBone( -1 ){
}

deoglSkinBone::~deoglSkinBone(){
}



// Management
///////////////

void deoglSkinBone::SetBone( int bone ){
	DEASSERT_TRUE( bone >= -1 )
	pBone = bone;
}
