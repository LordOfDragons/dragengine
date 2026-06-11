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

#include "igdeMetaPropertyPath.h"
#include "undo/igdeMetaPropertyPathUndo.h"
#include "widget/igdeMetaPropertyPathWidget.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyPath::Listener
/////////////////////////////////////////

void igdeMetaPropertyPath::Listener::OnBasePathChanged(igdeMetaPropertyPath*, const igdeMetaContext::Ref&){
}


// Class igdeMetaPropertyPath
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyPath::igdeMetaPropertyPath(const char *id, const char *name,
	const char *description, igdeEnvironment::eFilePatternListTypes resourceType) :
igdeMetaProperty(id, name, description),
pResourceType(resourceType){
}

igdeMetaPropertyPath::igdeMetaPropertyPath(const char *id, const char *name,
	const char *description, const igdeFilePattern::List &customPatternList) :
igdeMetaProperty(id, name, description),
pResourceType(igdeEnvironment::eFilePatternListTypes::efpltAll),
pCustomPatternList(customPatternList){
}

igdeMetaPropertyPath::~igdeMetaPropertyPath() = default;


// Management
///////////////

void igdeMetaPropertyPath::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

void igdeMetaPropertyPath::NotifyBasePathChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnBasePathChanged(this, context);
	});
}


igdeMetaPropertyPathUndo::Ref igdeMetaPropertyPath::ChangePropertyValue(
const igdeMetaContext::Ref &context, const char *newValue){
	if(context && context->GetUndoSystem()){
		auto undo = igdeMetaPropertyPathUndo::Ref::New(*this, context, newValue);
		undo->Redo();
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyPath::CreateWidget(const igdeMetaContext::Ref &context){
	return igdeMetaPropertyPathWidget::Ref::New(*this, context);
}
