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

#include "igdeMetaPropertyContextWidget.h"
#include "igdeMetaPropertyGroupWidget.h"
#include "../../../gui/igdeUIHelper.h"


// Class igdeMetaPropertyContextWidget::PropertyListener
//////////////////////////////////////////////////////////

igdeMetaPropertyContextWidget::PropertyListener::PropertyListener(
	igdeMetaPropertyContextWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyContextWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyContextWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyContext*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}


// Class igdeMetaPropertyContextWidget
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyContextWidget::igdeMetaPropertyContextWidget(igdeMetaPropertyContext &property) :
igdeMetaPropertyWidget(property),
pPropertyContext(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyContext.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyContextWidget::~igdeMetaPropertyContextWidget(){
	Drop();
	pPropertyContext.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyContextWidget::Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel){
	if(!pContextPanel){
		pContextPanel = igdeWPMetaContext::Ref::New(helper.GetEnvironment());
	}
	container.AddChild(pContextPanel);
}

void igdeMetaPropertyContextWidget::Drop(){
	// we keep the context panel alive to keep the caches inside alive
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyContextWidget::Update(){
	if(!pContextPanel){
		return;
	}
	
	const bool valid = pPropertyContext.IsValid(GetContext());
	pContextPanel->SetContext(valid ? pPropertyContext.GetPropertyValue(GetContext())
		: pPropertyContext.GetDefaultValue());
}

void igdeMetaPropertyContextWidget::Filter(const igdeFilter &filter){
	if(pContextPanel){
		pContextPanel->SetFilter(filter);
	}
	SetFilteredOut(filter && !pContextPanel->HasVisibleWidgets());
}


// Protected Functions
////////////////////////

void igdeMetaPropertyContextWidget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pContextPanel){
		pContextPanel->SetVisible(!GetFilteredOut());
	}
}

void igdeMetaPropertyContextWidget::OnContextChanged(){
	Update();
}
