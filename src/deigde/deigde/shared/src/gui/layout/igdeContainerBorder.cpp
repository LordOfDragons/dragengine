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

#include "igdeContainerBorder.h"
#include "../igdeWidget.h"
#include "../native/toolkit.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeContainerBorder
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainerBorder::igdeContainerBorder(igdeEnvironment &environment, int spacing) :
igdeContainer(environment),
pSpacing (spacing),
pWidgetTop(nullptr),
pWidgetBottom(nullptr),
pWidgetLeft(nullptr),
pWidgetRight(nullptr),
pWidgetCenter(nullptr)
{
	if(spacing < 0){
		DETHROW(deeInvalidParam);
	}
}

igdeContainerBorder::~igdeContainerBorder(){
}



// Management
///////////////

void igdeContainerBorder::AddChild(igdeWidget *child, eArea area){
	if(!child || child->GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	if(child == pWidgetTop || child == pWidgetBottom || child == pWidgetLeft
	|| child == pWidgetRight || child == pWidgetCenter){
		DETHROW(deeInvalidParam);
	}
	
	switch(area){
	case eaTop:
		if(pWidgetTop){
			DETHROW(deeInvalidParam);
		}
		pWidgetTop = child;
		
		try{
			igdeContainer::AddChild(child);
			
		}catch(const deException &){
			pWidgetTop = nullptr;
			throw;
		}
		break;
		
	case eaBottom:
		if(pWidgetBottom){
			DETHROW(deeInvalidParam);
		}
		pWidgetBottom = child;
		
		try{
			igdeContainer::AddChild(child);
			
		}catch(const deException &){
			pWidgetBottom = nullptr;
			throw;
		}
		break;
		
	case eaLeft:
		if(pWidgetLeft){
			DETHROW(deeInvalidParam);
		}
		pWidgetLeft = child;
		
		try{
			igdeContainer::AddChild(child);
			
		}catch(const deException &){
			pWidgetLeft = nullptr;
			throw;
		}
		break;
		
	case eaRight:
		if(pWidgetRight){
			DETHROW(deeInvalidParam);
		}
		pWidgetRight = child;
		
		try{
			igdeContainer::AddChild(child);
			
		}catch(const deException &){
			pWidgetRight = nullptr;
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
			pWidgetCenter = nullptr;
			throw;
		}
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
	
	if(GetNativeWidget()){
		// if the native widget is already created the order of the children can not be guaranteed
		// anymore unless the widget is fully recreated. If the widgets are though already in the
		// right order we can avoid a full recreation
		const auto &children = GetChildren();
		int index = 0;
		if((pWidgetTop && children[index++] != pWidgetTop)
		|| (pWidgetBottom && children[index++] != pWidgetBottom)
		|| (pWidgetLeft && children[index++] != pWidgetLeft)
		|| (pWidgetRight && children[index++] != pWidgetRight)
		|| (pWidgetCenter && children[index++] != pWidgetCenter)){
			DestroyNativeWidget();
			CreateNativeWidget();
		}
	}
}

void igdeContainerBorder::AddChild(igdeWidget *child){
	DETHROW(deeInvalidParam);
}

void igdeContainerBorder::RemoveChild(igdeWidget *child){
	igdeContainer::RemoveChild(child);
	
	if(child == pWidgetTop){
		pWidgetTop = nullptr;
		
	}else if(child == pWidgetBottom){
		pWidgetBottom = nullptr;
		
	}else if(child == pWidgetLeft){
		pWidgetLeft = nullptr;
		
	}else if(child == pWidgetRight){
		pWidgetRight = nullptr;
		
	}else if(child == pWidgetCenter){
		pWidgetCenter = nullptr;
	}
}

void igdeContainerBorder::RemoveAllChildren(){
	igdeContainer::RemoveAllChildren();
	
	pWidgetTop = nullptr;
	pWidgetBottom = nullptr;
	pWidgetLeft = nullptr;
	pWidgetRight = nullptr;
	pWidgetCenter = nullptr;
}

igdeWidget *igdeContainerBorder::GetWidgetIn(eArea area) const{
	switch(area){
	case eaTop:
		return pWidgetTop;
		
	case eaBottom:
		return pWidgetBottom;
		
	case eaLeft:
		return pWidgetLeft;
		
	case eaRight:
		return pWidgetRight;
		
	case eaCenter:
		return pWidgetCenter;
		
	default:
		DETHROW(deeInvalidParam);
	}
}



void igdeContainerBorder::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	igdeNativeContainerBorder * const native = igdeNativeContainerBorder::CreateNativeWidget(*this);
	SetNativeWidget(native);
	native->PostCreateNativeWidget();
	
	CreateChildWidgetNativeWidgets();
}

void igdeContainerBorder::DestroyNativeWidget(){
	if(!GetNativeWidget()){
		return;
	}
	
	igdeNativeContainerBorder * const native = (igdeNativeContainerBorder*)GetNativeWidget();
	DropNativeWidget();
	native->DestroyNativeWidget();
}


// Protected Functions
////////////////////////

void igdeContainerBorder::CreateChildWidgetNativeWidgets(){
	// we have to add the widgets in the correct order for the layout to work properly
	// for certain native backends
	if(pWidgetTop){
		pWidgetTop->CreateNativeWidget();
	}
	if(pWidgetBottom){
		pWidgetBottom->CreateNativeWidget();
	}
	if(pWidgetLeft){
		pWidgetLeft->CreateNativeWidget();
	}
	if(pWidgetRight){
		pWidgetRight->CreateNativeWidget();
	}
	if(pWidgetCenter){
		pWidgetCenter->CreateNativeWidget();
	}
}
