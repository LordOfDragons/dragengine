/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEWPVIEW_H_
#define _MEWPVIEW_H_

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/composed/igdeToggleTagsReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/properties/igdeWPSkyReference.h>
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
	igdeTextFieldReference pEditRectSelDist;
	igdeTextFieldReference pEditRectSelDistStep;
	
	igdeTextFieldReference pEditActiveCamera;
	
	igdeWPSkyReference pWPSky;
	igdeWPCameraReference pWPCamera;
	igdeWPTriggerTableReference pWPTriggerTable;
	igdeComboBoxReference pCBCameraObjects;
	igdeToggleTagsReference pEditClassHideTags;
	igdeToggleTagsReference pEditClassPartialHideTags;
	
	igdeActionReference pActionCameraFreeRoaming;
	igdeActionReference pActionCameraPlayer;
	igdeActionReference pActionCameraObject;
	
	
	
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
