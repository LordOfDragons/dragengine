/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _AEWPVIEW_H_
#define _AEWPVIEW_H_

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/properties/igdeWPWObject.h>
#include <deigde/gui/properties/igdeWPSky.h>
#include <deigde/gui/properties/igdeWPCamera.h>

#include <dragengine/common/math/decMath.h>

class aeWindowProperties;
class aeGameDefinition;
#include "aeWPViewListener.h"
#include "../../animator/aeAnimator.h"
class aeAttachment;



/**
 * View property window.
 */
class aeWPView : public igdeContainerScroll{
public:
	typedef deTObjectReference<aeWPView> Ref;
	
	
private:
	aeWindowProperties &pWindowProperties;
	aeWPViewListener::Ref pListener;
	aeAnimator::Ref pAnimator;
	
	igdeEditPath::Ref pEditDisplayModelPath;
	igdeEditPath::Ref pEditDisplaySkinPath;
	igdeEditPath::Ref pEditDisplayRigPath;
	
	igdeEditPath::Ref pEditTestingAnimatorPath;
	igdeCheckBox::Ref pChkResetState;
	
	igdeWPSky::Ref pWPSky;
	igdeWPWObject::Ref pWPEnvObject;
	igdeWPCamera::Ref pWPCamera;
	
	igdeCheckBox::Ref pChkCamAttach;
	igdeComboBoxFilter::Ref pCBCamBone;
	igdeEditVector::Ref pEditCamRelPos;
	igdeEditVector::Ref pEditCamRelRot;
	
	igdeTextField::Ref pEditPlaySpeed;
	igdeTextField::Ref pEditTimeStep;
	igdeButton::Ref pBtnFramePrev;
	igdeButton::Ref pBtnResetAnimation;
	igdeButton::Ref pBtnFrameNext;
	igdeButton::Ref pBtnPauseAnimation;
	
	igdeComboBox::Ref pCBAttachments;
	igdeButton::Ref pBtnAttMenu;
	igdeTextField::Ref pEditAttName;
	igdeWPWObject::Ref pWPAttachment;
	igdeComboBox::Ref pCBAttAttachType;
	igdeComboBoxFilter::Ref pCBAttBoneName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	aeWPView(aeWindowProperties &windowProperties);
	
protected:
	/** Clean up panel. */
	virtual ~aeWPView();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Properties window. */
	inline aeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** Animator. */
	inline const aeAnimator::Ref &GetAnimator() const{ return pAnimator; }
	
	/** Set animator. */
	void SetAnimator(aeAnimator *animator);
	
	/** Attachment. */
	aeAttachment *GetAttachment() const;
	
	/** Update path. */
	void UpdatePath();
	
	/** Update camera. */
	void UpdateCamera();
	
	/** Update camera view. */
	void UpdateCameraView();
	
	/** Update view. */
	void UpdateView();
	
	/** Update sky. */
	void UpdateSky();
	
	/** Update environment object. */
	void UpdateEnvObject();
	
	/** Update playback. */
	void UpdatePlayback();
	
	/** Update bone list. */
	void UpdateBoneList();
	
	/** Update attachment list. */
	void UpdateAttachmentList();
	
	/** Select active attachment. */
	void SelectActiveAttachment();
	
	/** Update attachment. */
	void UpdateAttachment();
	
	/** Animator path changed. */
	void OnAnimatorPathChanged();
	/*@}*/
};

#endif
