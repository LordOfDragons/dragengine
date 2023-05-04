/* 
 * Drag[en]gine IGDE Animator Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
