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

#include "igdeMetaPropertyObject.h"
#include "undo/igdeMetaPropertyObjectUndo.h"
#include "widget/igdeMetaPropertyObjectWidget.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyObject::Listener
///////////////////////////////////////////

void igdeMetaPropertyObject::Listener::OnObjectsChanged(igdeMetaPropertyObject*){
}


// Class igdeMetaPropertyObject
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyObject::igdeMetaPropertyObject(
	const char *id, const char *name, const char *description) :
igdeMetaProperty(id, name, description){
}

igdeMetaPropertyObject::~igdeMetaPropertyObject() = default;


// Management
///////////////

void igdeMetaPropertyObject::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

void igdeMetaPropertyObject::NotifyObjectsChanged(){
	pListeners.Notify([&](Listener &listener){
		listener.OnObjectsChanged(this);
	});
}


igdeMetaPropertyObjectUndo::Ref igdeMetaPropertyObject::ChangePropertyValue(
const igdeMetaContext::Ref &context, const deObject::Ref &newValue){
	if(context && context->GetUndoSystem()){
		auto undo = igdeMetaPropertyObjectUndo::Ref::New(*this, context, newValue);
		undo->Redo();
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyObject::CreateWidget(const igdeMetaContext::Ref &context){
	return igdeMetaPropertyObjectWidget::Ref::New(*this, context);
}
