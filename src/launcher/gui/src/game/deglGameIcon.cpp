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

#include "deglGameIcon.h"

#include <dragengine/common/exceptions.h>


// Class deglGameIcon
///////////////////////

// Constructors and Destructors
/////////////////////////////////

deglGameIcon::deglGameIcon(int size, const char *path) :
delGameIcon(size, path){
}

deglGameIcon::~deglGameIcon(){
}



// Management
///////////////

deglSharedIcon::Ref deglGameIcon::GetScaledFoxIcon(int size) const{
	if(!pFoxIcon){
		return nullptr;
	}
	
	if(GetSize() == size){
		return pFoxIcon;
	}
	
	const int quality = 2;
		// quality 0: fast but ugly scale
		// quality 1: slower box filtered scale
		// quality 2: slow gamma corrected scale
	
	deglSharedIcon::Ref icon(deglSharedIcon::Ref::New(
		new deglSharedIcon(new FXPNGIcon(FXApp::instance()))));
	
	FXMemoryStream stream(FXStreamLoad, (FXuchar*)GetContent()->GetPointer(), GetContent()->GetLength());
	stream.position(0);
	
	FXIcon &iconScaled = *icon->GetIcon();
	iconScaled.loadPixels(stream);
	iconScaled.scale(size, size, quality);
	iconScaled.create();
	
	return icon;
}



// Protected Functions
////////////////////////

void deglGameIcon::OnContentChanged(){
	pCreateFoxIcon();
}



// Private Functions
//////////////////////

void deglGameIcon::pCreateFoxIcon(){
	pFoxIcon = nullptr;
	
	if(!GetContent()){
		return;
	}
	
	if(GetPath().EndsWith(".png")){
		pCreatePNGFoxIcon();
		
	}else{
		// TODO create a placeholder icon instead
	}
}

void deglGameIcon::pCreatePNGFoxIcon(){
	try{
		FXMemoryStream stream(FXStreamLoad, (FXuchar*)GetContent()->GetPointer(), GetContent()->GetLength());
		stream.position(0);
		pFoxIcon.TakeOverWith(new FXPNGIcon(FXApp::instance()));
		FXIcon &icon = *pFoxIcon->GetIcon();
		icon.loadPixels(stream);
		icon.create();
		
	}catch(const deException &){
		pFoxIcon = nullptr;
	}
}
