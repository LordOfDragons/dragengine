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
 * \brief Debug Drawer Peer.
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
	/** \brief Create peer. */
	deoglDebugDrawer( deGraphicOpenGl &ogl, const deDebugDrawer &debugDrawer );
	
	/** \brief Clean up peer. */
	virtual ~deoglDebugDrawer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Opengl module. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** \brief Debug drawer resource. */
	inline const deDebugDrawer &GetDebugDrawer() const{ return pDebugDrawer; }
	
	
	
	/** \brief Render debug drawer. */
	inline deoglRDebugDrawer *GetRDebugDrawer() const{ return pRDebugDrawer; }
	
	/** \brief Update render thread counterpart if required. */
	void SyncToRender();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Scaling changed. */
	virtual void ScalingChanged();
	
	
	
	/** \brief Visibile changed. */
	virtual void VisibleChanged();
	
	/** \brief X-Ray changed. */
	virtual void XRayChanged();
	
	
	
	/** \brief Shape edge or fill colors changed. */
	virtual void ShapeColorChanged();
	
	/** \brief Shape position, orientation or scaling changed. */
	virtual void ShapeGeometryChanged();
	
	/** \brief Shape shapes or faces changed. */
	virtual void ShapeContentChanged();
	
	/** \brief Shapes have been added or removed. */
	virtual void ShapeLayoutChanged();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
