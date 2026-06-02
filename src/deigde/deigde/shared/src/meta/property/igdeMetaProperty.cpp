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

#include "igdeMetaProperty.h"


// Class igdeMetaProperty
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaProperty::igdeMetaProperty(const char *name, const char *description) :
pLabel(name),
pDescription(description){
}

igdeMetaProperty::~igdeMetaProperty() = default;


// Management
///////////////

void igdeMetaProperty::SetLabel(const char *label){
	pLabel = label;
}

void igdeMetaProperty::SetDescription(const char *description){
	pDescription = description;
}

void igdeMetaProperty::SetFilter(const char *filter){
	pFilter = filter;
}

void igdeMetaProperty::SetUndoInfo(const char *undoInfo){
	pUndoInfo = undoInfo;
}


const decString &igdeMetaProperty::GetFilterOrLabel() const{
	return pFilter.IsEmpty() ? pLabel : pFilter;
}

const decString &igdeMetaProperty::GetUndoInfoOrLabel() const{
	return pUndoInfo.IsEmpty() ? pLabel : pUndoInfo;
}

void igdeMetaProperty::AddContextMenuEntries(igdeMenuCascade&, const igdeMetaContext::Ref&){
}
