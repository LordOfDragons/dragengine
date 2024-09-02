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
#include <string.h>

#include "deSynthesizerEffect.h"
#include "deSynthesizerEffectVisitor.h"
#include "../deSynthesizer.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deSynthesizerEffect
//////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerEffect::deSynthesizerEffect() :
pSynthesizer( NULL ),
pStrength( 1.0f ),
pEnabled( true ){
}

deSynthesizerEffect::~deSynthesizerEffect(){
}



// Management
///////////////

void deSynthesizerEffect::SetSynthesizer( deSynthesizer *synthesizer ){
	pSynthesizer = synthesizer;
}

void deSynthesizerEffect::SetEnabled( bool enabled ){
	pEnabled = enabled;
}

void deSynthesizerEffect::SetStrength( float strength ){
	pStrength = strength;
}



// Visiting
/////////////

void deSynthesizerEffect::Visit( deSynthesizerEffectVisitor &visitor ){
	visitor.VisitEffect( *this );
}
