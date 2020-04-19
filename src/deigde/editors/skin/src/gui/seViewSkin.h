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

#ifndef _SEVIEWSKIN_H_
#define _SEVIEWSKIN_H_

#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/event/igdeMouseKeyListenerReference.h>
#include <deigde/gui/event/igdeMouseCameraListenerReference.h>
#include <deigde/undo/igdeUndoReference.h>


class seWindowMain;
class seSkin;


/**
 * \brief Skin preview.
 */
class seViewSkin : public igdeViewRenderWindow{
private:
	seWindowMain &pWindowMain;
	seSkin *pSkin;
	igdeMouseCameraListenerReference pCameraInteraction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	seViewSkin( seWindowMain &windowMain );
	
protected:
	/** \brief Clean up view. */
	virtual ~seViewSkin();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Reset view. */
	void ResetView();
	
	/** \brief Skin. */
	inline seSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin. */
	void SetSkin( seSkin *skin );
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate( float elapsed );
	
	/** \brief Create canvas. */
	virtual void CreateCanvas();
	/*@}*/
};

#endif
