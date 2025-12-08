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

#ifndef _MEWPVIEW_H_
#define _MEWPVIEW_H_

#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/composed/igdeToggleTags.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/properties/igdeWPSky.h>
#include <deigde/gui/properties/igdeWPWObject.h>
#include <deigde/gui/properties/igdeWPCamera.h>
#include <deigde/gui/properties/igdeWPTriggerTable.h>

class meWindowProperties;
class meWPViewListener;
class meWorld;
class meCamera;



/**
 * \brief View property window.
 */
class meWPView : public igdeContainerScroll{
private:
	meWindowProperties &pWindowProperties;
	meWPViewListener::Ref pListener;
	
	meWorld::Ref pWorld;
	
	igdeTextField::Ref pEditMoveStep;
	igdeCheckBox::Ref pChkMoveSnap;
	igdeTextField::Ref pEditRotStep;
	igdeCheckBox::Ref pChkRotSnap;
	igdeTextField::Ref pEditScaleStep;
	igdeCheckBox::Ref pChkScaleSnap;
	igdeCheckBox::Ref pChkAutoUpdate;
	igdeTextField::Ref pEditSensitivity;
	igdeButton::Ref pBtnSensitivity;
	igdeTextField::Ref pEditRectSelDist;
	igdeTextField::Ref pEditRectSelDistStep;
	
	igdeTextField::Ref pEditActiveCamera;
	
	igdeWPSky::Ref pWPSky;
	igdeWPWObject::Ref pWPBgObject;
	igdeWPCamera::Ref pWPCamera;
	igdeWPTriggerTable::Ref pWPTriggerTable;
	igdeComboBox::Ref pCBCameraObjects;
	igdeToggleTags::Ref pEditClassHideTags;
	igdeToggleTags::Ref pEditClassPartialHideTags;
	
	igdeAction::Ref pActionCameraFreeRoaming;
	igdeAction::Ref pActionCameraPlayer;
	igdeAction::Ref pActionCameraObject;
	
	igdeCheckBox::Ref pChkEnableAuralization;
	
	igdeEditVector *pEditLimitBoxMinExtend, pEditLimitBoxMaxExtend;
	igdeButton::Ref pBtnLimitBoxMenu;
	
	bool pPreventUpdateCamera;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	meWPView(meWindowProperties &windowProperties);
	
protected:
	/** \brief Clean up view. */
	virtual ~meWPView();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline meWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief World. */
	inline const meWorld::Ref &GetWorld() const{ return pWorld; }
	
	/** \brief Set world. */
	void SetWorld(meWorld *world);
	
	/** \brief Selected camera object. */
	meCamera *GetSelectedCameraObject() const;
	
	/** \brief Update view. */
	void UpdateView();
	
	/** \brief Update camera list. */
	void UpdateCameraList();
	
	/** \brief Update sky. */
	void UpdateSky();
	
	/** Update background object. */
	void UpdateBgObject();
	
	/** Update limit box. */
	void UpdateLimitBox();
	
	/** \brief Update active camera. */
	void UpdateCamera();
	
	/** \brief Update class (partial) hide tags list. */
	void UpdateClassHideTagLists();
	
	/** \brief Update trigger table. */
	void UpdateTriggerTable();
	
	/** \brief Game project game definition changed. */
	void OnGameDefinitionChanged();
	
	/*@}*/
};

#endif
