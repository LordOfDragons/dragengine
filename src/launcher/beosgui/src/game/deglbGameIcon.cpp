/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include <Bitmap.h>
#include <BitmapStream.h>
#include <TranslationUtils.h>
#include <DataIO.h>
#include <View.h>

#include "deglbGameIcon.h"

#include <dragengine/common/exceptions.h>


// Class deglbGameIcon
///////////////////////

// Constructors and Destructors
/////////////////////////////////

deglbGameIcon::deglbGameIcon(int size, const char *path) :
delGameIcon(size, path),
pBitmap(nullptr){
}

deglbGameIcon::~deglbGameIcon(){
	if(pBitmap){
		delete pBitmap;
	}
}



// Management
///////////////

BBitmap *deglbGameIcon::CreateScaledBitmap(int size) const{
	if(!pBitmap){
		return nullptr;
	}
	
	if(GetSize() == size){
		return new BBitmap(*pBitmap);
	}
	
	BRect newRect(0, 0, size, size);
	BBitmap * const scaled = new BBitmap(newRect, B_RGBA32, true); // true = acceptsViews
	if(scaled->Lock()){
		BView * const view = new BView(newRect, "scaler", B_FOLLOW_NONE, B_WILL_DRAW);
		scaled->AddChild(view);
		view->DrawBitmap(pBitmap, pBitmap->Bounds(), newRect);
		scaled->RemoveChild(view);
		delete view;
		scaled->Unlock();
	}
	
	return scaled;
}



// Protected Functions
////////////////////////

void deglbGameIcon::OnContentChanged(){
	pCreateBitmap();
}



// Private Functions
//////////////////////

void deglbGameIcon::pCreateBitmap(){
	if(pBitmap){
		delete pBitmap;
		pBitmap = nullptr;
	}
	
	if(!GetContent()){
		return;
	}
	
	if(GetPath().EndsWith(".png")){
		pCreatePNGBitmap();
	}
}

void deglbGameIcon::pCreatePNGBitmap(){
	try{
		BMemoryIO memIO(GetContent()->GetPointer(), GetContent()->GetLength());
		BBitmap * const bitmap = BTranslationUtils::GetBitmap(&memIO);
		if(bitmap){
			DEASSERT_TRUE(bitmap->IsValid())
			pBitmap = bitmap;
		}
		
	}catch(const deException &e){
		e.PrintError();
		if(pBitmap){
			delete pBitmap;
			pBitmap = nullptr;
		}
	}
}
