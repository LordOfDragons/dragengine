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

#include "igdeContainerSplitted.h"
#include "../igdeWidget.h"
#include "../native/toolkit.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/logger/deLogger.h>


// Class igdeContainerSplitted
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainerSplitted::igdeContainerSplitted(igdeEnvironment &environment,
	eSidePlacement sidePlacement) :
igdeContainer(environment),
pSidePlacement(sidePlacement),
pSplitPosition(-1),
pSplitPositionRelative(0.5f),
pWidgetSide(NULL),
pWidgetCenter(NULL){
}

igdeContainerSplitted::igdeContainerSplitted(igdeEnvironment &environment,
	eSidePlacement sidePlacement, int splitPosition) :
igdeContainer(environment),
pSidePlacement(sidePlacement),
pSplitPosition(splitPosition),
pSplitPositionRelative(0.0f),
pWidgetSide(NULL),
pWidgetCenter(NULL){
}

igdeContainerSplitted::igdeContainerSplitted(igdeEnvironment &environment,
	eSidePlacement sidePlacement, float splitPosition) :
igdeContainer(environment),
pSidePlacement(sidePlacement),
pSplitPosition(-1),
pSplitPositionRelative(decMath::clamp(splitPosition, 0.0f, 1.0f)),
pWidgetSide(NULL),
pWidgetCenter(NULL){
}

igdeContainerSplitted::~igdeContainerSplitted(){
}



// Management
///////////////

void igdeContainerSplitted::SetSplitPosition(int splitPosition){
	if(splitPosition == pSplitPosition){
		return;
	}
	
	pSplitPosition = splitPosition;
	
	if(GetNativeWidget()){
		((igdeNativeContainerSplitted*)GetNativeWidget())->UpdateSplitValue();
	}
}

void igdeContainerSplitted::SetSplitPositionRelative(float splitPosition){
	if(fabsf(splitPosition - pSplitPositionRelative) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pSplitPositionRelative = splitPosition;
	pSplitPosition = -1;
	
	if(GetNativeWidget()){
		((igdeNativeContainerSplitted*)GetNativeWidget())->UpdateSplitValue();
	}
}



void igdeContainerSplitted::AddChild(igdeWidget *child, eArea area){
	if(!child || child->GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	if(child == pWidgetSide || child == pWidgetCenter){
		DETHROW(deeInvalidParam);
	}
	
	switch(area){
	case eaSide:
		if(pWidgetSide){
			DETHROW(deeInvalidParam);
		}
		pWidgetSide = child;
		
		try{
			igdeContainer::AddChild(child);
			
		}catch(const deException &){
			pWidgetSide = NULL;
			throw;
		}
		break;
		
	case eaCenter:
		if(pWidgetCenter){
			DETHROW(deeInvalidParam);
		}
		pWidgetCenter = child;
		
		try{
			igdeContainer::AddChild(child);
			
		}catch(const deException &){
			pWidgetCenter = NULL;
			throw;
		}
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
	
	if(GetNativeWidget()){
		((igdeNativeContainerSplitted*)GetNativeWidget())->UpdateSplitValue();
	}
}

void igdeContainerSplitted::AddChild(igdeWidget*){
	DETHROW(deeInvalidParam);
}

void igdeContainerSplitted::RemoveChild(igdeWidget *child){
	igdeContainer::RemoveChild(child);
	
	if(child == pWidgetSide){
		pWidgetSide = NULL;
		
	}else if(child == pWidgetCenter){
		pWidgetCenter = NULL;
	}
}

void igdeContainerSplitted::RemoveAllChildren(){
	igdeContainer::RemoveAllChildren();
	
	pWidgetSide = NULL;
	pWidgetCenter = NULL;
}

igdeWidget *igdeContainerSplitted::GetWidgetIn(eArea area) const{
	switch(area){
	case eaSide:
		return pWidgetSide;
		
	case eaCenter:
		return pWidgetCenter;
		
	default:
		DETHROW(deeInvalidParam);
	}
}



void igdeContainerSplitted::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	igdeNativeContainerSplitted * const native = igdeNativeContainerSplitted::CreateNativeWidget(*this);
	SetNativeWidget(native);
	native->PostCreateNativeWidget();
	
	CreateChildWidgetNativeWidgets();
	
	native->UpdateSplitValue();
}

void igdeContainerSplitted::DestroyNativeWidget(){
	if(!GetNativeWidget()){
		return;
	}
	
	igdeNativeContainerSplitted * const native = (igdeNativeContainerSplitted*)GetNativeWidget();
	DropNativeWidget();
	native->DestroyNativeWidget();
}

void igdeContainerSplitted::CreateChildWidgetNativeWidgets(){
	switch(pSidePlacement){
	case igdeContainerSplitted::espBottom:
	case igdeContainerSplitted::espRight:
		if(pWidgetCenter){
			pWidgetCenter->CreateNativeWidget();
		}
		if(pWidgetSide){
			pWidgetSide->CreateNativeWidget();
		}
		break;
		
	case igdeContainerSplitted::espTop:
	case igdeContainerSplitted::espLeft:
	default:
		if(pWidgetSide){
			pWidgetSide->CreateNativeWidget();
		}
		if(pWidgetCenter){
			pWidgetCenter->CreateNativeWidget();
		}
		break;
	}
}
