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
#include "../../undosys/animator/aeUAnimatorMirrorBones.h"
#include "../../undosys/animator/aeUAnimatorMirrorVertexPositionSets.h"

#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/dialog/igdeDialogSetSelect.h>
#include <deigde/meta/property/undo/igdeMetaPropertyStringSetUndo.h>


// Class aeMCPAnimatorAffectedBones::cActionAdd
/////////////////////////////////////////////////

namespace {

class cActionAffectedBonesMirror : public igdeAction{
	aeMCPAnimatorAffectedBones &pProperty;
	const igdeMetaContext::Ref pContext;
	igdeWidget &pOwner;
	
public:
	cActionAffectedBonesMirror(aeMCPAnimatorAffectedBones &property,
		const igdeMetaContext::Ref &context, igdeWidget &owner) :
	igdeAction("@Animator.WPAnimator.Action.BoneMirror", nullptr,
		"@Animator.WPAnimator.Action.BoneMirror.ToolTip"),
	pProperty(property), pContext(context), pOwner(owner){}
	
	void OnAction() override{
		// TODO add a dialog to allow changing the mirror parameter (or add a new menu command)
		if(pProperty.IsValid(pContext)){
			auto undo = aeUAnimatorMirrorBones::Ref::New(&pProperty.Animator(pContext));
			if(undo->HasAnyEffect()){
				pProperty.Animator(pContext).GetUndoSystem()->Add(undo);
			}
		}
	}
	
	void Update() override{
		SetEnabled(pProperty.IsValid(pContext) && pProperty.Animator(pContext).GetListBones().IsNotEmpty());
	}
};

}

decStringSet aeMCPAnimatorAffectedBones::GetValidStrings(const igdeMetaContext::Ref &context) const{
	decStringSet strings;
	const auto rig = Animator(context).GetEngineRig();
	if(rig){
		rig->GetBones().Visit([&](const deRigBone &bone){
			strings.Add(bone.GetName());
		});
	}
	return strings;
}

void aeMCPAnimatorAffectedBones::AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand(menu, igdeMetaPropertyStringSet::ActionAdd::Ref::New(*this, context, owner));
	AddDefaultContextMenuEntries(menu, context, owner);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, deTObjectReference<cActionAffectedBonesMirror>::New(*this, context, owner));
}


// Class aeMCPAnimatorAffectedVertexPositionSets
//////////////////////////////////////////////////

namespace {

class cActionAffectedVertexPositionSetsMirror : public igdeAction{
	aeMCPAnimatorAffectedVertexPositionSets &pProperty;
	const igdeMetaContext::Ref pContext;
	igdeWidget &pOwner;
	
public:
	cActionAffectedVertexPositionSetsMirror(aeMCPAnimatorAffectedVertexPositionSets &property,
		const igdeMetaContext::Ref &context, igdeWidget &owner) :
	igdeAction("@Animator.WPAnimator.Action.VPSMirror", nullptr,
		"@Animator.WPAnimator.Action.VPSMirror.ToolTip"),
	pProperty(property), pContext(context), pOwner(owner){}
	
	void OnAction() override{
		// TODO add a dialog to allow changing the mirror parameter (or add a new menu command)
		if(pProperty.IsValid(pContext)){
			auto undo = aeUAnimatorMirrorVertexPositionSets::Ref::New(&pProperty.Animator(pContext));
			if(undo->HasAnyEffect()){
				pProperty.Animator(pContext).GetUndoSystem()->Add(undo);
			}
		}
	}
	
	void Update() override{
		SetEnabled(pProperty.IsValid(pContext) && pProperty.Animator(pContext).GetListVertexPositionSets().IsNotEmpty());
	}
};

}

decStringSet aeMCPAnimatorAffectedVertexPositionSets::GetValidStrings(const igdeMetaContext::Ref &context) const{
	decStringSet strings;
	const auto component = Animator(context).GetEngineComponent();
	if(component){
		const auto model = component->GetModel();
		if(model){
			model->GetVertexPositionSets().Visit([&](const deModelVertexPositionSet &vps){
				strings.Add(vps.GetName());
			});
		}
	}
	return strings;
}

void aeMCPAnimatorAffectedVertexPositionSets::AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand(menu, igdeMetaPropertyStringSet::ActionAdd::Ref::New(*this, context, owner));
	AddDefaultContextMenuEntries(menu, context, owner);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, deTObjectReference<cActionAffectedVertexPositionSetsMirror>::New(*this, context, owner));
}
