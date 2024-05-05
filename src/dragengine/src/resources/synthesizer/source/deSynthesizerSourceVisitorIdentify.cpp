/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>

#include "deSynthesizerSource.h"
#include "deSynthesizerSourceChain.h"
#include "deSynthesizerSourceGroup.h"
#include "deSynthesizerSourceSound.h"
#include "deSynthesizerSourceSynthesizer.h"
#include "deSynthesizerSourceVisitorIdentify.h"
#include "deSynthesizerSourceWave.h"

#include "../../../common/exceptions.h"



// Class deSynthesizerSourceVisitorIdentify
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerSourceVisitorIdentify::deSynthesizerSourceVisitorIdentify() :
pSource( NULL ),
pType( estUnknown ){
}

deSynthesizerSourceVisitorIdentify::~deSynthesizerSourceVisitorIdentify(){
}



// Management
///////////////

deSynthesizerSourceSound &deSynthesizerSourceVisitorIdentify::CastToSound() const{
	if( pType != estSound ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deSynthesizerSourceSound* )pSource );
}

deSynthesizerSourceWave &deSynthesizerSourceVisitorIdentify::CastToWave() const{
	if( pType != estWave ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deSynthesizerSourceWave* )pSource );
}

deSynthesizerSourceChain &deSynthesizerSourceVisitorIdentify::CastToChain() const{
	if( pType != estChain ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deSynthesizerSourceChain* )pSource );
}

deSynthesizerSourceSynthesizer &deSynthesizerSourceVisitorIdentify::CastToSynthesizer() const{
	if( pType != estSynthesizer ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deSynthesizerSourceSynthesizer* )pSource );
}

deSynthesizerSourceGroup &deSynthesizerSourceVisitorIdentify::CastToGroup() const{
	if( pType != estGroup ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deSynthesizerSourceGroup* )pSource );
}

void deSynthesizerSourceVisitorIdentify::Reset(){
	pSource = NULL;
	pType = estUnknown;
}



// Visiting
/////////////

void deSynthesizerSourceVisitorIdentify::VisitSource( deSynthesizerSource &source ){
	pSource = &source;
	pType = estUnknown;
}

void deSynthesizerSourceVisitorIdentify::VisitSound( deSynthesizerSourceSound &source ){
	pSource = &source;
	pType = estSound;
}

void deSynthesizerSourceVisitorIdentify::VisitWave( deSynthesizerSourceWave &source ){
	pSource = &source;
	pType = estWave;
}

void deSynthesizerSourceVisitorIdentify::VisitChain( deSynthesizerSourceChain &source ){
	pSource = &source;
	pType = estChain;
}

void deSynthesizerSourceVisitorIdentify::VisitSynthesizer( deSynthesizerSourceSynthesizer &source ){
	pSource = &source;
	pType = estSynthesizer;
}

void deSynthesizerSourceVisitorIdentify::VisitGroup( deSynthesizerSourceGroup &source ){
	pSource = &source;
	pType = estGroup;
}
