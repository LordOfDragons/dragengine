/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _REVIEW3D_H_
#define _REVIEW3D_H_

#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/event/igdeMouseDragListenerReference.h>
#include <deigde/gui/event/igdeMouseCameraListenerReference.h>
#include <deigde/undo/igdeUndoReference.h>

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
	
	reRig *pRig;
	
	igdeMouseCameraListenerReference pCameraInteraction;
	igdeMouseDragListenerReference pSimulationInteraction;
	igdeMouseDragListenerReference pSelectInteraction;
	igdeMouseDragListenerReference pMoveInteraction;
	igdeMouseDragListenerReference pScaleInteraction;
	igdeMouseDragListenerReference pRotateInteraction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create 3d view. */
	reView3D( reWindowMain &windowMain );
	
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
	inline reRig *GetRig() const{ return pRig; }
	
	/** \brief Set rig. */
	void SetRig( reRig *rig );
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate( float elapsed );
	
	/** \brief Create canvas. */
	virtual void CreateCanvas();
	/*@}*/
};

#endif
