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

#include "aeTMCPAnimator.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/aeSubAnimator.h"

#include <deigde/meta/igdeMetaContextItemInfo.h>
#include <deigde/meta/property/igdeMetaPropertyContext.h>
#include <deigde/meta/property/igdeMetaPropertyPath.h>
#include <deigde/meta/property/igdeMetaPropertyStringSet.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyGroup.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>


/** Hidden. */
class aeMCPHiddenBoneNames : public aeTMCPAnimator<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPHiddenBoneNames() : aeTMCPAnimator("animator.hiddenBoneNames", "", ""){}
	~aeMCPHiddenBoneNames() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).hiddenBoneNames;
	}
};

class aeMCPHiddenVPSNames : public aeTMCPAnimator<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPHiddenVPSNames() : aeTMCPAnimator("animator.hiddenVPSNames", "", ""){}
	~aeMCPHiddenVPSNames() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).hiddenVPSNames;
	}
};

class aeMCPHiddenMoveNames : public aeTMCPAnimator<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPHiddenMoveNames() : aeTMCPAnimator("animator.hiddenMoveNames", "", ""){}
	~aeMCPHiddenMoveNames() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).hiddenMoveNames;
	}
};


/** Rig path. */
class aeMCPAnimatorRig : public aeTMCPAnimator<igdeMetaPropertyPathStorage>{
public:
	aeMCPAnimatorRig() : aeTMCPAnimator("animator.rig", "Animator.WPAnimator.Rig", igdeEnvironment::efpltRig){}
	~aeMCPAnimatorRig() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).rigPath;
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetDirectoryPath();
	}
};


/** Animation path. */
class aeMCPAnimatorAnimation : public aeTMCPAnimator<igdeMetaPropertyPathStorage>{
public:
	aeMCPAnimatorAnimation() : aeTMCPAnimator("animator.animation", "Animator.WPAnimator.Animation", igdeEnvironment::efpltAnimation){}
	~aeMCPAnimatorAnimation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).animationPath;
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetDirectoryPath();
	}
};


/** Affected bones. */
class aeMCPAnimatorAffectedBones : public aeTMCPAnimator<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPAnimatorAffectedBones() : aeTMCPAnimator("animator.affectedBones", "Animator.WPAnimator.AffectedBones"){
		SetMultiSelection(true);
		SetRows(5);
	}
	
	~aeMCPAnimatorAffectedBones() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).affectedBones;
	}
	
	decStringSet GetAllowedStrings(const igdeMetaContext::Ref &context) const override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const igdeMetaContext::Ref &context, igdeWidget &owner) override;
};


/** Affected vertex position sets. */
class aeMCPAnimatorAffectedVertexPositionSets : public aeTMCPAnimator<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPAnimatorAffectedVertexPositionSets() : aeTMCPAnimator("animator.affectedVertexPositionSets", "Animator.WPAnimator.AffectedVertexPositionSets"){
		SetMultiSelection(true);
		SetRows(5);
	}
	
	~aeMCPAnimatorAffectedVertexPositionSets() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).affectedVertexPositionSets;
	}
	
	decStringSet GetAllowedStrings(const igdeMetaContext::Ref &context) const override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const igdeMetaContext::Ref &context, igdeWidget &owner) override;
};


/** Display model path. */
class aeMCPAnimatorDisplayModelPath : public aeTMCPAnimator<igdeMetaPropertyPathStorage>{
public:
	aeMCPAnimatorDisplayModelPath() : aeTMCPAnimator("animator.displayModelPath", "Animator.WPView.Model", igdeEnvironment::efpltModel){}
	~aeMCPAnimatorDisplayModelPath() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).displayModelPath;
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetDirectoryPath();
	}
};

/** Display skin path. */
class aeMCPAnimatorDisplaySkinPath : public aeTMCPAnimator<igdeMetaPropertyPathStorage>{
public:
	aeMCPAnimatorDisplaySkinPath() : aeTMCPAnimator("animator.displaySkinPath", "Animator.WPView.Skin", igdeEnvironment::efpltSkin){}
	~aeMCPAnimatorDisplaySkinPath() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).displaySkinPath;
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetDirectoryPath();
	}
};

/** Display rig path. */
class aeMCPAnimatorDisplayRigPath : public aeTMCPAnimator<igdeMetaPropertyPathStorage>{
public:
	aeMCPAnimatorDisplayRigPath() : aeTMCPAnimator("animator.displayRigPath", "Animator.WPView.Rig", igdeEnvironment::efpltRig){}
	~aeMCPAnimatorDisplayRigPath() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).displayRigPath;
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetDirectoryPath();
	}
};

/** Base animator path. */
class aeMCPAnimatorBaseAnimatorPath : public aeTMCPAnimator<igdeMetaPropertyPathStorage>{
public:
	aeMCPAnimatorBaseAnimatorPath() : aeTMCPAnimator("animator.baseAnimatorPath", "Animator.WPView.Animator", igdeEnvironment::efpltAnimator){}
	~aeMCPAnimatorBaseAnimatorPath() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).baseAnimatorPath;
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetDirectoryPath();
	}
};

/** Reset state. */
class aeMCPAnimatorResetState : public aeTMCPAnimator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPAnimatorResetState() : aeTMCPAnimator("animator.resetState", "Animator.WPView.ResetState"){}
	~aeMCPAnimatorResetState() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).resetState;
	}
};

/** Play speed. */
class aeMCPAnimatorPlaySpeed : public aeTMCPAnimator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorPlaySpeed() : aeTMCPAnimator("animator.playSpeed", "Animator.WPView.PlaySpeed"){
		SetDefaultValue(1.0f);
		SetLowerLimit(0.0f);
	}
	~aeMCPAnimatorPlaySpeed() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).playSpeed;
	}
};

/** Time step. */
class aeMCPAnimatorTimeStep : public aeTMCPAnimator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorTimeStep() : aeTMCPAnimator("animator.timeStep", "Animator.WPView.TimeStep"){
		SetDefaultValue(0.05f);
		SetLowerLimit(0.0f);
	}
	~aeMCPAnimatorTimeStep() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).timeStep;
	}
};

/** Paused. */
class aeMCPAnimatorPaused : public aeTMCPAnimator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPAnimatorPaused() : aeTMCPAnimator("animator.paused", "Animator.WPView.Paused"){}
	~aeMCPAnimatorPaused() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).paused;
	}
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const igdeMetaContext::Ref &context, igdeWidget &owner) override;
};

/** Sky. */
class aeMCPAnimatorSky : public aeTMCPAnimator<igdeMetaPropertyContextStorage>{
public:
	aeMCPAnimatorSky();
	~aeMCPAnimatorSky() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).sky;
	}
};

/** Environment object. */
class aeMCPAnimatorEnvironmentObject : public aeTMCPAnimator<igdeMetaPropertyContextStorage>{
public:
	aeMCPAnimatorEnvironmentObject();
	~aeMCPAnimatorEnvironmentObject() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).environmentObject;
	}
};

/** Camera. */
class aeMCPAnimatorCamera : public aeTMCPAnimator<igdeMetaPropertyContextStorage>{
public:
	aeMCPAnimatorCamera();
	~aeMCPAnimatorCamera() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).camera;
	}
};

/** Camera Attach. */
class aeMCPAnimatorCameraAttachToBone : public aeTMCPAnimator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPAnimatorCameraAttachToBone() : aeTMCPAnimator("animator.cameraAttach", "Animator.WPView.CamAttach"){}
	~aeMCPAnimatorCameraAttachToBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetCamera()->attachToBone;
	}
};

/** Camera attach bone. */
class aeMCPAnimatorCameraAttachBone : public aeTMCPAnimator<igdeMetaPropertyStringStorage>{
public:
	aeMCPAnimatorCameraAttachBone() : aeTMCPAnimator("animator.cameraAttachBone", "Animator.WPView.CamBone"){
		SetEnableAllowed(true);
	}
	
	~aeMCPAnimatorCameraAttachBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetCamera()->bone;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		return Animator(context).hiddenBoneNames;
	}
};

/** Camera attach relative position. */
class aeMCPAnimatorCameraAttachRelativePosition : public aeTMCPAnimator<igdeMetaPropertyVectorStorage>{
public:
	aeMCPAnimatorCameraAttachRelativePosition() : aeTMCPAnimator("animator.cameraAttachRelativePosition", "Animator.WPView.CamRelPos"){}
	~aeMCPAnimatorCameraAttachRelativePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetCamera()->relativePosition;
	}
};

/** Camera attach relative rotation. */
class aeMCPAnimatorCameraAttachRelativeRotation : public aeTMCPAnimator<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPAnimatorCameraAttachRelativeRotation() : aeTMCPAnimator("animator.cameraAttachRelativeRotation", "Animator.WPView.CamRelRot"){}
	~aeMCPAnimatorCameraAttachRelativeRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetCamera()->relativeRotation;
	}
};

#endif
