/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _AEVIEW3D_H_
#define _AEVIEW3D_H_

#include "gizmo/aeGizmoManager.h"

#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/event/igdeMouseKeyListener.h>
#include <deigde/gui/event/igdeMouseCameraListener.h>
#include <deigde/undo/igdeUndo.h>


class aeWindowMain;
class aeAnimator;



/**
 * 3D view.
 */
class aeView3D : public igdeViewRenderWindow{
private:
	aeWindowMain &pWindowMain;
	
	aeAnimator::Ref pAnimator;
	
	igdeMouseCameraListener::Ref pCameraInteraction;
	igdeMouseKeyListener::Ref pLocomotionInteraction;
	igdeMouseKeyListener::Ref pWakeboardInteraction;
	igdeMouseKeyListener::Ref pEditorInteraction;
	
	aeGizmoManager pGizmos;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create view. */
	aeView3D(aeWindowMain &windowMain);
	
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
	inline const aeAnimator::Ref &GetAnimator() const{ return pAnimator; }
	
	/** Set animator. */
	void SetAnimator(aeAnimator *animator);
	
	/** Start locomotion testing. */
	void StartLocomotionTesting();
	
	/** Stop locomotion testing. */
	void StopLocomotionTesting();
	
	/** Start wakeboarding. */
	void StartWakeboarding();
	
	/** Stop wakeboarding. */
	void StopWakeboarding();
	
	/** Game like frame update. */
	virtual void OnFrameUpdate(float elapsed);
	
	/** Create canvas. */
	virtual void CreateCanvas();
	
	/** Gizmos. */
	inline aeGizmoManager &GetGizoms(){ return pGizmos; }
	/*@}*/
};

#endif
