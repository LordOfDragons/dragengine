/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEWPSOCCAMERA_H_
#define _GDEWPSOCCAMERA_H_

#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class gdeGameDefinition;
class gdeObjectClass;
class gdeOCCamera;
class gdeWindowProperties;
class gdeWPSOCCameraListener;



/**
 * \brief Object class camera property panel.
 */
class gdeWPSOCCamera : public igdeContainerScroll{
private:
	gdeWindowProperties &pWindowProperties;
	gdeWPSOCCameraListener *pListener;
	
	gdeGameDefinition *pGameDefinition;
	
	igdeTextFieldReference pEditFov;
	igdeTextFieldReference pEditFovRatio;
	igdeTextFieldReference pEditImageDistance;
	igdeTextFieldReference pEditViewDistance;
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditRotation;
	igdeComboBoxReference pCBPropertyName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSOCCamera( gdeWindowProperties &windowMain );
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPSOCCamera();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Game definition or \em NULL if not set. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or \em NULL if not set. */
	void SetGameDefinition( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Active object class or \em NULL if absent. */
	gdeObjectClass *GetObjectClass() const;
	
	/** \brief Active object class camera or \em NULL if not set. */
	gdeOCCamera *GetCamera() const;
	
	
	
	/** \brief Update object class property list. */
	void UpdatePropertyList();
	
	/** \brief Update camera. */
	void UpdateCamera();
	/*@}*/
};

#endif
