/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLDEBUGDRAWER_H_
#define _DEOGLDEBUGDRAWER_H_

#include <dragengine/systems/modules/graphic/deBaseGraphicDebugDrawer.h>

class deoglRDebugDrawer;

class deGraphicOpenGl;
class deDebugDrawer;



/**
 * Debug Drawer Peer.
 */
class deoglDebugDrawer : public deBaseGraphicDebugDrawer{
private:
	deGraphicOpenGl &pOgl;
	const deDebugDrawer &pDebugDrawer;
	
	deoglRDebugDrawer *pRDebugDrawer;
	
	bool pDirtyDebugDrawer;
	bool pDirtyShapes;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglDebugDrawer( deGraphicOpenGl &ogl, const deDebugDrawer &debugDrawer );
	
	/** Clean up peer. */
	virtual ~deoglDebugDrawer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl module. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Debug drawer resource. */
	inline const deDebugDrawer &GetDebugDrawer() const{ return pDebugDrawer; }
	
	
	
	/** Render debug drawer. */
	inline deoglRDebugDrawer *GetRDebugDrawer() const{ return pRDebugDrawer; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Position changed. */
	virtual void PositionChanged();
	
	/** Orientation changed. */
	virtual void OrientationChanged();
	
	/** Scaling changed. */
	virtual void ScalingChanged();
	
	
	
	/** Visibile changed. */
	virtual void VisibleChanged();
	
	/** X-Ray changed. */
	virtual void XRayChanged();
	
	
	
	/** Shape edge or fill colors changed. */
	virtual void ShapeColorChanged();
	
	/** Shape position, orientation or scaling changed. */
	virtual void ShapeGeometryChanged();
	
	/** Shape shapes or faces changed. */
	virtual void ShapeContentChanged();
	
	/** Shapes have been added or removed. */
	virtual void ShapeLayoutChanged();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
