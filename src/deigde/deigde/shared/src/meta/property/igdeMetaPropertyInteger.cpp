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

#include "igdeMetaPropertyInteger.h"
#include "undo/igdeMetaPropertyIntegerUndo.h"
#include "widget/igdeMetaPropertyIntegerWidget.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyInteger
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyInteger::igdeMetaPropertyInteger(
	const char *id, const char *name, const char *description) :
igdeMetaProperty(id, name, description),
pDefaultValue(0),
pLowerLimit(0),
pUpperLimit(100),
pTickSpacing(10),
pEnableLowerLimit(false),
pEnableUpperLimit(false),
pEnableSpin(false){
}

igdeMetaPropertyInteger::~igdeMetaPropertyInteger() = default;


// Management
///////////////

void igdeMetaPropertyInteger::SetDefaultValue(int value){
	pDefaultValue = value;
}


void igdeMetaPropertyInteger::SetLowerLimit(int value){
	pLowerLimit = value;
}

void igdeMetaPropertyInteger::SetUpperLimit(int value){
	pUpperLimit = value;
}

void igdeMetaPropertyInteger::SetTickSpacing(int value){
	pTickSpacing = decMath::max(value, 1);
}

void igdeMetaPropertyInteger::SetEnableLowerLimit(bool enable){
	pEnableLowerLimit = enable;
}

void igdeMetaPropertyInteger::SetEnableUpperLimit(bool enable){
	pEnableUpperLimit = enable;
}

void igdeMetaPropertyInteger::SetEnableSpin(bool enable){
	pEnableSpin = enable;
}

void igdeMetaPropertyInteger::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

igdeMetaPropertyIntegerUndo::Ref igdeMetaPropertyInteger::ChangePropertyValue(
const igdeMetaContext::Ref &context, int newValue){
	if(context && context->GetUndoSystem()){
		auto undo = igdeMetaPropertyIntegerUndo::Ref::New(*this, context, newValue);
		undo->Redo();
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyInteger::CreateWidget(const igdeMetaContext::Ref &context){
	return igdeMetaPropertyIntegerWidget::Ref::New(*this, context);
}
