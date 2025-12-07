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

#include "dealDialog.h"
#include "dealDisplay.h"
#include "widgets/dealWidgetLayoutFlow.h"
#include "../dealLauncher.h"
#include "../common/exceptions.h"
#include "../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealDialog
/////////////////////

// Constructors, destructors
//////////////////////////////

dealDialog::dealDialog(dealDisplay &display, const char *title) :
pDisplay(display),

pTitle(title),
pContent(NULL),
pPopupDialog(NULL)
{
	pContent = new dealWidgetLayoutFlow(display, false, 0,
		dealWidgetLayoutFlow::eltFill, dealWidgetLayoutFlow::eltFill);
	pContent->SetSize(decPoint(display.GetWidth(), display.GetHeight()));
	pContent->SetBackgroundColor(decColor(0.9f, 0.9f, 0.9f));
}

dealDialog::~dealDialog(){
	if(pPopupDialog){
		pPopupDialog->FreeReference();
	}
	if(pContent){
		pContent->FreeReference();
	}
}



// Management
///////////////

void dealDialog::SetTitle(const char *title){
	if(pTitle == title){
		return;
	}
	
	pTitle = title;
}

void dealDialog::SetPopupDialog(dealDialog *dialog){
	if(pPopupDialog == dialog){
		return;
	}
	
	if(pPopupDialog){
		pPopupDialog->OnDeactivate();
		pPopupDialog->FreeReference();
	}
	
	pPopupDialog = dialog;
	
	if(dialog){
		dialog->AddReference();
		
		const decPoint screenSize(pDisplay.GetWidth(), pDisplay.GetHeight());
		const decPoint popupSize(dialog->pContent->GetMinimumSize());
		
		dialog->pContent->SetSize(popupSize);
		dialog->pContent->SetPosition((screenSize - popupSize) / 2);
		
		dialog->OnActivate();
	}
}



void dealDialog::OnActivate(){
}

void dealDialog::OnDeactivate(){
}

void dealDialog::OnFrameUpdate(){
}

dealDialog *dealDialog::BackKeyDialog(){
	return NULL;
}
