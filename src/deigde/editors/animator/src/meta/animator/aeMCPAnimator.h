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

#ifndef _AEMCPANIMATOR_H_
#define _AEMCPANIMATOR_H_

#include "../../animator/aeAnimator.h"
#include "../../animator/aeSubAnimator.h"

#include <deigde/meta/igdeMetaContextItemInfo.h>
#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyContext.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyGroup.h>
#include <deigde/meta/property/igdeMetaPropertyPath.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertySliderBoard.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyStringSet.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>
#include <deigde/meta/property/igdeMetaPropertyUndoHistory.h>


/** Hidden. */
class aeMCPHiddenBoneNames : public aeAnimator::MetaProperty<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPHiddenBoneNames() : igdeMetaPropertyMCT("animator.animator.hiddenBoneNames", "", ""){}
	~aeMCPHiddenBoneNames() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpHiddenBoneNames;
	}
};

class aeMCPHiddenVPSNames : public aeAnimator::MetaProperty<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPHiddenVPSNames() : igdeMetaPropertyMCT("animator.animator.hiddenVPSNames", "", ""){}
	~aeMCPHiddenVPSNames() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpHiddenVpsNames;
	}
};

class aeMCPHiddenMoveNames : public aeAnimator::MetaProperty<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPHiddenMoveNames() : igdeMetaPropertyMCT("animator.animator.hiddenMoveNames", "", ""){}
	~aeMCPHiddenMoveNames() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpHiddenMoveNames;
	}
};


/** Rig path. */
class aeMCPAnimatorRig : public aeAnimator::MetaProperty<igdeMetaPropertyPathStorage>{
public:
	aeMCPAnimatorRig() : igdeMetaPropertyMCT("animator.animator.rig",
		"Animator.WPAnimator.Rig", igdeEnvironment::efpltRig){}
	~aeMCPAnimatorRig() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpRigPath;
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetDirectoryPath();
	}
};


/** Animation path. */
class aeMCPAnimatorAnimation : public aeAnimator::MetaProperty<igdeMetaPropertyPathStorage>{
public:
	aeMCPAnimatorAnimation() : igdeMetaPropertyMCT("animator.animator.animation",
		"Animator.WPAnimator.Animation", igdeEnvironment::efpltAnimation){}
	~aeMCPAnimatorAnimation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpAnimationPath;
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetDirectoryPath();
	}
};


/** Affected bones. */
class aeMCPAnimatorAffectedBones : public aeAnimator::MetaProperty<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPAnimatorAffectedBones() : igdeMetaPropertyMCT("animator.animator.affectedBones",
		"Animator.WPAnimator.AffectedBones")
	{
		SetMultiSelection(true);
		SetRows(5);
	}
	
	~aeMCPAnimatorAffectedBones() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpAffectedBones;
	}
	
	decStringSet GetAllowedStrings(const igdeMetaContext::Ref &context) const override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu,
		const igdeMetaContext::Ref &context, igdeWidget &owner) override;
};


/** Affected vertex position sets. */
class aeMCPAnimatorAffectedVertexPositionSets :
	public aeAnimator::MetaProperty<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPAnimatorAffectedVertexPositionSets() : igdeMetaPropertyMCT(
		"animator.animator.affectedVertexPositionSets",
		"Animator.WPAnimator.AffectedVertexPositionSets")
	{
		SetMultiSelection(true);
		SetRows(5);
	}
	
	~aeMCPAnimatorAffectedVertexPositionSets() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpAffectedVps;
	}
	
	decStringSet GetAllowedStrings(const igdeMetaContext::Ref &context) const override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu,
		const igdeMetaContext::Ref &context, igdeWidget &owner) override;
};


/** Playground controllers. */
class aeMCPAnimatorPlaygroundControllers :
	public aeAnimator::MetaPropertyNoCapture<
		igdeMetaPropertySliderBoardStorage<aeController::MetaContext>>{
public:
	aeMCPAnimatorPlaygroundControllers(igdeMetaPropertyString *propertyName, igdeMetaPropertyFloat *propertyValue);
	~aeMCPAnimatorPlaygroundControllers() override;
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const igdeMetaContext::Ref &context, igdeWidget &owner) override;
};


/** Display model path. */
class aeMCPAnimatorDisplayModelPath : public aeAnimator::MetaProperty<igdeMetaPropertyPathStorage>{
public:
	aeMCPAnimatorDisplayModelPath() : igdeMetaPropertyMCT("animator.animator.displayModelPath",
		"Animator.WPView.Model", igdeEnvironment::efpltModel){}
	~aeMCPAnimatorDisplayModelPath() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpDisplayModelPath;
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetDirectoryPath();
	}
};

/** Display skin path. */
class aeMCPAnimatorDisplaySkinPath : public aeAnimator::MetaProperty<igdeMetaPropertyPathStorage>{
public:
	aeMCPAnimatorDisplaySkinPath() : igdeMetaPropertyMCT("animator.animator.displaySkinPath",
		"Animator.WPView.Skin", igdeEnvironment::efpltSkin){}
	~aeMCPAnimatorDisplaySkinPath() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpDisplaySkinPath;
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetDirectoryPath();
	}
};

/** Display rig path. */
class aeMCPAnimatorDisplayRigPath : public aeAnimator::MetaProperty<igdeMetaPropertyPathStorage>{
public:
	aeMCPAnimatorDisplayRigPath() : igdeMetaPropertyMCT("animator.animator.displayRigPath",
		"Animator.WPView.Rig", igdeEnvironment::efpltRig){}
	~aeMCPAnimatorDisplayRigPath() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpDisplayRigPath;
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetDirectoryPath();
	}
};

/** Base animator path. */
class aeMCPAnimatorBaseAnimatorPath : public aeAnimator::MetaProperty<igdeMetaPropertyPathStorage>{
public:
	aeMCPAnimatorBaseAnimatorPath() : igdeMetaPropertyMCT("animator.animator.baseAnimatorPath",
		"Animator.WPView.Animator", igdeEnvironment::efpltAnimator){}
	~aeMCPAnimatorBaseAnimatorPath() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpBaseAnimatorPath;
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetDirectoryPath();
	}
};

/** Reset state. */
class aeMCPAnimatorResetState : public aeAnimator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPAnimatorResetState() : igdeMetaPropertyMCT("animator.animator.resetState",
		"Animator.WPView.ResetState"){}
	~aeMCPAnimatorResetState() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpResetState;
	}
};

/** Play speed. */
class aeMCPAnimatorPlaySpeed : public aeAnimator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorPlaySpeed() : igdeMetaPropertyMCT("animator.animator.playSpeed",
		"Animator.WPView.PlaySpeed")
	{
		SetDefaultValue(1.0f);
		SetLowerLimit(0.0f);
	}
	~aeMCPAnimatorPlaySpeed() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpPlaySpeed;
	}
};

/** Time step. */
class aeMCPAnimatorTimeStep : public aeAnimator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorTimeStep() : igdeMetaPropertyMCT("animator.animator.timeStep",
		"Animator.WPView.TimeStep")
	{
		SetDefaultValue(0.05f);
		SetLowerLimit(0.0f);
	}
	~aeMCPAnimatorTimeStep() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTimeStep;
	}
};

/** Paused. */
class aeMCPAnimatorPaused : public aeAnimator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPAnimatorPaused() : igdeMetaPropertyMCT("animator.animator.paused",
		"Animator.WPView.Paused"){}
	~aeMCPAnimatorPaused() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpPaused;
	}
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu,
		const igdeMetaContext::Ref &context, igdeWidget &owner) override;
};

/** Sky. */
class aeMCPAnimatorSky : public aeAnimator::MetaProperty<igdeMetaPropertyContextStorage>{
public:
	aeMCPAnimatorSky();
	~aeMCPAnimatorSky() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpSky;
	}
};

/** Environment object. */
class aeMCPAnimatorEnvironmentObject : public aeAnimator::MetaProperty<igdeMetaPropertyContextStorage>{
public:
	aeMCPAnimatorEnvironmentObject();
	~aeMCPAnimatorEnvironmentObject() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnvironmentObject;
	}
};

/** Camera. */
class aeMCPAnimatorCamera : public aeAnimator::MetaProperty<igdeMetaPropertyContextStorage>{
public:
	aeMCPAnimatorCamera();
	~aeMCPAnimatorCamera() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpCamera;
	}
};

/** Camera Attach. */
class aeMCPAnimatorCameraAttachToBone : public aeAnimator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPAnimatorCameraAttachToBone() : igdeMetaPropertyMCT("animator.animator.cameraAttach",
		"Animator.WPView.CamAttach"){}
	~aeMCPAnimatorCameraAttachToBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetCamera()->mpAttachToBone;
	}
};

/** Camera attach bone. */
class aeMCPAnimatorCameraAttachBone : public aeAnimator::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPAnimatorCameraAttachBone() : igdeMetaPropertyMCT("animator.animator.cameraAttachBone",
		"Animator.WPView.CamBone"){
		SetEnableAllowed(true);
	}
	
	~aeMCPAnimatorCameraAttachBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetCamera()->mpBone;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		return Owner(context).mpHiddenBoneNames;
	}
};

/** Camera attach relative position. */
class aeMCPAnimatorCameraAttachRelativePosition :
	public aeAnimator::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPAnimatorCameraAttachRelativePosition() : igdeMetaPropertyMCT(
		"animator.animator.cameraAttachRelativePosition", "Animator.WPView.CamRelPos"){}
	~aeMCPAnimatorCameraAttachRelativePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetCamera()->mpRelativePosition;
	}
};

/** Camera attach relative rotation. */
class aeMCPAnimatorCameraAttachRelativeRotation :
	public aeAnimator::MetaProperty<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPAnimatorCameraAttachRelativeRotation() : igdeMetaPropertyMCT(
		"animator.animator.cameraAttachRelativeRotation", "Animator.WPView.CamRelRot"){}
	~aeMCPAnimatorCameraAttachRelativeRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetCamera()->mpRelativeRotation;
	}
};

#endif
