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

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/properties/igdeWPWObjectReference.h>
#include <deigde/gui/properties/igdeWPSkyReference.h>
#include <deigde/gui/properties/igdeWPCameraReference.h>

#include <dragengine/common/math/decMath.h>

class aeWindowProperties;
class aeGameDefinition;
class aeWPViewListener;
class aeAnimator;
class aeAttachment;



/**
 * View property window.
 */
class aeWPView : public igdeContainerScroll{
private:
	aeWindowProperties &pWindowProperties;
	aeWPViewListener *pListener;
	aeAnimator *pAnimator;
	
	igdeEditPathReference pEditDisplayModelPath;
	igdeEditPathReference pEditDisplaySkinPath;
	igdeEditPathReference pEditDisplayRigPath;
	
	igdeEditPathReference pEditTestingAnimatorPath;
	igdeCheckBoxReference pChkResetState;
	
	igdeWPSkyReference pWPSky;
	igdeWPWObjectReference pWPEnvObject;
	igdeWPCameraReference pWPCamera;
	
	igdeCheckBoxReference pChkCamAttach;
	igdeComboBoxFilterReference pCBCamBone;
	igdeEditVectorReference pEditCamRelPos;
	igdeEditVectorReference pEditCamRelRot;
	
	igdeTextFieldReference pEditPlaySpeed;
	igdeTextFieldReference pEditTimeStep;
	igdeButtonReference pBtnFramePrev;
	igdeButtonReference pBtnResetAnimation;
	igdeButtonReference pBtnFrameNext;
	igdeButtonReference pBtnPauseAnimation;
	
	igdeComboBoxReference pCBAttachments;
	igdeButtonReference pBtnAttMenu;
	igdeTextFieldReference pEditAttName;
	igdeWPWObjectReference pWPAttachment;
	igdeComboBoxReference pCBAttAttachType;
	igdeComboBoxFilterReference pCBAttBoneName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	aeWPView( aeWindowProperties &windowProperties );
	
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
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	
	/** Set animator. */
	void SetAnimator( aeAnimator *animator );
	
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
