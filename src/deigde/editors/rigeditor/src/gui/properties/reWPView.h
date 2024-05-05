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

#ifndef _REWPVIEW_H_
#define _REWPVIEW_H_

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeSliderReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditSliderTextReference.h>
#include <deigde/gui/composed/igdeEditDVectorReference.h>
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
	
	igdeEditDVectorReference pEditCamPosition;
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
