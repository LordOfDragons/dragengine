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

#include "dealWidgetImage.h"
#include "../dealDisplay.h"
#include "../dealImage.h"
#include "../dealShader.h"
#include "../../dealLauncher.h"
#include "../../common/exceptions.h"
#include "../../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealWidgetImage
//////////////////////////

// Constructors, destructors
//////////////////////////////

dealWidgetImage::dealWidgetImage(dealDisplay &display, dealImage *image) :
dealWidget(display),
pImage(NULL),
pColorize(1.0f, 1.0f, 1.0f),
pScaling(1.0f){
	SetImage(image);
}

dealWidgetImage::~dealWidgetImage(){
}



// Management
///////////////

void dealWidgetImage::SetImage(dealImage *image){
	if(image == pImage){
		return;
	}
	
	
	pImage = image;
	
	if(image){
		image->AddReference();
	}
	
	DirtyParentLayout();
}

void dealWidgetImage::SetColorize(const decColor &colorize){
	pColorize = colorize;
}

void dealWidgetImage::SetScaling(float scaling){
	scaling = decMath::max(scaling, 1.0f);
	if(fabsf(scaling - pScaling) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pScaling = scaling;
	
	DirtyParentLayout();
}



decPoint dealWidgetImage::GetMinimumSize(){
	if(GetHasExplicitMinimumSize()){
		return GetExplicitMinimumSize();
	}
	
	if(!pImage){
		return decPoint();
	}
	
	return decPoint((int)((float)pImage->GetWidth() * pScaling),
		(int)((float)pImage->GetHeight() * pScaling));
}



void dealWidgetImage::RenderContent(const sRenderContext &context){
	if(!pImage){
		return;
	}
	
	dealShader &shader = *GetDisplay().GetTexturedShapeShader();
	const decPoint &size = GetSize();
	
	shader.Activate();
	
	ShaderSetTransform(context, shader, 0, 0, size.x - 1, size.y - 1);
	ShaderSetTCTransform(shader);
	
	if(GetEnabled()){
		ShaderSetColorMatrix(shader, pColorize);
		
	}else{
		ShaderSetColorMatrixDisabled(shader, pColorize);
	}
	
	ShaderSetGamma(shader, 1.0f);
	ShaderSetClipRect(context, shader);
	ShaderSetTCClamp(shader);
	
	BindTexture(0, *pImage, true, false);
	
	DrawRectangle();
	
	OGL_CHECK(GetDisplay().GetLauncher(), glBindTexture(GL_TEXTURE_2D, 0));
}
