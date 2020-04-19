/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _REWPVIEW_H_
#define _REWPVIEW_H_

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeSliderReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditSliderTextReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/properties/igdeWPSkyReference.h>
#include <deigde/gui/properties/igdeWPWObjectReference.h>

class igdeWPWObject;
class reRig;
class reRigTexture;
class reWindowProperties;
class reWPViewListener;
class igdeGameDefinition;
class igdeWPWObject;



/**
 * \brief View property window.
 */
class reWPView : public igdeContainerScroll{
private:
	reWindowProperties &pWindowProperties;
	
	reWPViewListener *pListener;
	reRig *pRig;
	
	igdeEditPathReference pEditModelPath;
	igdeEditPathReference pEditSkinPath;
	igdeEditPathReference pEditAnimPath;
	igdeComboBoxReference pCBAnimMoves;
	igdeEditSliderTextReference pSldMoveTime;
	igdeCheckBoxReference pChkUseRestPose;
	igdeCheckBoxReference pChkPlaybackMove;
	
	igdeComboBoxReference pCBTexture;
	igdeEditPathReference pEditTexSkin;
	
	igdeEditVectorReference pEditGravity;
	igdeEditVectorReference pEditLocalGravity;
	igdeEditSliderTextReference pSldSlowmotion;
	
	igdeWPSkyReference pWPSky;
	igdeWPWObjectReference pWPEnvObject;
	
	igdeCheckBoxReference pChkSnapToGrid;
	igdeTextFieldReference pEditGridSize;
	igdeTextFieldReference pEditSensitivity;
	
	igdeEditVectorReference pEditCamPosition;
	igdeEditVectorReference pEditCamRotation;
	igdeEditVectorReference pEditCamViewDir;
	igdeTextFieldReference pEditCamFov;
	igdeTextFieldReference pEditCamFovRatio;
	igdeTextFieldReference pEditCamImageDist;
	igdeTextFieldReference pEditCamViewDist;
	igdeTextFieldReference pEditCamExposure;
	igdeTextFieldReference pEditCamLowInt;
	igdeTextFieldReference pEditCamHiInt;
	igdeTextFieldReference pEditCamAdaptTime;
	
	igdeCheckBoxReference pChkCamAttach;
	igdeComboBoxReference pCBCamBone;
	igdeEditVectorReference pEditCamRelPosition;
	igdeEditVectorReference pEditCamRelRotation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	reWPView( reWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~reWPView();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline reWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Rig. */
	inline reRig *GetRig() const{ return pRig; }
	
	/** \brief Set rig. */
	void SetRig( reRig *rig );
	
	/** \brief Update resources. */
	void UpdateResources();
	
	/** \brief Update camera. */
	void UpdateCamera();
	
	/** \brief Update camera view. */
	void UpdateCameraView();
	
	/** \brief Update sky. */
	void UpdateSky();
	
	/** \brief Update environment object. */
	void UpdateEnvObject();
	
	/** \brief Update view. */
	void UpdateView();
	
	/** \brief Update animation moves list. */
	void UpdateMoveList();
	
	/** \brief Update bones list. */
	void UpdateBoneList();
	
	/** \brief Update texture list. */
	void UpdateTextureList();
	
	/** \brief Update texture. */
	void UpdateTexture();
	
	/** \brief Selected texture. */
	reRigTexture *GetSelectedTexture() const;
	/*@}*/
};

#endif
