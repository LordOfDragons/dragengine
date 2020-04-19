/* 
 * Drag[en]gine Synthesizer Module
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

#include "desynCreateSynthesizerSource.h"
#include "source/desynSynthesizerSourceChain.h"
#include "source/desynSynthesizerSourceGroup.h"
#include "source/desynSynthesizerSourceSound.h"
#include "source/desynSynthesizerSourceSynthesizer.h"
#include "source/desynSynthesizerSourceWave.h"
#include "../deDESynthesizer.h"

#include <dragengine/common/exceptions.h>



// Class desynCreateSynthesizerSource
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

desynCreateSynthesizerSource::desynCreateSynthesizerSource( desynSynthesizer &synthesizer, int firstLink ) :
pSynthesizer( synthesizer ),
pFirstLink( firstLink ),
pSource( NULL ){
}

desynCreateSynthesizerSource::~desynCreateSynthesizerSource(){
}



// Management
///////////////

void desynCreateSynthesizerSource::Reset(){
	pSource = NULL;
}




// Visiting
/////////////

void desynCreateSynthesizerSource::VisitSource( deSynthesizerSource &source ){
	pSource = NULL;
}

void desynCreateSynthesizerSource::VisitSound( deSynthesizerSourceSound &source ){
	pSource = new desynSynthesizerSourceSound( pSynthesizer, pFirstLink, source );
}

void desynCreateSynthesizerSource::VisitWave( deSynthesizerSourceWave &source ){
	pSource = new desynSynthesizerSourceWave( pSynthesizer, pFirstLink, source );
}

void desynCreateSynthesizerSource::VisitChain( deSynthesizerSourceChain &source ){
	pSource = new desynSynthesizerSourceChain( pSynthesizer, pFirstLink, source );
}

void desynCreateSynthesizerSource::VisitSynthesizer( deSynthesizerSourceSynthesizer &source ){
	pSource = new desynSynthesizerSourceSynthesizer( pSynthesizer, pFirstLink, source );
}

void desynCreateSynthesizerSource::VisitGroup( deSynthesizerSourceGroup &source ){
	pSource = new desynSynthesizerSourceGroup( pSynthesizer, pFirstLink, source );
}
