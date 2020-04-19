/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#ifndef _PEEVIEWEMITTER_H_
#define _PEEVIEWEMITTER_H_

#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/event/igdeMouseDragListenerReference.h>
#include <deigde/gui/event/igdeMouseCameraListenerReference.h>

class peeWindowMain;
class peeEmitter;



/**
 * \brief Emitter preview.
 */
class peeViewEmitter : public igdeViewRenderWindow{
private:
	peeWindowMain &pWindowMain;
	
	peeEmitter *pEmitter;
	
	igdeMouseCameraListenerReference pCameraInteraction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create emitter view. */
	peeViewEmitter( peeWindowMain &windowMain );
	
protected:
	/** \brief Clean up emitter view. */
	virtual ~peeViewEmitter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline peeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Reset view. */
	void ResetView();
	
	/** \brief Emitter. */
	inline peeEmitter *GetEmitter() const{ return pEmitter; }
	
	/** \brief Set emitter. */
	void SetEmitter( peeEmitter *emitter );
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate( float elapsed );
	
	/** \brief Create canvas. */
	virtual void CreateCanvas();
	/*@}*/
};

#endif
