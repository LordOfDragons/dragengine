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

#include <stdlib.h>
#include <stdio.h>
#include "deSkinProperty.h"
#include "deSkinPropertyVisitor.h"
#include "deSkinPropertyValue.h"
#include "deSkinPropertyColor.h"
#include "deSkinPropertyImage.h"
#include "deSkinPropertyVideo.h"
#include "deSkinPropertyConstructed.h"
#include "deSkinPropertyMapped.h"
#include "../../../common/exceptions.h"



// Class deSkinPropertyVisitor
////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyVisitor::deSkinPropertyVisitor(){
}

deSkinPropertyVisitor::~deSkinPropertyVisitor(){
}



// Visiting
/////////////

void deSkinPropertyVisitor::VisitProperty(deSkinProperty &property){
}

void deSkinPropertyVisitor::VisitValue(deSkinPropertyValue &property){
	VisitProperty(property);
}

void deSkinPropertyVisitor::VisitColor(deSkinPropertyColor &property){
	VisitProperty(property);
}

void deSkinPropertyVisitor::VisitImage(deSkinPropertyImage &property){
	VisitProperty(property);
}

void deSkinPropertyVisitor::VisitVideo(deSkinPropertyVideo &property){
	VisitProperty(property);
}

void deSkinPropertyVisitor::VisitConstructed(deSkinPropertyConstructed &property){
	VisitProperty(property);
}

void deSkinPropertyVisitor::VisitMapped(deSkinPropertyMapped &property){
	VisitProperty(property);
}
