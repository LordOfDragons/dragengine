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

#include "igdeMetaPropertySelection.h"
#include "undo/igdeMetaPropertySelectionUndo.h"
#include "widget/igdeMetaPropertySelectionWidget.h"
#include "../igdeMetaContext.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertySelection::Listener
//////////////////////////////////////////////

void igdeMetaPropertySelection::Listener::OnChoicesChanged(igdeMetaPropertySelection*){
}


// Class igdeMetaPropertySelection
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertySelection::igdeMetaPropertySelection(
	const char *id, const char *name, const char *description) :
igdeMetaProperty(id, name, description),
pDefaultValue(nullptr){
}

igdeMetaPropertySelection::~igdeMetaPropertySelection() = default;


// Management
///////////////

void igdeMetaPropertySelection::SetChoices(const ListChoices &choices){
	pChoices = choices;
}

void igdeMetaPropertySelection::SetDefaultValue(void *value){
	pDefaultValue = value;
}

void igdeMetaPropertySelection::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

void igdeMetaPropertySelection::NotifyChoicesChanged(){
	pListeners.Notify([&](Listener &listener){
		listener.OnChoicesChanged(this);
	});
}

igdeMetaPropertySelectionUndo::Ref igdeMetaPropertySelection::ChangePropertyValue(
const igdeMetaContext::Ref &context, void *newValue, const char *undoInfo, const char *undoInfoLong){
	if(context && context->GetUndoSystem()){
		auto undo = igdeMetaPropertySelectionUndo::Ref::New(
			*this, context, newValue, undoInfo, undoInfoLong);
		undo->Redo();
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertyWidget::Ref igdeMetaPropertySelection::CreateWidget(const igdeMetaContext::Ref &context){
	return igdeMetaPropertySelectionWidget::Ref::New(*this, context);
}


// TEST
#if 0
#include "../../environment/igdeEnvironment.h"
#include "../igdeMetaContextItemInfo.h"
class TestClass : public igdeMetaPropertySelectionEnum<igdeEnvironment::eFilePatternListTypes> {
public:
	TestClass() : igdeMetaPropertySelectionEnum("test.property", "Test", "Test description"){
		ListChoicesEnum choices;
		choices.Add(igdeEnvironment::eFilePatternListTypes::efpltAll);
		choices.Add(igdeEnvironment::eFilePatternListTypes::efpltAnimation);
		choices.Add(igdeEnvironment::eFilePatternListTypes::efpltModel);
		SetChoicesEnum(choices);
		
		SetDefaultValueEnum(igdeEnvironment::eFilePatternListTypes::efpltModel);
	}
	igdeMetaContext::Ref Capture(const igdeMetaContext::Ref &context) const override{ return {}; }
	bool IsValid(const igdeMetaContext::Ref &context) const override{ return true; }
	igdeEnvironment::eFilePatternListTypes GetPropertyValueEnum(const igdeMetaContext::Ref &context) const override{ return igdeEnvironment::eFilePatternListTypes::efpltAll; }
	void SetPropertyValueEnum(const igdeMetaContext::Ref &context, igdeEnvironment::eFilePatternListTypes value) override{ }
	
	void GetChoiceItemInfoEnum(igdeEnvironment::eFilePatternListTypes choice, igdeMetaContextItemInfo &info) const override{
		switch(choice){
		case igdeEnvironment::eFilePatternListTypes::efpltAll:
			info.SetAll("All");
			break;
		case igdeEnvironment::eFilePatternListTypes::efpltAnimation:
			info.SetAll("Animation");
			break;
		case igdeEnvironment::eFilePatternListTypes::efpltModel:
			info.SetAll("Model");
			break;
		default:
			info.SetAll("?");
			break;
		}
	}
};
static TestClass vTestClass;

void test(){
	vTestClass.SetPropertyValue({}, vTestClass.GetDefaultValue());
	vTestClass.SetPropertyValueEnum({}, vTestClass.GetDefaultValueEnum());
}
#endif
