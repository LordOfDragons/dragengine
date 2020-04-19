/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gdeParticleEmitter.h"
#include "../gdeGameDefinition.h"

#include <dragengine/common/exceptions.h>



// Class gdeParticleEmitter
/////////////////////////////

// Constructor, destructor
////////////////////////////

gdeParticleEmitter::gdeParticleEmitter() :
pGameDefinition( NULL ){
}

gdeParticleEmitter::gdeParticleEmitter( const char *path, const char *name ) :
pGameDefinition( NULL ),
pPath( path ),
pName( name ){
}

gdeParticleEmitter::gdeParticleEmitter( const gdeParticleEmitter &particleEmitter ) :
pGameDefinition( NULL ),

pPath( particleEmitter.pPath ),
pName( particleEmitter.pName ),
pDescription( particleEmitter.pDescription ),
pCategory( particleEmitter.pCategory ){
}

gdeParticleEmitter::~gdeParticleEmitter(){
	pCleanUp();
}



// Management
///////////////

void gdeParticleEmitter::SetGameDefinition( gdeGameDefinition *gamedef ){
	pGameDefinition = gamedef;
}



void gdeParticleEmitter::SetPath( const char *path ){
	if( pPath == path ){
		return;
	}
	
	pPath = path;
	
	if( pGameDefinition ){
		pGameDefinition->NotifyParticleEmitterChanged( this );
	}
}

void gdeParticleEmitter::SetName( const char *name ){
	if( pName == name ){
		return;
	}
	
	pName = name;
	
	if( pGameDefinition ){
		pGameDefinition->NotifyParticleEmitterNameChanged( this );
	}
}

void gdeParticleEmitter::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	
	if( pGameDefinition ){
		pGameDefinition->NotifyParticleEmitterChanged( this );
	}
}

void gdeParticleEmitter::SetCategory( const char *category ){
	if( pCategory == category ){
		return;
	}
	
	pCategory = category;
	
	if( pGameDefinition ){
		pGameDefinition->NotifyParticleEmitterChanged( this );
	}
}



// Private functions
//////////////////////

void gdeParticleEmitter::pCleanUp(){
	SetGameDefinition( NULL );
}
