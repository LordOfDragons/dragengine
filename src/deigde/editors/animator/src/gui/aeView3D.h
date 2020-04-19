/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AEVIEW3D_H_
#define _AEVIEW3D_H_

#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/event/igdeMouseKeyListenerReference.h>
#include <deigde/gui/event/igdeMouseCameraListenerReference.h>
#include <deigde/undo/igdeUndoReference.h>


class aeWindowMain;
class aeAnimator;



/**
 * \brief 3D view.
 */
class aeView3D : public igdeViewRenderWindow{
private:
	aeWindowMain &pWindowMain;
	
	aeAnimator *pAnimator;
	
	igdeMouseCameraListenerReference pCameraInteraction;
	igdeMouseKeyListenerReference pLocomotionInteraction;
	igdeMouseKeyListenerReference pWakeboardInteraction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	aeView3D( aeWindowMain &windowMain );
	
protected:
	/** \brief Clean up view. */
	virtual ~aeView3D();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline aeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Reset view. */
	void ResetView();
	
	/** \brief Animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	
	/** \brief Set animator. */
	void SetAnimator( aeAnimator *animator );
	
	/** \brief Start locomotion testing. */
	void StartLocomotionTesting();
	
	/** \brief Stop locomotion testing. */
	void StopLocomotionTesting();
	
	/** \brief Start wakeboarding. */
	void StartWakeboarding();
	
	/** \brief Stop wakeboarding. */
	void StopWakeboarding();
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate( float elapsed );
	
	/** \brief Create canvas. */
	virtual void CreateCanvas();
	/*@}*/
};

#endif
