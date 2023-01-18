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

#ifndef _DEOGLRDEBUGDRAWER_H_
#define _DEOGLRDEBUGDRAWER_H_

#include "../deoglBasics.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>

class deoglRWorld;
class deoglDebugDrawerShape;
class deoglVBOLayout;
class deoglVAO;
class deoglRenderThread;

class deDebugDrawer;



/**
 * Debug Drawer Peer.
 */
class deoglRDebugDrawer : public deObject{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRWorld *pParentWorld;
	
	decDMatrix pMatrix;
	
	bool pVisible;
	bool pXRay;
	
	bool pHasShapes;
	bool pHasFaces;
	
	deoglDebugDrawerShape *pShapes;
	int pShapeCount;
	
	oglVector3 *pVBOData;
	int pVBOPointCount;
	
	GLuint pVBO;
	deoglVAO *pVAO;
	
	bool pDirtyVBO;
	
	bool pWorldMarkedRemove;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render debug drawer. */
	deoglRDebugDrawer( deoglRenderThread &renderThread );
	
	/** Cleans up render debug drawer. */
	virtual ~deoglRDebugDrawer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** Parent world or \em NULL if not part of a world. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** Set parent world. */
	void SetParentWorld( deoglRWorld *parentWorld );
	
	
	
	/** Matrix. */
	inline const decDMatrix &GetMatrix() const{ return pMatrix; }
	
	/** Set matrix. */
	void SetMatrix( const decDMatrix &matrix );
	
	/** Debug drawer is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** Set if debug drawer is visible. */
	void SetVisible( bool visible );
	
	/** Debug drawer is rendered in x-ray mode. */
	inline bool GetXRay() const{ return pXRay; }
	
	/** Set debug drawer is rendered in x-ray mode. */
	void SetXRay( bool xray );
	
	
	
	/** Update shapes. */
	void UpdateShapes( const deDebugDrawer &debugDrawer );
	
	/** Determines if the debug drawer has shapes. */
	inline bool GetHasShapes() const{ return pHasShapes; }
	
	/** Determines if the debug drawer has faces. */
	inline bool GetHasFaces() const{ return pHasFaces; }
	
	/** Number of shapes. */
	inline int GetShapeCount() const{ return pShapeCount; }
	
	/** Retrieves a shape. */
	deoglDebugDrawerShape &GetShapeAt( int index ) const;
	
	
	
	/** Update the VBO if required and contains faces. */
	void UpdateVBO();
	
	/** Retrieves the vao or NULL if there are no shapes with faces. */
	inline deoglVAO *GetVAO() const{ return pVAO; }
	
	
	
	/** Prepare for quick disposal of debug drawer. */
	void PrepareQuickDispose();
	/*@}*/
	
	
	
	/** \name Render world usage */
	/*@{*/
	/**
	 * Marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	inline bool GetWorldMarkedRemove() const{ return pWorldMarkedRemove; }
	
	/**
	 * Set marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	void SetWorldMarkedRemove( bool marked );
	/*@}*/
	
private:
	void pUpdateHasFlags( const deDebugDrawer &debugDrawer );
	void pWriteVBOData( const deDebugDrawer &debugDrawer );
};

#endif
