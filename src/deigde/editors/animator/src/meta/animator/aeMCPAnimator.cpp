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

#include "aeMCPAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/dialog/igdeDialogSetSelect.h>
#include <deigde/meta/property/undo/igdeMetaPropertyStringSetUndo.h>


// Class aeMCPAnimatorAffectedBones::cActionAdd
/////////////////////////////////////////////////

namespace {

class cActionAffectedBoneAdd : public igdeAction{
	aeMCPAnimatorAffectedBones &pProperty;
	const igdeMetaContext::Ref pContext;
	igdeWidget &pOwner;
	
public:
	cActionAffectedBoneAdd(aeMCPAnimatorAffectedBones &property, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment, igdeWidget &owner) :
	igdeAction("@Igde.MetaPropertyList.Action.Add", environment.GetStockIcon(igdeEnvironment::esiPlus),
		"@Igde.MetaPropertyList.Action.Add.ToolTip"),
	pProperty(property), pContext(context), pOwner(owner){}
	
	void OnAction() override{
		if(!pProperty.IsValid(pContext)){
			return;
		}
		
		const auto rig = pProperty.Animator(pContext).GetEngineRig();
		if(!rig){
			return;
		}
		
		decStringSet bones;
		rig->GetBones().Visit([&](const deRigBone &bone){
			bones.Add(bone.GetName());
		});
		
		auto &environment = pOwner.GetEnvironment();
		auto dialog = igdeDialogSetSelect::Ref::New(environment,
			"@Animator.WPAnimator.AffectedBones.Select",
			"@Animator.WPAnimator.AffectedBones.Select.Message", bones);
		
		auto iconPresent = environment.GetStockIcon(igdeEnvironment::esiSmallPlus);
		auto iconAbsent = environment.GetStockIcon(igdeEnvironment::esiSmallMinus);
		
		const auto oldValue = pProperty.GetPropertyValue(pContext);
		dialog->MarkItems(oldValue, iconPresent, iconAbsent);
		if(!dialog->Run(&pOwner)){
			return;
		}
		
		const auto newValue = oldValue + dialog->GetSelection();
		if(newValue != oldValue){
			pProperty.ChangePropertyValue(pContext, newValue);
		}
	}
};

}

igdeAction::Ref aeMCPAnimatorAffectedBones::CreateButtonAction(TargetButton target, const igdeMetaContext::Ref &context, igdeWidget &owner){
	switch(target){
	case TargetButton::add:
		return deTObjectReference<cActionAffectedBoneAdd>::New(*this, context, owner.GetEnvironment(), owner);
		
	default:
		return CreateDefaultButtonAction(target, context, owner);
	}
}

void aeMCPAnimatorAffectedBones::AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand(menu, deTObjectReference<cActionAffectedBoneAdd>::New(*this, context, menu.GetEnvironment(), owner));
	AddDefaultContextMenuEntries(menu, context, owner);
}


// Class aeMCPAnimatorAffectedVertexPositionSets
//////////////////////////////////////////////////

igdeAction::Ref aeMCPAnimatorAffectedVertexPositionSets::CreateButtonAction(TargetButton target, const igdeMetaContext::Ref &context, igdeWidget &owner){
	return CreateDefaultButtonAction(target, context, owner);
}

void aeMCPAnimatorAffectedVertexPositionSets::AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner){
	AddDefaultContextMenuEntries(menu, context, owner);
}
