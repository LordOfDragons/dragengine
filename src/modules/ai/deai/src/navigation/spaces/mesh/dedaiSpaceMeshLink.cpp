/* 
 * Drag[en]gine AI Module
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

#include "dedaiSpaceMeshLink.h"
#include "dedaiSpaceMesh.h"

#include <dragengine/common/exceptions.h>



// Class dedaiSpaceMeshLink
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiSpaceMeshLink::dedaiSpaceMeshLink() :
pMesh( NULL ),
pFace( 0 ),
pCorner( 0 ){
}

dedaiSpaceMeshLink::~dedaiSpaceMeshLink(){
}



// Management
///////////////

void dedaiSpaceMeshLink::SetMesh( dedaiSpaceMesh *mesh ){
	pMesh = mesh;
}

void dedaiSpaceMeshLink::SetFace( unsigned short face ){
	pFace = face;
}

void dedaiSpaceMeshLink::SetCorner( unsigned short corner ){
	pCorner = corner;
}

void dedaiSpaceMeshLink::SetTransform( const decMatrix &transform ){
	pTransform = transform;
}



bool dedaiSpaceMeshLink::Matches( dedaiSpaceMesh *mesh, unsigned short face, unsigned short corner ) const{
	return pMesh == mesh && pFace == face && pCorner == corner;
}

dedaiSpaceMeshLink &dedaiSpaceMeshLink::GetForeignLink() const{
	return pMesh->GetLinkWith( *this );
}



// Operators
//////////////

dedaiSpaceMeshLink &dedaiSpaceMeshLink::operator=( const dedaiSpaceMeshLink &link ){
	pMesh = link.pMesh;
	pFace = link.pFace;
	pCorner = link.pCorner;
	pTransform = link.pTransform;
	return *this;
}
