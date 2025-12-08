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

#ifndef _DEOGLDEBUGDRAWER_H_
#define _DEOGLDEBUGDRAWER_H_

#include <dragengine/systems/modules/graphic/deBaseGraphicDebugDrawer.h>

#include "deoglRDebugDrawer.h"

class deGraphicOpenGl;
class deDebugDrawer;



/**
 * Debug Drawer Peer.
 */
class deoglDebugDrawer : public deBaseGraphicDebugDrawer{
private:
	deGraphicOpenGl &pOgl;
	const deDebugDrawer &pDebugDrawer;
	
	deoglRDebugDrawer::Ref pRDebugDrawer;
	
	bool pDirtyDebugDrawer;
	bool pDirtyShapes;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglDebugDrawer(deGraphicOpenGl &ogl, const deDebugDrawer &debugDrawer);
	
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
	inline const deoglRDebugDrawer::Ref &GetRDebugDrawer() const{ return pRDebugDrawer; }
	
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
