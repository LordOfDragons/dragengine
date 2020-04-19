/* 
 * Drag[en]gine Game Engine
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

#include "deSynthesizerSourceChain.h"
#include "deSynthesizerSourceVisitor.h"
#include "../../sound/deSound.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deSynthesizerSourceChain
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerSourceChain::deSynthesizerSourceChain() :
pMinSpeed( 1.0f ),
pMaxSpeed( 1.0f ){
}

deSynthesizerSourceChain::~deSynthesizerSourceChain(){
}



// Management
///////////////

int deSynthesizerSourceChain::GetSoundCount() const{
	return pSounds.GetCount();
}

deSound *deSynthesizerSourceChain::GetSoundAt( int index ) const{
	return ( deSound* )pSounds.GetAt( index );
}

void deSynthesizerSourceChain::AddSound( deSound *sound ){
	pSounds.Add( sound );
}

void deSynthesizerSourceChain::RemoveSound( int index ){
	pSounds.RemoveFrom( index );
}

void deSynthesizerSourceChain::RemoveAllSounds(){
	pSounds.RemoveAll();
}



void deSynthesizerSourceChain::SetMinSpeed( float speed ){
	pMinSpeed = speed;
}

void deSynthesizerSourceChain::SetMaxSpeed( float speed ){
	pMaxSpeed = speed;
}



// Visiting
/////////////

void deSynthesizerSourceChain::Visit( deSynthesizerSourceVisitor &visitor ){
	visitor.VisitChain( *this );
}
