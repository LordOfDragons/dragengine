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

#include "igdeMetaContext.h"
#include "property/igdeMetaProperty.h"


// Class igdeMetaContext::Listener
////////////////////////////////////

igdeMetaContext::Listener::Listener() = default;
igdeMetaContext::Listener::~Listener() = default;

void igdeMetaContext::Listener::OnPropertiesChanged(igdeMetaContext*){}


// Class igdeMetaContext
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaContext::igdeMetaContext(const char *identifier) :
pIdentifier(identifier),
pUndoSystem(nullptr){
}

igdeMetaContext::~igdeMetaContext() = default;


// Management
///////////////

void igdeMetaContext::SetLabel(const char *label){
	pLabel = label;
}

void igdeMetaContext::SetDescription(const char *description){
	pDescription = description;
}

void igdeMetaContext::SetIcon(const igdeIcon::Ref &icon){
	pIcon = icon;
}

void igdeMetaContext::SetUndoSystem(igdeUndoSystem *undoSystem){
	pUndoSystem = undoSystem;
}

void igdeMetaContext::NotifyPropertiesChanged(){
	pListeners.Notify([&](Listener &listener){
		listener.OnPropertiesChanged(this);
	});
}
