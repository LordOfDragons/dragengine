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

#include "igdeTabBook.h"
#include "native/toolkit.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeTabBook::cHeader
///////////////////////////////

igdeTabBook::cHeader::cHeader(const char *ptext, igdeIcon *picon, const char *pdescription) :
text(ptext),
description(pdescription),
icon(picon){
}

igdeTabBook::cHeader::~cHeader(){
}



// Class igdeTabBook
//////////////////////

// Constructor, destructor
////////////////////////////

igdeTabBook::igdeTabBook(igdeEnvironment &environment) :
igdeContainer(environment),
pActivePanel(-1),
pNativeTabBook(nullptr){
}

igdeTabBook::~igdeTabBook(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeTabBook::SetActivePanel(int index){
	if(index == pActivePanel){
		return;
	}
	
	if(index == -1){
		if(GetChildren().IsNotEmpty()){
			DETHROW(deeInvalidParam);
		}
		
	}else if(index < 0 || index >= GetChildren().GetCount()){
		DETHROW(deeInvalidParam);
	}
	
	pActivePanel = index;
	
	if(pNativeTabBook){
		pNativeTabBook->ChangePanel(index);
	}
}



void igdeTabBook::AddChild(igdeWidget *child, const char *text){
	AddChild(child, text, nullptr, "");
}

void igdeTabBook::AddChild(igdeWidget *child, const char *text, igdeIcon *icon){
	AddChild(child, text, icon, "");
}

void igdeTabBook::AddChild(igdeWidget *child, const char *text, igdeIcon *icon,
const char *description){
	if(!child || child->GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	igdeContainer::AddChild(child);
	
	const cHeader::Ref header(cHeader::Ref::New(text, icon, description));
	try{
		pHeaders.Add(header);
		
	}catch(const deException &){
		igdeContainer::RemoveChild(child);
		throw;
	}
	
	if(!pNativeTabBook){
		return;
	}
	
	try{
		pNativeTabBook->AddHeader((const cHeader &)*header);
		
	}catch(const deException &){
		pHeaders.RemoveFrom(pHeaders.GetCount() - 1);
		igdeContainer::RemoveChild(child);
		throw;
	}
}

void igdeTabBook::AddChild(igdeWidget *child){
	DETHROW(deeInvalidParam);
}

void igdeTabBook::RemoveChild(igdeWidget *child){
	const int index = GetChildren().IndexOf(child);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	if(pNativeTabBook){
		pNativeTabBook->RemoveHeader(index);
	}
	pHeaders.RemoveFrom(index);
	
	igdeContainer::RemoveChild(child);
}

void igdeTabBook::RemoveAllChildren(){
	if(pNativeTabBook){
		pNativeTabBook->RemoveAllHeaders();
	}
	pHeaders.RemoveAll();
	
	igdeContainer::RemoveAllChildren();
}


void igdeTabBook::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	igdeNativeTabBook * const native = igdeNativeTabBook::CreateNativeWidget(*this);
	SetNativeWidget(native);
	pNativeTabBook = native;
	native->PostCreateNativeWidget();
	
	CreateChildWidgetNativeWidgets();
	
	native->ChangePanel(pActivePanel);
}

void igdeTabBook::DestroyNativeWidget(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeTabBook*)GetNativeWidget())->DestroyNativeWidget();
	DropNativeWidget();
}

void igdeTabBook::DropNativeWidget(){
	pNativeTabBook = nullptr;
	igdeContainer::DropNativeWidget();
}

void *igdeTabBook::GetNativeContainer() const{
	return pNativeTabBook ? pNativeTabBook->GetNativeContainer() : nullptr;
}



void igdeTabBook::CreateChildWidgetNativeWidgets(){
	DEASSERT_NOTNULL(pNativeTabBook)
	
	igdeContainer::CreateChildWidgetNativeWidgets();
	
	pHeaders.Visit([&](const cHeader &header){
		pNativeTabBook->AddHeader(header);
	});
}

void igdeTabBook::OnNativeWidgetLanguageChanged(){
	if(pNativeTabBook){
		pHeaders.VisitIndexed([&](int index, const cHeader &header){
			pNativeTabBook->UpdateHeader(index, header);
		});
	}
}
