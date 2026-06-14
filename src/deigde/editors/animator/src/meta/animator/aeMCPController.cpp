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

#include "aeMCPController.h"
#include "../../animator/link/aeLink.h"
#include "../../gui/aeWindowMain.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/meta/property/undo/igdeMetaPropertyListUndo.h>

#include <dragengine/common/collection/decTList.h>


// Class aeMCPControllers
///////////////////////////

namespace {

class cUndoSetControllers : public igdeMetaPropertyListUndo{
public:
	struct sLinkRemoveController{
		aeLink::Ref link;
		aeController::Ref controller;
	};
	
private:
	decTList<sLinkRemoveController> pLinksRemoveControllers;
	
public:
	cUndoSetControllers(aeMCPControllers &property, const igdeMetaContext::Ref &context,
	const igdeMetaPropertyList::List &newValue, const char *undoInfo, const char *undoInfoLong) :
	igdeMetaPropertyListUndo(property, context, newValue, undoInfo, undoInfoLong)
	{
		const auto &animator = property.Animator(context);
		const auto oldControllers = property.ConvertList(GetOldValue());
		const auto newControllers = property.ConvertList(GetNewValue());
		const auto &links = animator.GetLinks();
		
		oldControllers.Visit([&](const aeController::Ref &controller){
			if(newControllers.Has(controller)){
				return;
			}
			
			links.Visit([&](const aeLink::Ref &link){
				if(link->GetController() == controller){
					pLinksRemoveControllers.Add({link, controller});
				}
			});
		});
	}
	
	inline const decTList<sLinkRemoveController> &GetLinksRemoveControllers() const{ return pLinksRemoveControllers; }
	
	void Undo() override{
		igdeMetaPropertyListUndo::Undo();
		pLinksRemoveControllers.Visit([&](const sLinkRemoveController &each){
			each.link->SetController(each.controller, false);
		});
	}
	
	void Redo() override{
		pLinksRemoveControllers.Visit([&](const sLinkRemoveController &each){
			each.link->SetController(nullptr, false);
		});
		igdeMetaPropertyListUndo::Redo();
	}
};

}

igdeMetaPropertyListUndo::Ref aeMCPControllers::ChangePropertyValue(const ContextRef &context,
const List &newValue, const char *undoInfo, const char *undoInfoLong){
	const auto undo = deTObjectReference<cUndoSetControllers>::New(*this, context, newValue, undoInfo, undoInfoLong);
	
	const auto &linksRemoveControllers = undo->GetLinksRemoveControllers();
	if(linksRemoveControllers.IsNotEmpty()){
		decStringList names;
		linksRemoveControllers.Visit([&](const cUndoSetControllers::sLinkRemoveController &each){
			names.Add(each.link->GetName());
		});
		names.SortAscending();
		auto strNames = names.GetCount() > 5 ? DEJoin(names.GetHead(5), ", ") + ", ..." : DEJoin(names, ", ");
		
		if(igdeCommonDialogs::QuestionFormat(WindowMain(context), igdeCommonDialogs::ebsYesNo,
		"@Animator.Dialog.RemoveController.Title", "@Animator.Dialog.RemoveController.Message",
		names.GetCount(), strNames.GetString()) != igdeCommonDialogs::ebYes){
			return {};
		}
	}
	
	if(context->GetUndoSystem()){
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		undo->Redo();
		return {};
	}
}

igdeAction::Ref aeMCPControllers::CreateButtonAction(TargetButton target, const ContextRef &context, igdeWidget &owner){
	switch(target){
	case TargetButton::add:
		return Animator(context).GetWindowMain().GetActionControllerAdd();
		
	default:
		return CreateDefaultButtonAction(target, context, owner);
	}
}

void aeMCPControllers::AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner){
	const auto &windowMain = Animator(context).GetWindowMain();
	auto &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand(menu, windowMain.GetActionControllerAdd());
	helper.MenuCommand(menu, windowMain.GetActionControllerDuplicate());
	AddDefaultContextMenuEntries(menu, context, owner);
}
