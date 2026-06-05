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

#include "igdeWPMetaContext.h"
#include "igdeWPMetaContextList.h"
#include "../igdeUIHelper.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerFlow.h"
#include "../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>


namespace{

class cTextFilterListener : public igdeTextFieldListener{
protected:
	igdeWPMetaContextList &pPanel;
	
public:
	cTextFilterListener(igdeWPMetaContextList &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		pPanel.SetFilter(igdeFilter(textField->GetText()));
	}
	
	void OnTextChanging(igdeTextField *textField) override{
		OnTextChanged(textField);
	}
};

}


// Class igdeWPMetaContextList
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWPMetaContextList::igdeWPMetaContextList(igdeEnvironment &environment) :
igdeContainerBorder(environment, 2)
{
	pCreateContent();
}

igdeWPMetaContextList::~igdeWPMetaContextList(){
	pClearContextPanels();
}


// Management
///////////////

void igdeWPMetaContextList::SetData(const igdeMetaContext::Data::Ref &data){
	if(data == pData){
		return;
	}
	
	pData = data;
	pCreateContextPanels();
	if(pFilter){
		pFilterContextPanels();
	}
}

void igdeWPMetaContextList::SetFilter(const igdeFilter &filter){
	if(filter == pFilter){
		return;
	}
	
	pFilter = filter;
	pFilterContextPanels();
}


// Private Functions
//////////////////////

void igdeWPMetaContextList::pCreateContent(){
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	
	// top panel with filter
	auto topPanel = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaX, igdeContainerFlow::esLast);
	AddChild(topPanel, igdeContainerBorder::eaTop);
	
	topPanel->AddChild(igdeLabel::Ref::New(env,
		"@Igde.MetaContextList.Filter", "@Igde.MetaContextList.Filter.ToolTip",
		env.GetStockIcon(igdeEnvironment::esiSearch)));
	
	helper.EditString(topPanel, "@Igde.MetaContextList.Filter.ToolTip", 20,
		pEditFilter, deTObjectReference<cTextFilterListener>::New(*this));
	
	// content with tab book
	pTabBook = igdeTabBook::Ref::New(env);
	AddChild(pTabBook, igdeContainerBorder::eaCenter);
}

void igdeWPMetaContextList::pCreateContextPanels(){
	pClearContextPanels();
	
	if(!pData){
		return;
	}
	
	igdeEnvironment &env = GetEnvironment();
	
	pData->GetData().Visit([&](const igdeMetaContext::Ref &context){
		auto panel = igdeWPMetaContext::Ref::New(env);
		pTabBook->AddChild(panel, context->GetLabel(), context->GetIcon(), context->GetDescription());
		pContextPanels.Add(panel);
		panel->SetContext(context);
		panel->SetFilter(pFilter);
	});
}

void igdeWPMetaContextList::pClearContextPanels(){
	pContextPanels.RemoveAll();
	pTabBook->RemoveAllChildren();
}

void igdeWPMetaContextList::pFilterContextPanels(){
	pContextPanels.Visit([&](igdeWPMetaContext &panel){
		panel.SetFilter(pFilter);
	});
}
