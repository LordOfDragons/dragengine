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

#include "igdeMetaPropertyList.h"
#include "widget/igdeMetaPropertyListWidget.h"


// Class igdeMetaPropertyList::Listener
///////////////////////////////////////////////

void igdeMetaPropertyList::Listener::OnActiveChanged(igdeMetaPropertyList*, const igdeMetaContext::Ref&){
}

void igdeMetaPropertyList::Listener::OnSelectionChanged(igdeMetaPropertyList*, const igdeMetaContext::Ref&){
}


// Class igdeMetaPropertyList
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyList::igdeMetaPropertyList(
	const char *id, const char *name, const char *description) :
igdeMetaProperty(id, name, description),
pRows(4){
}

igdeMetaPropertyList::~igdeMetaPropertyList() = default;


// Management
///////////////

void igdeMetaPropertyList::SetRows(int rows){
	pRows = decMath::max(rows, 1);
}

void igdeMetaPropertyList::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

void igdeMetaPropertyList::NotifyActiveChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnActiveChanged(this, context);
		listener.OnPropertyContextChanged(this, context);
	});
}

void igdeMetaPropertyList::NotifySelectionChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnSelectionChanged(this, context);
	});
}


igdeMetaPropertyWidget::Ref igdeMetaPropertyList::CreateWidget(){
	return igdeMetaPropertyListWidget::Ref::New(*this);
}
