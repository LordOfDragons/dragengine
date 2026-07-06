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


void igdeMetaPropertyAdapter::OnChanged(igdeMetaPropertyList &watch, igdeMetaPropertyString &target){
	watch.GetListeners().Add(NotifyStringOnChanged<igdeMetaPropertyString,
		WatchValueObjectItemInfoChanged<igdeMetaPropertyList>>::Ref::New(target));
}

void igdeMetaPropertyAdapter::OnChanged(igdeMetaPropertyList &watch, igdeMetaPropertyObject &target){
	watch.GetListeners().Add(NotifyObjectOnChanged<igdeMetaPropertyObject,
		WatchValueObjectItemInfoChanged<igdeMetaPropertyList>>::Ref::New(target));
}

void igdeMetaPropertyAdapter::OnChanged(igdeMetaPropertyList &watch, igdeMetaPropertyTreeList &target){
	watch.GetListeners().Add(NotifyTreeListOnChanged<igdeMetaPropertyTreeList,
		WatchValueObjectItemInfoChanged<igdeMetaPropertyList>>::Ref::New(target));
}


void igdeMetaPropertyAdapter::OnChanged(igdeMetaPropertySet &watch, igdeMetaPropertyString &target){
	watch.GetListeners().Add(NotifyStringOnChanged<igdeMetaPropertyString,
		WatchValueObjectItemInfoChanged<igdeMetaPropertySet>>::Ref::New(target));
}

void igdeMetaPropertyAdapter::OnChanged(igdeMetaPropertySet &watch, igdeMetaPropertyObject &target){
	watch.GetListeners().Add(NotifyObjectOnChanged<igdeMetaPropertyObject,
		WatchValueObjectItemInfoChanged<igdeMetaPropertySet>>::Ref::New(target));
}

void igdeMetaPropertyAdapter::OnChanged(igdeMetaPropertySet &watch, igdeMetaPropertyTreeList &target){
	watch.GetListeners().Add(NotifyTreeListOnChanged<igdeMetaPropertyTreeList,
		WatchValueObjectItemInfoChanged<igdeMetaPropertySet>>::Ref::New(target));
}


void igdeMetaPropertyAdapter::OnChanged(igdeMetaPropertyStringSet &watch, igdeMetaPropertyString &target){
	watch.GetListeners().Add(NotifyStringOnChanged<igdeMetaPropertyString,
		WatchValueChanged<igdeMetaPropertyStringSet>>::Ref::New(target));
}

void igdeMetaPropertyAdapter::OnChanged(igdeMetaPropertyStringSet &watch, igdeMetaPropertyObject &target){
	watch.GetListeners().Add(NotifyObjectOnChanged<igdeMetaPropertyObject,
		WatchValueChanged<igdeMetaPropertyStringSet>>::Ref::New(target));
}

void igdeMetaPropertyAdapter::OnChanged(igdeMetaPropertyStringSet &watch, igdeMetaPropertyTreeList &target){
	watch.GetListeners().Add(NotifyTreeListOnChanged<igdeMetaPropertyTreeList,
		WatchValueChanged<igdeMetaPropertyStringSet>>::Ref::New(target));
}


void igdeMetaPropertyAdapter::OnChanged(igdeMetaPropertyString &watch, igdeMetaPropertyList &target){
	watch.GetListeners().Add(NotifyListOnChanged<igdeMetaPropertyList,
		WatchValueChanged<igdeMetaPropertyString>>::Ref::New(target));
}

void igdeMetaPropertyAdapter::OnChanged(igdeMetaPropertyString &watch, igdeMetaPropertySet &target){
	watch.GetListeners().Add(NotifyListOnChanged<igdeMetaPropertySet,
		WatchValueChanged<igdeMetaPropertyString>>::Ref::New(target));
}

void igdeMetaPropertyAdapter::OnChanged(igdeMetaPropertyString &watch, igdeMetaPropertyTreeList &target){
	watch.GetListeners().Add(NotifyTreeListOnChanged<igdeMetaPropertyTreeList,
		WatchValueChanged<igdeMetaPropertyString>>::Ref::New(target));
}
