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

#include "igdeMetaPropertyVector2.h"
#include "undo/igdeMetaPropertyVector2Undo.h"
#include "widget/igdeMetaPropertyVector2Widget.h"
#include "../igdeMetaContext.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyVector2
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyVector2::igdeMetaPropertyVector2(
	const char *id, const char *name, const char *description) :
igdeMetaProperty(id, name, description){
}

igdeMetaPropertyVector2::~igdeMetaPropertyVector2() = default;


// Management
///////////////

void igdeMetaPropertyVector2::SetDefaultValue(const decVector2 &value){
	pDefaultValue = value;
}

void igdeMetaPropertyVector2::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

igdeMetaPropertyVector2Undo::Ref igdeMetaPropertyVector2::ChangePropertyValue(
const igdeMetaContext::Ref &context, const decVector2 &newValue,
const char *undoInfo, const char *undoInfoLong){
	if(context && context->GetUndoSystem()){
		auto undo = igdeMetaPropertyVector2Undo::Ref::New(
			*this, context, newValue, undoInfo, undoInfoLong);
		undo->Redo();
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyVector2::CreateWidget(const igdeMetaContext::Ref &context){
	return igdeMetaPropertyVector2Widget::Ref::New(*this, context);
}
