/* 
 * Drag[en]gine IGDE Sky Editor
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

#ifndef _SEWPSKY_H_
#define _SEWPSKY_H_

#include <deigde/gui/igdeColorBoxReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class seSky;
class seWindowProperties;
class seWPSkyListener;


/**
 * \brief Sky panel.
 */
class seWPSky : public igdeContainerScroll{
private:
	seWindowProperties &pWindowProperties;
	seSky *pSky;
	seWPSkyListener *pListener;
	
	igdeColorBoxReference pClrBg;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPSky( seWindowProperties &windowProperties );
	
	/** \brief Clean up panel. */
	virtual ~seWPSky();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Sky. */
	inline seSky *GetSky() const{ return pSky; }
	
	/** \brief Set sky. */
	void SetSky( seSky *sky );
	
	/** \brief Update sky. */
	void UpdateSky();
	/*@}*/
};

#endif
