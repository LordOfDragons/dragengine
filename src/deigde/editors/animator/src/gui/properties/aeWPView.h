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
 * \brief View property window.
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
	/** \brief Create panel. */
	aeWPView( aeWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~aeWPView();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline aeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	
	/** \brief Set animator. */
	void SetAnimator( aeAnimator *animator );
	
	/** \brief Attachment. */
	aeAttachment *GetAttachment() const;
	
	/** \brief Update path. */
	void UpdatePath();
	
	/** \brief Update camera. */
	void UpdateCamera();
	
	/** \brief Update camera view. */
	void UpdateCameraView();
	
	/** \brief Update view. */
	void UpdateView();
	
	/** \brief Update sky. */
	void UpdateSky();
	
	/** \brief Update environment object. */
	void UpdateEnvObject();
	
	/** \brief Update playback. */
	void UpdatePlayback();
	
	/** \brief Update bone list. */
	void UpdateBoneList();
	
	/** \brief Update attachment list. */
	void UpdateAttachmentList();
	
	/** \brief Select active attachment. */
	void SelectActiveAttachment();
	
	/** \brief Update attachment. */
	void UpdateAttachment();
	/*@}*/
};

#endif
