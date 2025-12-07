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

desynCreateSynthesizerSource::desynCreateSynthesizerSource(desynSynthesizer &synthesizer, int firstLink) :
pSynthesizer(synthesizer),
pFirstLink(firstLink),
pSource(NULL){
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

void desynCreateSynthesizerSource::VisitSource(deSynthesizerSource &source){
	pSource = NULL;
}

void desynCreateSynthesizerSource::VisitSound(deSynthesizerSourceSound &source){
	pSource = new desynSynthesizerSourceSound(pSynthesizer, pFirstLink, source);
}

void desynCreateSynthesizerSource::VisitWave(deSynthesizerSourceWave &source){
	pSource = new desynSynthesizerSourceWave(pSynthesizer, pFirstLink, source);
}

void desynCreateSynthesizerSource::VisitChain(deSynthesizerSourceChain &source){
	pSource = new desynSynthesizerSourceChain(pSynthesizer, pFirstLink, source);
}

void desynCreateSynthesizerSource::VisitSynthesizer(deSynthesizerSourceSynthesizer &source){
	pSource = new desynSynthesizerSourceSynthesizer(pSynthesizer, pFirstLink, source);
}

void desynCreateSynthesizerSource::VisitGroup(deSynthesizerSourceGroup &source){
	pSource = new desynSynthesizerSourceGroup(pSynthesizer, pFirstLink, source);
}
