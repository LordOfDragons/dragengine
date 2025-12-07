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

#include "igdeSwitcher.h"
#include "native/toolkit.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>


// Class igdeSwitcher
///////////////////////

// Constructor, destructor
////////////////////////////

igdeSwitcher::igdeSwitcher(igdeEnvironment &environment) :
igdeContainer(environment),
pCurrent(-1){
}

igdeSwitcher::~igdeSwitcher(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeSwitcher::SetCurrent(int index){
	if(index == pCurrent){
		return;
	}
	
	if(GetChildCount() > 0){
		if(index < 0 || index >= GetChildCount()){
			DETHROW(deeInvalidParam);
		}
		
	}else{
		if(index != -1){
			DETHROW(deeInvalidParam);
		}
	}
	
	pCurrent = index;
	
	OnCurrentChanged();
}



void igdeSwitcher::AddChild(igdeWidget *child){
	igdeContainer::AddChild(child);
	pCurrent = 0;
}

void igdeSwitcher::RemoveChild(igdeWidget *child){
	const int index = IndexOfChild(child);
	igdeContainer::RemoveChild(child);
	
	int current = pCurrent;
	
	if(index < current){
		current--;
		
	}else if(current >= GetChildCount()){
		current = GetChildCount() - 1;
	}
	
	SetCurrent(current);
}

void igdeSwitcher::RemoveAllChildren(){
	igdeContainer::RemoveAllChildren();
	pCurrent = -1;
}



void igdeSwitcher::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	igdeNativeSwitcher * const native = igdeNativeSwitcher::CreateNativeWidget(*this);
	SetNativeWidget(native);
	native->PostCreateNativeWidget();
	
	CreateChildWidgetNativeWidgets();
	
	native->UpdateCurrent();
}

void igdeSwitcher::DestroyNativeWidget(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeSwitcher*)GetNativeWidget())->DestroyNativeWidget();
	DropNativeWidget();
}



// Protected Functions
////////////////////////

void igdeSwitcher::OnCurrentChanged(){
	if(GetNativeWidget()){
		((igdeNativeSwitcher*)GetNativeWidget())->UpdateCurrent();
	}
}
