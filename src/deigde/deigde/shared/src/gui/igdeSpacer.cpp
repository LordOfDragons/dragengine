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

#include "igdeSpacer.h"
#include "native/toolkit.h"
#include "igdeContainer.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeSpacer
/////////////////////

// Constructor, destructor
////////////////////////////

igdeSpacer::igdeSpacer(igdeEnvironment &environment, const decPoint &size) :
igdeWidget(environment),
pSize(size)
{
	if(!(size >= decPoint())){
		DETHROW(deeInvalidParam);
	}
}

igdeSpacer::~igdeSpacer(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeSpacer::SetSize(const decPoint &size){
	if(size == pSize){
		return;
	}
	
	if(!(size >= decPoint())){
		DETHROW(deeInvalidParam);
	}
	
	pSize = size;
	OnSizeChanged();
}



void igdeSpacer::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	igdeNativeSpacer * const native = igdeNativeSpacer::CreateNativeWidget(*this);
	SetNativeWidget(native);
	native->PostCreateNativeWidget();
}

void igdeSpacer::DestroyNativeWidget(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeSpacer*)GetNativeWidget())->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeSpacer::OnSizeChanged(){
	if(GetNativeWidget()){
		((igdeNativeSpacer*)GetNativeWidget())->SetSize(pSize.x, pSize.y);
	}
}
