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

#ifndef _SEVIEWSKY_H_
#define _SEVIEWSKY_H_

#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/event/igdeMouseCameraListenerReference.h>

class seWindowMain;
class seSky;



/**
 * \brief View of the sky.
 */
class seViewSky : public igdeViewRenderWindow{
private:
	seWindowMain &pWindowMain;
	
	seSky *pSky;
	
	igdeMouseCameraListenerReference pCameraInteraction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new sky view. */
	seViewSky( seWindowMain &windowMain );
	
protected:
	/** \brief Clean up the sky view. */
	virtual ~seViewSky();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline seWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brie Reset view. */
	void ResetView();
	
	/** \brief Sky. */
	inline seSky *GetSky() const{ return pSky; }
	
	/** \brief Set sky. */
	void SetSky( seSky *sky );
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate( float elapsed );
	
	/** \brief Create canvas. */
	virtual void CreateCanvas();
	/*@}*/
};

#endif
