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
	aeMCPHiddenBoneNames() : igdeMetaPropertyMCT("animator.hiddenBoneNames", "", ""){}
	~aeMCPHiddenBoneNames() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPHiddenBoneNames();
	}
};

class aeMCPHiddenVPSNames : public aeAnimator::MetaProperty<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPHiddenVPSNames() : igdeMetaPropertyMCT("animator.hiddenVPSNames", "", ""){}
	~aeMCPHiddenVPSNames() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPHiddenVPSNames();
	}
};

class aeMCPHiddenMoveNames : public aeAnimator::MetaProperty<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPHiddenMoveNames() : igdeMetaPropertyMCT("animator.hiddenMoveNames", "", ""){}
	~aeMCPHiddenMoveNames() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPHiddenMoveNames();
	}
};


/** Rig path. */
class aeMCPAnimatorRig : public aeAnimator::MetaProperty<igdeMetaPropertyPathStorage>{
public:
	aeMCPAnimatorRig() : igdeMetaPropertyMCT("animator.rig", "Animator.WPAnimator.Rig", igdeEnvironment::efpltRig){}
	~aeMCPAnimatorRig() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPRigPath();
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetDirectoryPath();
	}
};


/** Animation path. */
class aeMCPAnimatorAnimation : public aeAnimator::MetaProperty<igdeMetaPropertyPathStorage>{
public:
	aeMCPAnimatorAnimation() : igdeMetaPropertyMCT("animator.animation", "Animator.WPAnimator.Animation", igdeEnvironment::efpltAnimation){}
	~aeMCPAnimatorAnimation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPAnimationPath();
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetDirectoryPath();
	}
};


/** Affected bones. */
class aeMCPAnimatorAffectedBones : public aeAnimator::MetaProperty<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPAnimatorAffectedBones() : igdeMetaPropertyMCT("animator.affectedBones", "Animator.WPAnimator.AffectedBones"){
		SetMultiSelection(true);
		SetRows(5);
	}
	
	~aeMCPAnimatorAffectedBones() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPAffectedBones();
	}
	
	decStringSet GetAllowedStrings(const igdeMetaContext::Ref &context) const override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const igdeMetaContext::Ref &context, igdeWidget &owner) override;
};


/** Affected vertex position sets. */
class aeMCPAnimatorAffectedVertexPositionSets : public aeAnimator::MetaProperty<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPAnimatorAffectedVertexPositionSets() : igdeMetaPropertyMCT("animator.affectedVertexPositionSets", "Animator.WPAnimator.AffectedVertexPositionSets"){
		SetMultiSelection(true);
		SetRows(5);
	}
	
	~aeMCPAnimatorAffectedVertexPositionSets() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPAffectedVertexPositionSets();
	}
	
	decStringSet GetAllowedStrings(const igdeMetaContext::Ref &context) const override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const igdeMetaContext::Ref &context, igdeWidget &owner) override;
};


/** Playground controllers. */
class aeMCPAnimatorPlaygroundControllers : public aeAnimator::MetaPropertyNoCapture<igdeMetaPropertySliderBoardStorage<aeController::MetaContext>>{
public:
	aeMCPAnimatorPlaygroundControllers(igdeMetaPropertyString *propertyName, igdeMetaPropertyFloat *propertyValue);
	~aeMCPAnimatorPlaygroundControllers() override;
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const igdeMetaContext::Ref &context, igdeWidget &owner) override;
};


/** Display model path. */
class aeMCPAnimatorDisplayModelPath : public aeAnimator::MetaProperty<igdeMetaPropertyPathStorage>{
public:
	aeMCPAnimatorDisplayModelPath() : igdeMetaPropertyMCT("animator.displayModelPath", "Animator.WPView.Model", igdeEnvironment::efpltModel){}
	~aeMCPAnimatorDisplayModelPath() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPDisplayModelPath();
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetDirectoryPath();
	}
};

/** Display skin path. */
class aeMCPAnimatorDisplaySkinPath : public aeAnimator::MetaProperty<igdeMetaPropertyPathStorage>{
public:
	aeMCPAnimatorDisplaySkinPath() : igdeMetaPropertyMCT("animator.displaySkinPath", "Animator.WPView.Skin", igdeEnvironment::efpltSkin){}
	~aeMCPAnimatorDisplaySkinPath() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPDisplaySkinPath();
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetDirectoryPath();
	}
};

/** Display rig path. */
class aeMCPAnimatorDisplayRigPath : public aeAnimator::MetaProperty<igdeMetaPropertyPathStorage>{
public:
	aeMCPAnimatorDisplayRigPath() : igdeMetaPropertyMCT("animator.displayRigPath", "Animator.WPView.Rig", igdeEnvironment::efpltRig){}
	~aeMCPAnimatorDisplayRigPath() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPDisplayRigPath();
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetDirectoryPath();
	}
};

/** Base animator path. */
class aeMCPAnimatorBaseAnimatorPath : public aeAnimator::MetaProperty<igdeMetaPropertyPathStorage>{
public:
	aeMCPAnimatorBaseAnimatorPath() : igdeMetaPropertyMCT("animator.baseAnimatorPath", "Animator.WPView.Animator", igdeEnvironment::efpltAnimator){}
	~aeMCPAnimatorBaseAnimatorPath() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPBaseAnimatorPath();
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetDirectoryPath();
	}
};

/** Reset state. */
class aeMCPAnimatorResetState : public aeAnimator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPAnimatorResetState() : igdeMetaPropertyMCT("animator.resetState", "Animator.WPView.ResetState"){}
	~aeMCPAnimatorResetState() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPResetState();
	}
};

/** Play speed. */
class aeMCPAnimatorPlaySpeed : public aeAnimator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorPlaySpeed() : igdeMetaPropertyMCT("animator.playSpeed", "Animator.WPView.PlaySpeed"){
		SetDefaultValue(1.0f);
		SetLowerLimit(0.0f);
	}
	~aeMCPAnimatorPlaySpeed() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPPlaySpeed();
	}
};

/** Time step. */
class aeMCPAnimatorTimeStep : public aeAnimator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorTimeStep() : igdeMetaPropertyMCT("animator.timeStep", "Animator.WPView.TimeStep"){
		SetDefaultValue(0.05f);
		SetLowerLimit(0.0f);
	}
	~aeMCPAnimatorTimeStep() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTimeStep();
	}
};

/** Paused. */
class aeMCPAnimatorPaused : public aeAnimator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPAnimatorPaused() : igdeMetaPropertyMCT("animator.paused", "Animator.WPView.Paused"){}
	~aeMCPAnimatorPaused() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPPaused();
	}
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const igdeMetaContext::Ref &context, igdeWidget &owner) override;
};

/** Sky. */
class aeMCPAnimatorSky : public aeAnimator::MetaProperty<igdeMetaPropertyContextStorage>{
public:
	aeMCPAnimatorSky();
	~aeMCPAnimatorSky() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPSky();
	}
};

/** Environment object. */
class aeMCPAnimatorEnvironmentObject : public aeAnimator::MetaProperty<igdeMetaPropertyContextStorage>{
public:
	aeMCPAnimatorEnvironmentObject();
	~aeMCPAnimatorEnvironmentObject() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnvironmentObject();
	}
};

/** Camera. */
class aeMCPAnimatorCamera : public aeAnimator::MetaProperty<igdeMetaPropertyContextStorage>{
public:
	aeMCPAnimatorCamera();
	~aeMCPAnimatorCamera() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPCamera();
	}
};

/** Camera Attach. */
class aeMCPAnimatorCameraAttachToBone : public aeAnimator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPAnimatorCameraAttachToBone() : igdeMetaPropertyMCT("animator.cameraAttach", "Animator.WPView.CamAttach"){}
	~aeMCPAnimatorCameraAttachToBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetCamera()->GetMPAttachToBone();
	}
};

/** Camera attach bone. */
class aeMCPAnimatorCameraAttachBone : public aeAnimator::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPAnimatorCameraAttachBone() : igdeMetaPropertyMCT("animator.cameraAttachBone", "Animator.WPView.CamBone"){
		SetEnableAllowed(true);
	}
	
	~aeMCPAnimatorCameraAttachBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetCamera()->GetMPBone();
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		return Owner(context).GetMPHiddenBoneNames();
	}
};

/** Camera attach relative position. */
class aeMCPAnimatorCameraAttachRelativePosition : public aeAnimator::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPAnimatorCameraAttachRelativePosition() : igdeMetaPropertyMCT("animator.cameraAttachRelativePosition", "Animator.WPView.CamRelPos"){}
	~aeMCPAnimatorCameraAttachRelativePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetCamera()->GetMPRelativePosition();
	}
};

/** Camera attach relative rotation. */
class aeMCPAnimatorCameraAttachRelativeRotation : public aeAnimator::MetaProperty<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPAnimatorCameraAttachRelativeRotation() : igdeMetaPropertyMCT("animator.cameraAttachRelativeRotation", "Animator.WPView.CamRelRot"){}
	~aeMCPAnimatorCameraAttachRelativeRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetCamera()->GetMPRelativeRotation();
	}
};

#endif
