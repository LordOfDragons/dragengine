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

#include "dealWidgetBorderImage.h"
#include "../dealDisplay.h"
#include "../dealImage.h"
#include "../dealShader.h"
#include "../../dealLauncher.h"
#include "../../common/exceptions.h"
#include "../../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealWidgetBorderImage
////////////////////////////////

// Constructors, destructors
//////////////////////////////

dealWidgetBorderImage::dealWidgetBorderImage(dealDisplay &display, dealImage *image, int borderLeft,
int borderTop, int borderRight, int borderBottom) :
dealWidgetImage(display, image),
pBorderLeft(decMath::max(borderLeft, 0)),
pBorderTop(decMath::max(borderTop, 0)),
pBorderRight(decMath::max(borderRight, 0)),
pBorderBottom(decMath::max(borderBottom, 0)){
}

dealWidgetBorderImage::~dealWidgetBorderImage(){
}



// Management
///////////////

void dealWidgetBorderImage::SetBorderLeft(int size){
	size = decMath::max(size, 0);
	if(size == pBorderLeft){
		return;
	}
	
	pBorderLeft = size;
	
	DirtyParentLayout();
}

void dealWidgetBorderImage::SetBorderTop(int size){
	size = decMath::max(size, 0);
	if(size == pBorderTop){
		return;
	}
	
	pBorderTop = size;
	
	DirtyParentLayout();
}

void dealWidgetBorderImage::SetBorderRight(int size){
	size = decMath::max(size, 0);
	if(size == pBorderRight){
		return;
	}
	
	pBorderRight = size;
	
	DirtyParentLayout();
}

void dealWidgetBorderImage::SetBorderBottom(int size){
	size = decMath::max(size, 0);
	if(size == pBorderBottom){
		return;
	}
	
	pBorderBottom = size;
	
	DirtyParentLayout();
}

void dealWidgetBorderImage::SetBorders(int left, int top, int right, int bottom){
	left = decMath::max(left, 0);
	top = decMath::max(top, 0);
	right = decMath::max(right, 0);
	bottom = decMath::max(bottom, 0);
	
	if(left == pBorderLeft && top == pBorderTop && right == pBorderRight && bottom == pBorderBottom){
		return;
	}
	
	pBorderLeft = left;
	pBorderTop = top;
	pBorderRight = right;
	pBorderBottom = bottom;
	
	DirtyParentLayout();
}



void dealWidgetBorderImage::RenderContent(const sRenderContext &context){
	dealImage * const image = GetImage();
	if(! image){
		return;
	}
	
	const int imageWidth = image->GetWidth();
	const int imageHeight = image->GetHeight();
	if(imageWidth == 0 || imageHeight == 0){
		return;
	}
	
	const decPoint &size = GetSize();
	if(size.x == 0 || size.y == 0){
		return;
	}
	
	const float scaling = GetScaling();
	const int borderLeft = (int)((float)pBorderLeft * scaling);
	const int borderTop = (int)((float)pBorderTop * scaling);
	const int borderRight = (int)((float)size.x - (float)pBorderRight * scaling);
	const int borderBottom = (int)((float)size.y - (float)pBorderBottom * scaling);
	
	const float factorU = 1.0f / (float)(imageWidth - 1);
	const float factorV = 1.0f / (float)(imageHeight - 1);
	const float tcLeft = factorU * (float)pBorderLeft;
	const float tcTop = factorV * (float)pBorderTop;
	const float tcRight = factorU * (float)(imageWidth - 1 - pBorderRight);
	const float tcBottom = factorV * (float)(imageHeight - 1 - pBorderBottom);
	
	dealShader &shader = *GetDisplay().GetTexturedShapeShader();
	shader.Activate();
	
	if(GetEnabled()){
		ShaderSetColorMatrix(shader, GetColorize());
		
	}else{
		ShaderSetColorMatrixDisabled(shader, GetColorize());
	}
	
	ShaderSetGamma(shader, 1.0f);
	ShaderSetClipRect(context, shader);
	ShaderSetTCClamp(shader);
	
	BindTexture(0, *image, true, false);
	
	// center
	ShaderSetTransform(context, shader, borderLeft, borderTop, borderRight, borderBottom);
	ShaderSetTCTransform(shader, tcLeft, tcTop, tcRight, tcBottom);
	DrawRectangle();
	
	// left side
	if(borderLeft > 0){
		ShaderSetTransform(context, shader, 0, borderTop, borderLeft, borderBottom);
		ShaderSetTCTransform(shader, 0.0f, tcTop, tcLeft, tcBottom);
		DrawRectangle();
	}
	
	// top side
	if(borderTop > 0){
		ShaderSetTransform(context, shader, borderLeft, 0, borderRight, borderTop);
		ShaderSetTCTransform(shader, tcLeft, 0.0f, tcRight, tcTop);
		DrawRectangle();
	}
	
	// right side
	if(borderRight > 0){
		ShaderSetTransform(context, shader, borderRight - 1, borderTop, size.x, borderBottom);
		ShaderSetTCTransform(shader, tcRight, tcTop, 1.0f, tcBottom);
		DrawRectangle();
	}
	
	// bottom side
	if(borderBottom > 0){
		ShaderSetTransform(context, shader, borderLeft, borderBottom - 1, borderRight, size.y);
		ShaderSetTCTransform(shader, tcLeft, tcBottom, tcRight, 1.0f);
		DrawRectangle();
	}
	
	// top left corner
	if(borderLeft > 0 && borderTop > 0){
		ShaderSetTransform(context, shader, 0, 0, borderLeft, borderTop);
		ShaderSetTCTransform(shader, 0.0f, 0.0f, tcLeft, tcTop);
		DrawRectangle();
	}
	
	// top right corner
	if(borderRight > 0 && borderTop > 0){
		ShaderSetTransform(context, shader, borderRight - 1, 0, size.x, borderTop);
		ShaderSetTCTransform(shader, tcRight, 0.0f, 1.0f, tcTop);
		DrawRectangle();
	}
	
	// bottom left corner
	if(borderLeft > 0 && borderBottom > 0){
		ShaderSetTransform(context, shader, 0, borderBottom - 1, borderLeft, size.y);
		ShaderSetTCTransform(shader, 0.0f, tcBottom, tcLeft, 1.0f);
		DrawRectangle();
	}
	
	// bottom right corner
	if(borderRight > 0 && borderBottom > 0){
		ShaderSetTransform(context, shader, borderRight - 1, borderBottom - 1, size.x, size.y);
		ShaderSetTCTransform(shader, tcRight, tcBottom, 1.0f, 1.0f);
		DrawRectangle();
	}
	
	OGL_CHECK(GetDisplay().GetLauncher(), glBindTexture(GL_TEXTURE_2D, 0));
}
