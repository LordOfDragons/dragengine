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

#include "igdeMetaPropertyDirectory.h"
#include "undo/igdeMetaPropertyDirectoryUndo.h"
#include "widget/igdeMetaPropertyDirectoryWidget.h"
#include "../igdeMetaContext.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyDirectory
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyDirectory::igdeMetaPropertyDirectory(
	const char *id, const char *name, const char *description, bool useGameVFS) :
igdeMetaProperty(id, name, description),
pUseGameVFS(useGameVFS){
}

igdeMetaPropertyDirectory::~igdeMetaPropertyDirectory() = default;


// Management
///////////////

void igdeMetaPropertyDirectory::SetUseGameVFS(bool useGameVFS){
	pUseGameVFS = useGameVFS;
}

void igdeMetaPropertyDirectory::SetDefaultValue(const decString &value){
	pDefaultValue = value;
}

void igdeMetaPropertyDirectory::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

igdeMetaPropertyDirectoryUndo::Ref igdeMetaPropertyDirectory::ChangePropertyValue(
const igdeMetaContext::Ref &context, const char *newValue,
const char *undoInfo, const char *undoInfoLong){
	if(context && context->GetUndoSystem()){
		auto undo = igdeMetaPropertyDirectoryUndo::Ref::New(
			*this, context, newValue, undoInfo, undoInfoLong);
		undo->Redo();
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyDirectory::CreateWidget(const igdeMetaContext::Ref &context){
	return igdeMetaPropertyDirectoryWidget::Ref::New(*this, context);
}


// igdeMetaPropertyDirectoryStorage
/////////////////////////////////////

igdeMetaPropertyDirectoryStorage::igdeMetaPropertyDirectoryStorage(
	const char *id, const char *name, const char *description) :
igdeMetaPropertyDirectory(id, name, description){
}

igdeMetaPropertyDirectoryStorage::~igdeMetaPropertyDirectoryStorage() = default;

const decString &igdeMetaPropertyDirectoryStorage::GetPropertyValue(const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetValue();
}

void igdeMetaPropertyDirectoryStorage::SetPropertyValue(const igdeMetaContext::Ref &context, const decString &value){
	GetStorage(context).SetValue(value);
}
