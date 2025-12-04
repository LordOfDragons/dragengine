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

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/composed/igdeToggleTagsReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/event/igdeAction::Ref.h>
#include <deigde/gui/properties/igdeWPSkyReference.h>
#include <deigde/gui/properties/igdeWPWObjectReference.h>
#include <deigde/gui/properties/igdeWPCameraReference.h>
#include <deigde/gui/properties/igdeWPTriggerTableReference.h>

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
	meWPViewListener *pListener;
	
	meWorld *pWorld;
	
	igdeTextFieldReference pEditMoveStep;
	igdeCheckBoxReference pChkMoveSnap;
	igdeTextFieldReference pEditRotStep;
	igdeCheckBoxReference pChkRotSnap;
	igdeTextFieldReference pEditScaleStep;
	igdeCheckBoxReference pChkScaleSnap;
	igdeCheckBoxReference pChkAutoUpdate;
	igdeTextFieldReference pEditSensitivity;
	igdeButton::Ref pBtnSensitivity;
	igdeTextFieldReference pEditRectSelDist;
	igdeTextFieldReference pEditRectSelDistStep;
	
	igdeTextFieldReference pEditActiveCamera;
	
	igdeWPSkyReference pWPSky;
	igdeWPWObjectReference pWPBgObject;
	igdeWPCameraReference pWPCamera;
	igdeWPTriggerTableReference pWPTriggerTable;
	igdeComboBoxReference pCBCameraObjects;
	igdeToggleTagsReference pEditClassHideTags;
	igdeToggleTagsReference pEditClassPartialHideTags;
	
	igdeAction::Ref pActionCameraFreeRoaming;
	igdeAction::Ref pActionCameraPlayer;
	igdeAction::Ref pActionCameraObject;
	
	igdeCheckBoxReference pChkEnableAuralization;
	
	igdeEditVectorReference pEditLimitBoxMinExtend, pEditLimitBoxMaxExtend;
	igdeButton::Ref pBtnLimitBoxMenu;
	
	bool pPreventUpdateCamera;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	meWPView( meWindowProperties &windowProperties );
	
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
	inline meWorld *GetWorld() const{ return pWorld; }
	
	/** \brief Set world. */
	void SetWorld( meWorld *world );
	
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
