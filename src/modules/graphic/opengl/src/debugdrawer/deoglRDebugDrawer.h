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
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRDebugDrawer> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render debug drawer. */
	deoglRDebugDrawer(deoglRenderThread &renderThread);
	
	/** Cleans up render debug drawer. */
	virtual ~deoglRDebugDrawer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{return pRenderThread;}
	
	
	
	/** Parent world or \em NULL if not part of a world. */
	inline deoglRWorld *GetParentWorld() const{return pParentWorld;}
	
	/** Set parent world. */
	void SetParentWorld(deoglRWorld *parentWorld);
	
	
	
	/** Matrix. */
	inline const decDMatrix &GetMatrix() const{return pMatrix;}
	
	/** Set matrix. */
	void SetMatrix(const decDMatrix &matrix);
	
	/** Debug drawer is visible. */
	inline bool GetVisible() const{return pVisible;}
	
	/** Set if debug drawer is visible. */
	void SetVisible(bool visible);
	
	/** Debug drawer is rendered in x-ray mode. */
	inline bool GetXRay() const{return pXRay;}
	
	/** Set debug drawer is rendered in x-ray mode. */
	void SetXRay(bool xray);
	
	
	
	/** Update shapes. */
	void UpdateShapes(const deDebugDrawer &debugDrawer);
	
	/** Determines if the debug drawer has shapes. */
	inline bool GetHasShapes() const{return pHasShapes;}
	
	/** Determines if the debug drawer has faces. */
	inline bool GetHasFaces() const{return pHasFaces;}
	
	/** Number of shapes. */
	inline int GetShapeCount() const{return pShapeCount;}
	
	/** Retrieves a shape. */
	deoglDebugDrawerShape &GetShapeAt(int index) const;
	
	
	
	/** Update the VBO if required and contains faces. */
	void UpdateVBO();
	
	/** Retrieves the vao or NULL if there are no shapes with faces. */
	inline deoglVAO *GetVAO() const{return pVAO;}
	
	
	
	/** Prepare for quick disposal of debug drawer. */
	void PrepareQuickDispose();
	/*@}*/
	
	
	
	/** \name Render world usage */
	/*@{*/
	/**
	 * Marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	inline bool GetWorldMarkedRemove() const{return pWorldMarkedRemove;}
	
	/**
	 * Set marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	void SetWorldMarkedRemove(bool marked);
	/*@}*/
	
private:
	void pUpdateHasFlags(const deDebugDrawer &debugDrawer);
	void pWriteVBOData(const deDebugDrawer &debugDrawer);
};

#endif
