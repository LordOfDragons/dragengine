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

#include "deSynthesizerEffectStretch.h"
#include "deSynthesizerEffectVisitor.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/math/decMath.h"



// Class deSynthesizerEffectStretch
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerEffectStretch::deSynthesizerEffectStretch() :
pMinTime( 0.0f ),
pMaxTime( 0.0f ),
pMinPitch( 0.0f ),
pMaxPitch( 0.0f ){
}

deSynthesizerEffectStretch::~deSynthesizerEffectStretch(){
}



// Management
///////////////

void deSynthesizerEffectStretch::SetMinTime( float stretch ){
	pMinTime = stretch;
}

void deSynthesizerEffectStretch::SetMaxTime( float stretch ){
	pMaxTime = stretch;
}

void deSynthesizerEffectStretch::SetMinPitch( float shift ){
	pMinPitch = shift;
}

void deSynthesizerEffectStretch::SetMaxPitch( float shift ){
	pMaxPitch = shift;
}



// Visiting
/////////////

void deSynthesizerEffectStretch::Visit( deSynthesizerEffectVisitor &visitor ){
	visitor.VisitStretch( *this );
}
