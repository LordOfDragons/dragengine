/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglModelTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglModelTexture
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglModelTexture::deoglModelTexture( deoglRenderThread &renderThread ) :
pFirstFace( 0 ),
pFaceCount( 0 ),
pDoubleSided( false ),
pDecal( false ),
pDecalOffset( 0 ),
pRTIGroups( deoglSharedSPBRTIGroupList::Ref::New( new deoglSharedSPBRTIGroupList( renderThread ) ) ){
}

deoglModelTexture::~deoglModelTexture(){
}



// Management
///////////////

void deoglModelTexture::SetFirstFace( int faceIndex ){
	pFirstFace = faceIndex;
}

void deoglModelTexture::SetFaceCount( int faceCount ){
	pFaceCount = faceCount;
}

void deoglModelTexture::SetDoubleSided( bool doubleSided ){
	pDoubleSided = doubleSided;
}

void deoglModelTexture::SetDecal( bool decal ){
	pDecal = decal;
}

void deoglModelTexture::SetDecalOffset( int offset ){
	pDecalOffset = offset;
}
