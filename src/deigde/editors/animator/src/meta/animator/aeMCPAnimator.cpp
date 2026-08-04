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
#include "aeMCPShared.h"
#include "../../gui/aeWindowMain.h"

#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/dialog/igdeDialogSetSelect.h>
#include <deigde/meta/property/undo/igdeMetaPropertyStringSetUndo.h>

#include <dragengine/common/collection/decTList.h>


// Class aeMCPAnimatorAffectedBones
/////////////////////////////////////

namespace {

class cActionAffectedBonesMirror : public aeActionMirrorStringSet{
	aeMCPAnimatorAffectedBones &pProperty;
	
public:
	cActionAffectedBonesMirror(aeMCPAnimatorAffectedBones &property, igdeWidget &owner, const igdeMetaContext::Ref &context) :
	aeActionMirrorStringSet(property, owner, context),
	pProperty(property){
		SetText("@Animator.WPAnimator.Action.BoneMirror");
		SetDescription("@Animator.WPAnimator.Action.BoneMirror.ToolTip");
	}
};

class cActionAffectedBonesMirrorSelected : public cActionAffectedBonesMirror{
public:
	cActionAffectedBonesMirrorSelected(aeMCPAnimatorAffectedBones &property, igdeWidget &owner, const igdeMetaContext::Ref &context) :
	cActionAffectedBonesMirror(property, owner, context){
		pSelected = true;
		SetText("@Animator.WPAnimator.Action.BoneMirrorSelected");
		SetDescription("@Animator.WPAnimator.Action.BoneMirrorSelected.ToolTip");
	}
};

}

decStringSet aeMCPAnimatorAffectedBones::GetAllowedStrings(const igdeMetaContext::Ref &context) const{
	return Owner(context).mpHiddenBoneNames;
}

void aeMCPAnimatorAffectedBones::AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand(menu, igdeMetaPropertyStringSet::ActionAdd::Ref::New(*this, owner, context));
	AddDefaultContextMenuEntries(menu, context, owner);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, deTObjectReference<cActionAffectedBonesMirror>::New(*this, owner, context));
	helper.MenuCommand(menu, deTObjectReference<cActionAffectedBonesMirrorSelected>::New(*this, owner, context));
}


// Class aeMCPAnimatorAffectedVertexPositionSets
//////////////////////////////////////////////////

namespace {

class cActionAffectedVpsMirror : public aeActionMirrorStringSet{
	aeMCPAnimatorAffectedVertexPositionSets &pProperty;
	
public:
	cActionAffectedVpsMirror(aeMCPAnimatorAffectedVertexPositionSets &property, igdeWidget &owner, const igdeMetaContext::Ref &context) :
	aeActionMirrorStringSet(property, owner, context),
	pProperty(property){
		SetText("@Animator.WPAnimator.Action.VPSMirror");
		SetDescription("@Animator.WPAnimator.Action.VPSMirror.ToolTip");
	}
};

class cActionAffectedVpsMirrorSelected : public cActionAffectedVpsMirror{
public:
	cActionAffectedVpsMirrorSelected(aeMCPAnimatorAffectedVertexPositionSets &property, igdeWidget &owner, const igdeMetaContext::Ref &context) :
	cActionAffectedVpsMirror(property, owner, context){
		pSelected = true;
		SetText("@Animator.WPAnimator.Action.VPSMirrorSelected");
		SetDescription("@Animator.WPAnimator.Action.VPSMirrorSelected.ToolTip");
	}
};

}

decStringSet aeMCPAnimatorAffectedVertexPositionSets::GetAllowedStrings(const igdeMetaContext::Ref &context) const{
	return Owner(context).mpHiddenVpsNames;
}

void aeMCPAnimatorAffectedVertexPositionSets::AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand(menu, igdeMetaPropertyStringSet::ActionAdd::Ref::New(*this, owner, context));
	AddDefaultContextMenuEntries(menu, context, owner);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, deTObjectReference<cActionAffectedVpsMirror>::New(*this, owner, context));
	helper.MenuCommand(menu, deTObjectReference<cActionAffectedVpsMirrorSelected>::New(*this, owner, context));
}


// Playground controllers
///////////////////////////

namespace{

class cActionPlaygroundControllersReset : public igdeMetaProperty::Action{
	aeMCPAnimatorPlaygroundControllers &pProperty;
	
public:
	cActionPlaygroundControllersReset(aeMCPAnimatorPlaygroundControllers &property, igdeWidget &owner, const igdeMetaContext::Ref &context) :
	igdeMetaProperty::Action(owner, context, "@Animator.WPPlayground.Action.ControllersReset",
		nullptr, "@Animator.WPPlayground.Action.ControllersReset.ToolTip"),
	pProperty(property){}
	
	~cActionPlaygroundControllersReset() override = default;
	
	void OnAction() override{
		auto &context = GetContext();
		if(!pProperty.IsValid(context)){
			return;
		}
		
		pProperty.Owner(context).mpControllers->Visit([&](aeController &controller){
			controller.ResetValue();
		});
	}
};

class cActionPlaygroundControllersFullReset : public igdeMetaProperty::Action{
	aeMCPAnimatorPlaygroundControllers &pProperty;
	
public:
	cActionPlaygroundControllersFullReset(aeMCPAnimatorPlaygroundControllers &property, igdeWidget &owner, const igdeMetaContext::Ref &context) :
	igdeMetaProperty::Action(owner, context, "@Animator.WPPlayground.Action.ControllersFullReset",
		nullptr, "@Animator.WPPlayground.Action.ControllersFullReset.ToolTip"),
	pProperty(property){}
	
	~cActionPlaygroundControllersFullReset() override = default;
	
	void OnAction() override{
		auto &context = GetContext();
		if(!pProperty.IsValid(context)){
			return;
		}
		
		pProperty.Owner(context).mpControllers->Visit([&](aeController &controller){
			controller.ResetValue(true);
		});
	}
};

}

aeMCPAnimatorPlaygroundControllers::aeMCPAnimatorPlaygroundControllers(igdeMetaPropertyString *propertyName, igdeMetaPropertyFloat *propertyValue) :
igdeMetaPropertyMCTNoCapture("animator.playgroundControllers", propertyName, propertyValue){
}

aeMCPAnimatorPlaygroundControllers::~aeMCPAnimatorPlaygroundControllers() = default;

aeMCPAnimatorPlaygroundControllers::Storage &aeMCPAnimatorPlaygroundControllers::GetStorage(const igdeMetaContext::Ref &context) const{
	return Owner(context).mpPlaygroundControllers;
}

void aeMCPAnimatorPlaygroundControllers::AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	helper.MenuCommand(menu, deTObjectReference<cActionPlaygroundControllersReset>::New(*this, owner, context));
	helper.MenuCommand(menu, deTObjectReference<cActionPlaygroundControllersFullReset>::New(*this, owner, context));
}


// Class aeMCPAnimatorSky
///////////////////////////

aeMCPAnimatorSky::aeMCPAnimatorSky() : igdeMetaPropertyMCT("animator.sky", "", ""){
	SetProperties(igdeWSky::MetaProperties::global.properties);
}


// Class aeMCPAnimatorEnvironmentObject
/////////////////////////////////////////

aeMCPAnimatorEnvironmentObject::aeMCPAnimatorEnvironmentObject() : igdeMetaPropertyMCT("animator.environmentObject", "", ""){
	SetProperties(igdeWObject::MetaProperties::global.properties);
}


// Class aeMCPAnimatorCamera
//////////////////////////////

aeMCPAnimatorCamera::aeMCPAnimatorCamera() : igdeMetaPropertyMCT("animator.camera", "", ""){
	SetProperties(igdeCamera::MetaProperties::global.properties);
}


// Class aeMCPAnimatorPaused
//////////////////////////////

namespace {

class cActionPlaybackReset : public igdeMetaProperty::Action{
	aeMCPAnimatorPaused &pProperty;
	
public:
	cActionPlaybackReset(aeMCPAnimatorPaused &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
	Action(owner, context, "@Animator.WPView.Reset", nullptr, "@Animator.WPView.Reset.ToolTip"), pProperty(property){}
	
	void OnAction() override{
		const auto &context = GetContext();
		if(!pProperty.IsValid(context)){
			return;
		}
		
		pProperty.Owner(context).mpControllers->Visit([](aeController &controller){
			controller.ResetValue();
		});
	}
};

}

void aeMCPAnimatorPaused::AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand(menu, deTObjectReference<cActionPlaybackReset>::New(*this, owner, context));
}
