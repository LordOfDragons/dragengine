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
#include <deigde/gui/gizmo/igdeGizmoManager.h>
#include <deigde/undo/igdeUndoReference.h>


class aeWindowMain;
class aeAnimator;



/**
 * 3D view.
 */
class aeView3D : public igdeViewRenderWindow{
private:
	aeWindowMain &pWindowMain;
	
	aeAnimator *pAnimator;
	
	igdeMouseCameraListenerReference pCameraInteraction;
	igdeMouseKeyListenerReference pLocomotionInteraction;
	igdeMouseKeyListenerReference pWakeboardInteraction;
	igdeMouseKeyListenerReference pEditorInteraction;
	
	igdeGizmoManager pGizmos;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create view. */
	aeView3D( aeWindowMain &windowMain );
	
protected:
	/** Clean up view. */
	virtual ~aeView3D();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Main window. */
	inline aeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** Reset view. */
	void ResetView();
	
	/** Animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	
	/** Set animator. */
	void SetAnimator( aeAnimator *animator );
	
	/** Start locomotion testing. */
	void StartLocomotionTesting();
	
	/** Stop locomotion testing. */
	void StopLocomotionTesting();
	
	/** Start wakeboarding. */
	void StartWakeboarding();
	
	/** Stop wakeboarding. */
	void StopWakeboarding();
	
	/** Game like frame update. */
	virtual void OnFrameUpdate( float elapsed );
	
	/** Create canvas. */
	virtual void CreateCanvas();
	
	/** Gizmos. */
	inline igdeGizmoManager &GetGizoms(){ return pGizmos; }
	/*@}*/
};

#endif
