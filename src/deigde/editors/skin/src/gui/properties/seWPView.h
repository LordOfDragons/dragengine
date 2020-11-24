/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SEWPVIEW_H_
#define _SEWPVIEW_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/properties/igdeWPCameraReference.h>
#include <deigde/gui/properties/igdeWPSkyReference.h>
#include <deigde/gui/properties/igdeWPWObjectReference.h>

class seSkin;
class seWindowProperties;
class seWPViewListener;



/**
 * \brief View Panel.
 */
class seWPView : public igdeContainerScroll{
private:
	seWindowProperties &pWindowProperties;
	seWPViewListener *pListener;
	
	seSkin *pSkin;
	
	igdeComboBoxReference pCBPreviewMode;
	
	igdeEditPathReference pEditModelPath;
	igdeEditPathReference pEditRigPath;
	igdeEditPathReference pEditAnimPath;
	igdeComboBoxFilterReference pCBAnimMoves;
	igdeCheckBoxReference pChkPlayback;
	
	igdeButtonReference pBtnRewindTextures;
	
	igdeWPCameraReference pWPCamera;
	igdeWPSkyReference pWPSky;
	igdeWPWObjectReference pWPEnvObject;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPView( seWindowProperties &windowProperties );
	
protected:
	/** \brief Cleanup panel. */
	virtual ~seWPView();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline seWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Skin. */
	inline seSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin. */
	void SetSkin( seSkin *sky );
	
	/** \brief Update view. */
	void UpdateView();
	
	/** \brief Update camera. */
	void UpdateCamera();
	
	/** \brief Update sky. */
	void UpdateSky();
	
	/** \brief Update environment object. */
	void UpdateEnvObject();
	
	/** \brief Update animation moves list. */
	void UpdateMoveList();
	/*@}*/
};

#endif
