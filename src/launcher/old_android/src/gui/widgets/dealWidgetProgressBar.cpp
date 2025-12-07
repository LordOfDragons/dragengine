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

#include "dealWidgetProgressBar.h"
#include "dealWidgetLayoutFlow.h"
#include "dealWidgetLabel.h"
#include "../dealDisplay.h"
#include "../dealFont.h"
#include "../../dealLauncher.h"
#include "../../common/exceptions.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealWidgetProgressBar
////////////////////////////////

// Constructors, destructors
//////////////////////////////

dealWidgetProgressBar::dealWidgetProgressBar(dealDisplay &display) :
dealWidgetLayoutStack(display, true),

pBoxProgress(NULL),
pLabelProgress(NULL),

pMinProgress(0),
pMaxProgress(100),
pProgress(0)
{
	pBuildContent();
}

dealWidgetProgressBar::dealWidgetProgressBar(dealDisplay &display, int minProgress, int maxProgress) :
dealWidgetLayoutStack(display, true),

pBoxProgress(NULL),
pLabelProgress(NULL),

pMinProgress(minProgress),
pMaxProgress(decMath::max(maxProgress, minProgress)),
pProgress(minProgress)
{
	pBuildContent();
}

dealWidgetProgressBar::~dealWidgetProgressBar(){
}



// Management
///////////////

void dealWidgetProgressBar::SetRange(int minProgress, int maxProgress){
	maxProgress = decMath::max(maxProgress, minProgress);
	
	if(pMinProgress == minProgress && pMaxProgress == maxProgress){
		return;
	}
	
	pMinProgress = minProgress;
	pMaxProgress = maxProgress;
	pProgress = decMath::clamp(pProgress, minProgress, maxProgress);
	
	pUpdateContent();
}

void dealWidgetProgressBar::SetProgress(int progress){
	progress = decMath::clamp(progress, pMinProgress, pMaxProgress);
	
	if(pProgress == progress){
		return;
	}
	
	pProgress = progress;
	
	pUpdateContent();
}

float dealWidgetProgressBar::GetProgressPercentage() const{
	return decMath::linearStep((float)pProgress, (float)pMinProgress, (float)pMaxProgress);
}



void dealWidgetProgressBar::OnSizeChanged(){
	pUpdateContent();
}

void dealWidgetProgressBar::OnPaddingChanged(){
	pUpdateContent();
}



// Private functions
//////////////////////

void dealWidgetProgressBar::pBuildContent(){
	dealDisplay &display = GetDisplay();
	
	dealWidgetLayoutFlow * const layoutBox = new dealWidgetLayoutFlow(display,
		true, 0, dealWidgetLayoutFlow::eltTop, dealWidgetLayoutFlow::eltFill);
	layoutBox->SetBackgroundColor(decColor(0.2f, 0.2f, 0.2f));
	
	pBoxProgress = new dealWidget(display);
	pBoxProgress->SetBackgroundColor(decColor(0.0f, 0.0f, 1.0f));
	layoutBox->AddWidget(pBoxProgress);
	AddWidget(layoutBox);
	layoutBox->FreeReference();
	
	pLabelProgress = new dealWidgetLabel(display, "0 %");
	AddWidget(pLabelProgress);
}

void dealWidgetProgressBar::pUpdateContent(){
	const float percentage = GetProgressPercentage();
	
	decString text;
	text.Format("%.0f %%", percentage * 100.0f);
	pLabelProgress->SetText(text);
	
	const float colorBlend = decMath::linearStep(percentage, 0.4f, 0.6f);
	const decColor color1(1.0f, 1.0f, 1.0f);
	const decColor color2(1.0f, 1.0f, 0.0f);
	pLabelProgress->SetColor(color1 * (1.0f - colorBlend) + color2 * colorBlend);
	
	const int width = (int)(percentage * (float)GetContentArea().x);
	pBoxProgress->SetExplicitMinimumSize(decPoint(width, 0));
}
