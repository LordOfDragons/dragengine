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
#include "deSkinProperty.h"
#include "deSkinPropertyValue.h"
#include "deSkinPropertyColor.h"
#include "deSkinPropertyImage.h"
#include "deSkinPropertyVideo.h"
#include "deSkinPropertyConstructed.h"
#include "deSkinPropertyMapped.h"
#include "deSkinPropertyVisitorIdentify.h"
#include "../../../common/exceptions.h"



// Class deSkinPropertyVisitorIdentify
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyVisitorIdentify::deSkinPropertyVisitorIdentify(){
	Reset();
}

deSkinPropertyVisitorIdentify::~deSkinPropertyVisitorIdentify(){
}



// Management
///////////////

deSkinPropertyValue &deSkinPropertyVisitorIdentify::CastToValue() const{
	if(pType != eptValue){
		DETHROW(deeInvalidParam);
	}
	
	return *((deSkinPropertyValue*)pProperty);
}

deSkinPropertyColor &deSkinPropertyVisitorIdentify::CastToColor() const{
	if(pType != eptColor){
		DETHROW(deeInvalidParam);
	}
	
	return *((deSkinPropertyColor*)pProperty);
}

deSkinPropertyImage &deSkinPropertyVisitorIdentify::CastToImage() const{
	if(pType != eptImage){
		DETHROW(deeInvalidParam);
	}
	
	return *((deSkinPropertyImage*)pProperty);
}

deSkinPropertyVideo &deSkinPropertyVisitorIdentify::CastToVideo() const{
	if(pType != eptVideo){
		DETHROW(deeInvalidParam);
	}
	
	return *((deSkinPropertyVideo*)pProperty);
}

deSkinPropertyConstructed &deSkinPropertyVisitorIdentify::CastToConstructed() const{
	if(pType != eptConstructed){
		DETHROW(deeInvalidParam);
	}
	
	return *((deSkinPropertyConstructed*)pProperty);
}

deSkinPropertyMapped &deSkinPropertyVisitorIdentify::CastToMapped() const{
	if(pType != eptMapped){
		DETHROW(deeInvalidParam);
	}
	
	return *((deSkinPropertyMapped*)pProperty);
}

void deSkinPropertyVisitorIdentify::Reset(){
	pProperty = NULL;
	pType = eptUnknown;
}



// Visiting
/////////////

void deSkinPropertyVisitorIdentify::VisitProperty(deSkinProperty &property){
	pProperty = &property;
	pType = eptUnknown;
}

void deSkinPropertyVisitorIdentify::VisitValue(deSkinPropertyValue &property){
	pProperty = &property;
	pType = eptValue;
}

void deSkinPropertyVisitorIdentify::VisitColor(deSkinPropertyColor &property){
	pProperty = &property;
	pType = eptColor;
}

void deSkinPropertyVisitorIdentify::VisitImage(deSkinPropertyImage &property){
	pProperty = &property;
	pType = eptImage;
}

void deSkinPropertyVisitorIdentify::VisitVideo(deSkinPropertyVideo &property){
	pProperty = &property;
	pType = eptVideo;
}

void deSkinPropertyVisitorIdentify::VisitConstructed(deSkinPropertyConstructed &property){
	pProperty = &property;
	pType = eptConstructed;
}

void deSkinPropertyVisitorIdentify::VisitMapped(deSkinPropertyMapped &property){
	pProperty = &property;
	pType = eptMapped;
}
