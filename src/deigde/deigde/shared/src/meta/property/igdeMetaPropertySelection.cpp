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
#include "widget/igdeMetaPropertySelectionWidget.h"


// Class igdeMetaPropertySelection
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertySelection::igdeMetaPropertySelection(const char *id, const char *name,
	const char *description, const igdeListItem::List &choices) :
igdeMetaProperty(id, name, description),
pChoices(choices),
pDefaultValue(nullptr){
}

igdeMetaPropertySelection::~igdeMetaPropertySelection() = default;


// Management
///////////////

void igdeMetaPropertySelection::SetDefaultValue(void *value){
	pDefaultValue = value;
}

void igdeMetaPropertySelection::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

igdeMetaPropertyWidget::Ref igdeMetaPropertySelection::CreateWidget(const igdeMetaContext::Ref &context){
	return igdeMetaPropertySelectionWidget::Ref::New(*this, context);
}


// TEST
#if 0
#include "../../environment/igdeEnvironment.h"
class TestClass : public igdeMetaPropertySelectionEnum<igdeEnvironment::eFilePatternListTypes> {
public:
	TestClass() : igdeMetaPropertySelectionEnum("Test", "Test description", CreateChoices()){ SetDefaultValueEnum(igdeEnvironment::eFilePatternListTypes::efpltModel); }
	igdeMetaContext::Ref Capture(const igdeMetaContext::Ref &context) const override{ return {}; }
	bool IsValid(const igdeMetaContext::Ref &context) const override{ return true; }
	igdeEnvironment::eFilePatternListTypes GetPropertyValueEnum(const igdeMetaContext::Ref &context) const override{ return igdeEnvironment::eFilePatternListTypes::efpltAll; }
	void SetPropertyValueEnum(const igdeMetaContext::Ref &context, igdeEnvironment::eFilePatternListTypes value) override{ }
	
	static igdeListItem::List CreateChoices(){
		igdeListItem::List c;
		AddChoice(c, igdeEnvironment::eFilePatternListTypes::efpltAll, "All");
		AddChoice(c, igdeEnvironment::eFilePatternListTypes::efpltAnimation, "Animation");
		AddChoice(c, igdeEnvironment::eFilePatternListTypes::efpltModel, "Model");
		return c;
	}
};
static TestClass vTestClass;

void test(){
	vTestClass.SetPropertyValue({}, vTestClass.GetDefaultValue());
	vTestClass.SetPropertyValueEnum({}, vTestClass.GetDefaultValueEnum());
}
#endif
