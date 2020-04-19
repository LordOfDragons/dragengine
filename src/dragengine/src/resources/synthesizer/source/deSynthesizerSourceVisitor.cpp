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

#include "deSynthesizerSource.h"
#include "deSynthesizerSourceChain.h"
#include "deSynthesizerSourceGroup.h"
#include "deSynthesizerSourceSound.h"
#include "deSynthesizerSourceSynthesizer.h"
#include "deSynthesizerSourceVisitor.h"
#include "deSynthesizerSourceWave.h"


#include "../../../common/exceptions.h"



// Class deSynthesizerSourceVisitor
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerSourceVisitor::deSynthesizerSourceVisitor(){
}

deSynthesizerSourceVisitor::~deSynthesizerSourceVisitor(){
}



// Visiting
/////////////

void deSynthesizerSourceVisitor::VisitSource( deSynthesizerSource &source ){
}

void deSynthesizerSourceVisitor::VisitSound( deSynthesizerSourceSound &source ){
	VisitSource( source );
}

void deSynthesizerSourceVisitor::VisitWave( deSynthesizerSourceWave &source ){
	VisitSource( source );
}

void deSynthesizerSourceVisitor::VisitChain( deSynthesizerSourceChain &source ){
	VisitSource( source );
}

void deSynthesizerSourceVisitor::VisitSynthesizer( deSynthesizerSourceSynthesizer &source ){
	VisitSource( source );
}

void deSynthesizerSourceVisitor::VisitGroup( deSynthesizerSourceGroup &source ){
	VisitSource( source );
}
