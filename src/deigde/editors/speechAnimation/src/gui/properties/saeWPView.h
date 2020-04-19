/* 
 * Drag[en]gine IGDE Speech Animation Editor
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

#ifndef _SAEWPVIEW_H_
#define _SAEWPVIEW_H_

#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/properties/igdeWPSkyReference.h>
#include <deigde/gui/properties/igdeWPCameraReference.h>

class saeWindowProperties;
class saeGameDefinition;
class saeWPViewListener;
class saeSAnimation;



/**
 * \brief View property window.
 */
class saeWPView : public igdeContainerScroll{
private:
	saeWindowProperties &pWindowProperties;
	saeWPViewListener *pListener;
	saeSAnimation *pSAnimation;
	
	igdeEditPathReference pEditDisplayModelPath;
	igdeEditPathReference pEditDisplaySkinPath;
	igdeEditPathReference pEditDisplayRigPath;
	
	igdeWPSkyReference pWPSky;
	igdeWPCameraReference pWPCamera;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	saeWPView( saeWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~saeWPView();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline saeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Speech animation. */
	inline saeSAnimation *GetSAnimation() const{ return pSAnimation; }
	
	/** \brief Set speech animation. */
	void SetSAnimation( saeSAnimation *sanimation );
	
	/** \brief Update view. */
	void UpdateView();
	
	/** \brief Update sky. */
	void UpdateSky();
	
	/** \brief Update camera. */
	void UpdateCamera();
	/*@}*/
};

#endif
