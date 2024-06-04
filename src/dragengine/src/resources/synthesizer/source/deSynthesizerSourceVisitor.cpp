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
