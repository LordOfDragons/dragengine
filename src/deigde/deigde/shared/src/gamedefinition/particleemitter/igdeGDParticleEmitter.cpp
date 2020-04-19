/* 
 * Drag[en]gine IGDE
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

#include "igdeGDParticleEmitter.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>



// Class igdeGDParticleEmitter
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDParticleEmitter::igdeGDParticleEmitter( const char *path, const char *name ) :
pPath( path ),
pName( name ){
}

igdeGDParticleEmitter::igdeGDParticleEmitter( const igdeGDParticleEmitter &particleEmitter ) :
pPath( particleEmitter.pPath ),
pName( particleEmitter.pName ),
pDescription( particleEmitter.pDescription ),
pCategory( particleEmitter.pCategory ){
}

igdeGDParticleEmitter::~igdeGDParticleEmitter(){
}



// Management
///////////////

void igdeGDParticleEmitter::SetPath( const char *path ){
	pPath = path;
}

void igdeGDParticleEmitter::SetName( const char *name ){
	pName = name;
}

void igdeGDParticleEmitter::SetDescription( const char *description ){
	pDescription = description;
}

void igdeGDParticleEmitter::SetCategory( const char *category ){
	pCategory = category;
}

deImage *igdeGDParticleEmitter::GetPreviewImage( int size ) const{
	if( size < 0 || size > 2 ){
		DETHROW( deeInvalidParam );
	}
	return pPreviewImages[ size ];
}

void igdeGDParticleEmitter::SetPreviewImage( int size, deImage *image ){
	if( size < 0 || size > 2 ){
		DETHROW( deeInvalidParam );
	}
	pPreviewImages[ size ] = image;
}
