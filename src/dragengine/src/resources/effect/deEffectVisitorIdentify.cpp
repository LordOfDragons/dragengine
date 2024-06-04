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

// includes
#include <stdio.h>
#include <stdlib.h>
#include "deEffect.h"
#include "deEffectFilterKernel.h"
#include "deEffectOverlayImage.h"
#include "deEffectColorMatrix.h"
#include "deEffectDistortImage.h"
#include "deEffectVisitorIdentify.h"
#include "../../common/exceptions.h"



// Class deEffectVisitorIdentify
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deEffectVisitorIdentify::deEffectVisitorIdentify(){
	Reset();
}

deEffectVisitorIdentify::~deEffectVisitorIdentify(){
}



// Management
///////////////

deEffectFilterKernel &deEffectVisitorIdentify::CastToFilterKernel() const{
	if( pType != eetFilterKernel ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deEffectFilterKernel* )pEffect );
}

deEffectOverlayImage &deEffectVisitorIdentify::CastToOverlayImage() const{
	if( pType != eetOverlayImage ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deEffectOverlayImage* )pEffect );
}

deEffectColorMatrix &deEffectVisitorIdentify::CastToColorMatrix() const{
	if( pType != eetColorMatrix ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deEffectColorMatrix* )pEffect );
}

deEffectDistortImage &deEffectVisitorIdentify::CastToDistortImage() const{
	if( pType != eetDistortImage ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deEffectDistortImage* )pEffect );
}

void deEffectVisitorIdentify::Reset(){
	pEffect = NULL;
	pType = eetUnknown;
}



// Visiting
/////////////

void deEffectVisitorIdentify::VisitEffect( deEffect &effect ){
	pEffect = &effect;
	pType = eetUnknown;
}

void deEffectVisitorIdentify::VisitFilterKernel( deEffectFilterKernel &effect ){
	pEffect = &effect;
	pType = eetFilterKernel;
}

void deEffectVisitorIdentify::VisitOverlayImage( deEffectOverlayImage &effect ){
	pEffect = &effect;
	pType = eetOverlayImage;
}

void deEffectVisitorIdentify::VisitColorMatrix( deEffectColorMatrix &effect ){
	pEffect = &effect;
	pType = eetColorMatrix;
}

void deEffectVisitorIdentify::VisitDistortImage( deEffectDistortImage &effect ){
	pEffect = &effect;
	pType = eetDistortImage;
}
