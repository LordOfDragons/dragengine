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

#include "cePCBOption.h"
#include "cePlayerChoiceBox.h"
#include "../action/ceCAPlayerChoice.h"
#include "../action/ceCAPlayerChoiceOption.h"
#include "../ceConversation.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasText.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>



// Class cePCBOption
//////////////////////

// Constructor, destructor
////////////////////////////

cePCBOption::cePCBOption() :
pAction(NULL),
pActionOption(NULL),
pCanvasView(NULL){
}

cePCBOption::~cePCBOption(){
	if(pCanvasView){
		pCanvasView->FreeReference();
	}
	SetActionOption(NULL, NULL);
}



// Management
///////////////

void cePCBOption::SetText(const decUnicodeString &text){
	pText = text;
}

void cePCBOption::SetActionOption(ceCAPlayerChoice *action, ceCAPlayerChoiceOption *option){
	if(action != pAction){
		if(pAction){
			pAction->FreeReference();
		}
		
		pAction = action;
		
		if(action){
			action->AddReference();
		}
	}
	
	if(option != pActionOption){
		if(pActionOption){
			pActionOption->FreeReference();
		}
		
		pActionOption = option;
		
		if(option){
			option->AddReference();
		}
	}
}



void cePCBOption::Layout(const cePlayerChoiceBox &pcbox, bool selected){
	// create canvas if not existing
	deCanvasManager &canvasManager = *pcbox.GetConversation().GetEngine()->GetCanvasManager();
	if(! pCanvasView){
		pCanvasView = canvasManager.CreateCanvasView();
	}
	
	// clear canvas
	pCanvasView->RemoveAllCanvas();
	pCanvasView->SetSize(decPoint(10, 1));
	
	// get font. if not existing exit since we can not render anything this way
	deFont * const font = pcbox.GetFont();
	if(! font){
		return;
	}
	
	// determine required height
	deCanvasView &parentVide = *pcbox.GetCanvasView();
	const decPoint &parentSize = parentVide.GetSize();
	deCanvasText *canvasText = NULL;
	deCanvasPaint *canvasBackground = NULL;
	
	const decString text(pText.ToUTF8());
	
	const int textHeight = pcbox.GetTextSize();
	
	pCanvasView->SetSize(decPoint(parentSize.x, textHeight));
	
	try{
		canvasBackground = canvasManager.CreateCanvasPaint();
		canvasBackground->SetShapeType(deCanvasPaint::estRectangle);
		if(selected){
			canvasBackground->SetFillColor(pcbox.GetSelectedBackgroundColor());
		}else{
			canvasBackground->SetFillColor(pcbox.GetBackgroundColor());
		}
		canvasBackground->SetLineColor(decColor(canvasBackground->GetFillColor(), 0.0f));
		canvasBackground->SetThickness(0.0f);
		canvasBackground->SetOrder(0.0f);
		canvasBackground->SetSize(pCanvasView->GetSize());
		pCanvasView->AddCanvas(canvasBackground);
		canvasBackground->FreeReference();
		canvasBackground = NULL;
		
		canvasText = canvasManager.CreateCanvasText();
		if(selected){
			canvasText->SetColor(pcbox.GetSelectedTextColor());
		}else{
			canvasText->SetColor(pcbox.GetTextColor());
		}
		canvasText->SetFont(font);
		canvasText->SetFontSize((float)textHeight);
		canvasText->SetText(text);
		canvasText->SetOrder(0.0f);
		canvasText->SetSize(pCanvasView->GetSize());
		pCanvasView->AddCanvas(canvasText);
		canvasText->FreeReference();
		canvasText = NULL;
		
	}catch(const deException &){
		if(canvasText){
			canvasText->FreeReference();
		}
		if(canvasBackground){
			canvasBackground->FreeReference();
		}
		throw;
	}
}
