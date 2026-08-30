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

#ifdef IGDE_TOOLKIT_BEOS

#include "igdeNativeBeosImage.h"
#include "../../igdeImage.h"
#include "../../igdeContainer.h"
#include "../../theme/igdeGuiTheme.h"
#include <dragengine/common/exceptions.h>
#include <interface/View.h>
#include <graphics/Bitmap.h>
#include <graphics/GraphicsDefs.h>


// Class igdeNativeBeosImage
//////////////////////////////

class igdeNativeBeosImageView : public BView {
public:
	igdeNativeBeosImageView() :
	BView(BRect(0, 0, 100, 100), "imageview", B_FOLLOW_ALL, B_WILL_DRAW),
	pOwner(nullptr),
	pBitmap(nullptr)
	{
		SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	}
	
	virtual ~igdeNativeBeosImageView(){
		if(pBitmap){
			delete pBitmap;
		}
	}
	
	void Draw(BRect updateRect){
		BView::Draw(updateRect);
		
		try{
			// Draw image bitmap if available
			if(pBitmap){
				DrawBitmapAsync(pBitmap, BPoint(5, 5));
				
			}else{
				// Draw placeholder for empty image
				SetHighColor(150, 150, 150);
				FillRect(BRect(5, 5, 95, 95));
				SetHighColor(100, 100, 100);
				StrokeRect(BRect(5, 5, 95, 95));
			}
			
		}catch(const deException &e){
			if(pOwner){
				pOwner->GetLogger()->LogException("IGDE Image", e);
			}
		}
	}
	
	void SetImage(BBitmap *bitmap){
		if(pBitmap){
			delete pBitmap;
		}
		pBitmap = bitmap;
		Invalidate();
	}
	
private:
	igdeImage *pOwner;
	BBitmap *pBitmap;
};


igdeNativeBeosImage::igdeNativeBeosImage() = default;
igdeNativeBeosImage::~igdeNativeBeosImage() = default;

igdeNativeBeosImageView* igdeNativeBeosImage::CreateNativeWidget(igdeImage &owner){
	// Image widget displays a static image or icon
	// Implementation renders bitmap with optional scaling
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosImageView();
	widget->pOwner = &owner;
	parent->AddChild(widget);
	
	return widget;
}

void igdeNativeBeosImage::PostCreateNativeWidget(igdeImage &owner, void *native){
	// Image post-creation setup
	auto widget = (igdeNativeBeosImageView*)native;
	if(widget){
		widget->pOwner = &owner;
	}
}

void igdeNativeBeosImage::DestroyNativeWidget(igdeImage &owner, void *native){
	auto widget = (igdeNativeBeosImageView*)native;
	if(widget){
		widget->RemoveSelf();
		delete widget;
	}
}

#endif
