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

#include "igdeMetaPropertyTreeList.h"
#include "widget/igdeMetaPropertyTreeListWidget.h"
#include "../igdeMetaContext.h"

#include <dragengine/common/collection/decTList.h>


// Class igdeMetaPropertyTreeList::Listener
/////////////////////////////////////////////

void igdeMetaPropertyTreeList::Listener::OnActiveChanged(
igdeMetaPropertyTreeList*, const ContextRef&){
}

void igdeMetaPropertyTreeList::Listener::OnExpandedChanged(
igdeMetaPropertyTreeList*, const ContextRef&){
}

void igdeMetaPropertyTreeList::Listener::OnObjectItemInfoChanged(
igdeMetaPropertyTreeList*, const ContextRef&){
}


// Class igdeMetaPropertyTreeList::Walker
///////////////////////////////////////////

igdeMetaPropertyTreeList::Walker::Walker() = default;
igdeMetaPropertyTreeList::Walker::~Walker() = default;


// Class igdeMetaPropertyTreeList
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyTreeList::igdeMetaPropertyTreeList(const char *id, const char *name, const char *description) :
igdeMetaProperty(id, name, description){
}

igdeMetaPropertyTreeList::igdeMetaPropertyTreeList(const char *id, const char *translationTag) :
igdeMetaProperty(id, translationTag){
}

igdeMetaPropertyTreeList::~igdeMetaPropertyTreeList() = default;


// Management
///////////////

void igdeMetaPropertyTreeList::SetRows(int rows){
	pRows = decMath::max(rows, 1);
}

void igdeMetaPropertyTreeList::SetSorted(bool sorted){
	pSorted = sorted;
}

void igdeMetaPropertyTreeList::NotifyValueChanged(const ContextRef &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

void igdeMetaPropertyTreeList::NotifyActiveChanged(const ContextRef &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnActiveChanged(this, context);
	});
}

void igdeMetaPropertyTreeList::NotifyExpandedChanged(const ContextRef &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnExpandedChanged(this, context);
	});
}

void igdeMetaPropertyTreeList::NotifyObjectItemInfoChanged(const ContextRef &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnObjectItemInfoChanged(this, context);
	});
}

igdeMetaPropertyTreeList::ExpandedSet igdeMetaPropertyTreeList::GetExpanded(const ContextRef &context) const{
	auto active = GetActiveObject(context);
	return active ? ExpandedSet(devctag, active) : ExpandedSet();
}

void igdeMetaPropertyTreeList::SetExpanded(const ContextRef &context, const ExpandedSet &expanded){
}

igdeMetaProperty::Action::Ref igdeMetaPropertyTreeList::CreateButtonAction(TargetButton, igdeWidget&){
	return {};
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyTreeList::CreateWidget(){
	return igdeMetaPropertyTreeListWidget::Ref::New(*this);
}
