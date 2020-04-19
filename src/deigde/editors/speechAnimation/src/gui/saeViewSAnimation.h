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

#ifndef _SAEVIEWSANIMATION_H_
#define _SAEVIEWSANIMATION_H_

#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/event/igdeMouseCameraListenerReference.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/font/deFontReference.h>

class saeWindowMain;
class saeSAnimation;



/**
 * \brief Speech Animation View.
 */
class saeViewSAnimation : public igdeViewRenderWindow{
private:
	saeWindowMain &pWindowMain;
	deFontReference pFontStats;
	
	saeSAnimation *pSAnimation;
	
	igdeMouseCameraListenerReference pCameraInteraction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create speech animation view. */
	saeViewSAnimation( saeWindowMain &windowMain );
	
protected:
	/** \brief Clean up speech animation view. */
	virtual ~saeViewSAnimation();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Reset view. */
	void ResetView();
	
	/** \brief Speech animation. */
	inline saeSAnimation *GetSAnimation() const{ return pSAnimation; }
	
	/** \brief Set speech animation. */
	void SetSAnimation( saeSAnimation *sanimation );
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate( float elapsed );
	
	/** \brief Create canvas. */
	virtual void CreateCanvas();
	/*@}*/
};

#endif
