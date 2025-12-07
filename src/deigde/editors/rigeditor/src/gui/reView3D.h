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

#ifndef _REVIEW3D_H_
#define _REVIEW3D_H_

#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/event/igdeMouseDragListener.h>
#include <deigde/gui/event/igdeMouseCameraListener.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

class reWindowMain;
class reRig;
class reCamera;
class deColliderVolume;
class deBaseScriptingCollider;
class decLayerMask;
class reTemporaryConstraint;



/**
 * \brief 3D view.
 */
class reView3D : public igdeViewRenderWindow{
private:
	reWindowMain &pWindowMain;
	
	reRig::Ref pRig;
	
	igdeMouseCameraListener::Ref pCameraInteraction;
	igdeMouseDragListener::Ref pSimulationInteraction;
	igdeMouseDragListener::Ref pSelectInteraction;
	igdeMouseDragListener::Ref pMoveInteraction;
	igdeMouseDragListener::Ref pScaleInteraction;
	igdeMouseDragListener::Ref pRotateInteraction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create 3d view. */
	reView3D(reWindowMain &windowMain);
	
protected:
	/** \brief Clean up 3d view. */
	virtual ~reView3D();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline reWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Reset view. */
	void ResetView();
	
	/** \brief Rig. */
	inline const reRig::Ref &GetRig() const{ return pRig; }
	
	/** \brief Set rig. */
	void SetRig(reRig *rig);
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate(float elapsed);
	
	/** \brief Create canvas. */
	virtual void CreateCanvas();
	/*@}*/
};

#endif
