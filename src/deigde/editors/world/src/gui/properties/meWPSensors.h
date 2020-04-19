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

#ifndef _MEWPSENSORS_H_
#define _MEWPSENSORS_H_

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeColorBoxReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>


class meWindowProperties;



/**
 * \brief Sensors property panel.
 */
class meWPSensors : public igdeContainerScroll{
private:
	meWindowProperties &pWindowProperties;
	
	igdeCheckBoxReference pChkLMTrackCam;
	igdeEditVectorReference pEditLMPos;
	igdeEditVectorReference pEditLMDir;
	igdeTextFieldReference pEditLMConeIA;
	igdeTextFieldReference pEditLMConeOA;
	igdeTextFieldReference pEditLMConeExp;
	igdeTextFieldReference pEditLMLumi;
	igdeColorBoxReference pEditLMColor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	meWPSensors( meWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~meWPSensors();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline meWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Update sensors. */
	void UpdateSensors();
	
	/** \brief Update lumimeters. */
	void UpdateLumimeter();
	/*@}*/
};

#endif
