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

#include "reWPViewListener.h"
#include "../../rig/reRig.h"

#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeSlider.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/composed/igdeEditDVector.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/properties/igdeWPSky.h>
#include <deigde/gui/properties/igdeWPWObject.h>

class igdeWPWObject;
class reRigTexture;
class reWindowProperties;
class igdeGameDefinition;
class igdeWPWObject;



/**
 * \brief View property window.
 */
class reWPView : public igdeContainerScroll{
public:
	typedef deTObjectReference<reWPView> Ref;
	
private:
	reWindowProperties &pWindowProperties;
	
	reWPViewListener::Ref pListener;
	reRig::Ref pRig;
	
	igdeEditPath::Ref pEditModelPath;
	igdeEditPath::Ref pEditSkinPath;
	igdeEditPath::Ref pEditAnimPath;
	igdeComboBox::Ref pCBAnimMoves;
	igdeEditSliderText::Ref pSldMoveTime;
	igdeCheckBox::Ref pChkUseRestPose;
	igdeCheckBox::Ref pChkPlaybackMove;
	
	igdeComboBox::Ref pCBTexture;
	igdeEditPath::Ref pEditTexSkin;
	
	igdeEditVector::Ref pEditGravity;
	igdeEditVector::Ref pEditLocalGravity;
	igdeEditSliderText::Ref pSldSlowmotion;
	
	igdeWPSky::Ref pWPSky;
	igdeWPWObject::Ref pWPEnvObject;
	
	igdeCheckBox::Ref pChkSnapToGrid;
	igdeTextField::Ref pEditGridSize;
	igdeTextField::Ref pEditSensitivity;
	
	igdeEditDVector::Ref pEditCamPosition;
	igdeEditVector::Ref pEditCamRotation;
	igdeEditVector::Ref pEditCamViewDir;
	igdeTextField::Ref pEditCamFov;
	igdeTextField::Ref pEditCamFovRatio;
	igdeTextField::Ref pEditCamImageDist;
	igdeTextField::Ref pEditCamViewDist;
	igdeTextField::Ref pEditCamExposure;
	igdeTextField::Ref pEditCamLowInt;
	igdeTextField::Ref pEditCamHiInt;
	igdeTextField::Ref pEditCamAdaptTime;
	
	igdeCheckBox::Ref pChkCamAttach;
	igdeComboBox::Ref pCBCamBone;
	igdeEditVector::Ref pEditCamRelPosition;
	igdeEditVector::Ref pEditCamRelRotation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	reWPView(reWindowProperties &windowProperties);
	
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
	inline const reRig::Ref &GetRig() const{ return pRig; }
	
	/** \brief Set rig. */
	void SetRig(reRig *rig);
	
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
