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

#include "igdeMetaPropertyAdapters.h"


// igdeMetaPropertyAdapter::StringOnListChanged
/////////////////////////////////////////////////

igdeMetaPropertyAdapter::StringOnListChanged::StringOnListChanged(igdeMetaPropertyString &property) :
pProperty(property){
}

igdeMetaPropertyAdapter::StringOnListChanged::~StringOnListChanged() = default;

void igdeMetaPropertyAdapter::StringOnListChanged::OnValueChanged(igdeMetaPropertyList*, const igdeMetaContext::Ref&){
	pProperty.NotifyAllowedStringsChanged({});
}

void igdeMetaPropertyAdapter::StringOnListChanged::OnObjectItemInfoChanged(igdeMetaPropertyList*, const igdeMetaContext::Ref&){
	pProperty.NotifyAllowedStringsChanged({});
}


// igdeMetaPropertyAdapter::ObjectOnListChanged
/////////////////////////////////////////////////

igdeMetaPropertyAdapter::ObjectOnListChanged::ObjectOnListChanged(igdeMetaPropertyObject &property) :
pProperty(property){
}

igdeMetaPropertyAdapter::ObjectOnListChanged::~ObjectOnListChanged() = default;

void igdeMetaPropertyAdapter::ObjectOnListChanged::OnValueChanged(igdeMetaPropertyList*, const igdeMetaContext::Ref&){
	pProperty.NotifyAllowedObjectsChanged({});
}

void igdeMetaPropertyAdapter::ObjectOnListChanged::OnObjectItemInfoChanged(igdeMetaPropertyList*, const igdeMetaContext::Ref&){
	pProperty.NotifyAllowedObjectsChanged({});
}


// igdeMetaPropertyAdapter::TreeOnListChanged
///////////////////////////////////////////////

igdeMetaPropertyAdapter::TreeOnListChanged::TreeOnListChanged(igdeMetaPropertyTreeList &property) :
pProperty(property){
}

igdeMetaPropertyAdapter::TreeOnListChanged::~TreeOnListChanged() = default;

void igdeMetaPropertyAdapter::TreeOnListChanged::OnValueChanged(igdeMetaPropertyList*, const igdeMetaContext::Ref&){
	pProperty.NotifyValueChanged({});
}

void igdeMetaPropertyAdapter::TreeOnListChanged::OnObjectItemInfoChanged(igdeMetaPropertyList*, const igdeMetaContext::Ref&){
	pProperty.NotifyObjectItemInfoChanged({});
}


// igdeMetaPropertyAdapter::StringOnStringSetChanged
//////////////////////////////////////////////////////

igdeMetaPropertyAdapter::StringOnStringSetChanged::StringOnStringSetChanged(igdeMetaPropertyString &property) :
pProperty(property){
}

igdeMetaPropertyAdapter::StringOnStringSetChanged::~StringOnStringSetChanged() = default;

void igdeMetaPropertyAdapter::StringOnStringSetChanged::OnValueChanged(igdeMetaPropertyStringSet*, const igdeMetaContext::Ref&){
	pProperty.NotifyAllowedStringsChanged({});
}


// igdeMetaPropertyAdapter::ListOnStringChanged
/////////////////////////////////////////////////

igdeMetaPropertyAdapter::ListOnStringChanged::ListOnStringChanged(igdeMetaPropertyList &property) :
pProperty(property){
}

igdeMetaPropertyAdapter::ListOnStringChanged::~ListOnStringChanged() = default;

void igdeMetaPropertyAdapter::ListOnStringChanged::OnValueChanged(igdeMetaPropertyString*, const igdeMetaContext::Ref&){
	pProperty.NotifyObjectItemInfoChanged({});
}


// igdeMetaPropertyAdapter
////////////////////////////

void igdeMetaPropertyAdapter::OnChanged(igdeMetaPropertyList &watch, igdeMetaPropertyString &target){
	watch.GetListeners().Add(StringOnListChanged::Ref::New(target));
}

void igdeMetaPropertyAdapter::OnChanged(igdeMetaPropertyList &watch, igdeMetaPropertyObject &target){
	watch.GetListeners().Add(ObjectOnListChanged::Ref::New(target));
}

void igdeMetaPropertyAdapter::OnChanged(igdeMetaPropertyList &watch, igdeMetaPropertyTreeList &target){
	watch.GetListeners().Add(TreeOnListChanged::Ref::New(target));
}

void igdeMetaPropertyAdapter::OnChanged(igdeMetaPropertyStringSet &watch, igdeMetaPropertyString &target){
	watch.GetListeners().Add(StringOnStringSetChanged::Ref::New(target));
}

void igdeMetaPropertyAdapter::OnChanged(igdeMetaPropertyString &watch, igdeMetaPropertyList &target){
	watch.GetListeners().Add(ListOnStringChanged::Ref::New(target));
}
