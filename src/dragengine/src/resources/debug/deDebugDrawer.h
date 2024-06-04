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

#ifndef _DEDEBUGDRAWER_H_
#define _DEDEBUGDRAWER_H_

#include "../../common/math/decMath.h"
#include "../../common/collection/decPointerList.h"
#include "../deResource.h"

class deDebugDrawerShape;
class deDebugDrawerManager;
class deBaseGraphicDebugDrawer;
class deWorld;


/**
 * \brief Debug Drawer Object.
 * 
 * A debug drawer is a helper object useful for debuging various engine
 * modules. The graphic module knows how to render efficiently debug
 * drawers ontop of a rendered world. Engine modules can register a
 * debug drawer with world objects to display useful information. Every
 * debug drawer object specifies a list of shapes and visual information
 * on how to render them. Debug drawers can be individually set invisible
 * if required.
 * \par Parameters
 * There are a couple of parameters that can be set for a debug drawer to define
 * the appearance on screen. The visible flag determines if the debug drawer is
 * rendered at all. The X-Ray flag determines if the shape can be seen through the
 * already rendered geometry or if it is clipped by the depth buffer. The edge and
 * fill color flag determine the color and transparency of the edges and the faces
 * of the shape. By default the edge is set to white fully opaque and the fill is
 * set to white fully transparent.
 */
class DE_DLL_EXPORT deDebugDrawer : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deDebugDrawer> Ref;
	
	
	
private:
	decDVector pPosition;
	decQuaternion pOrientation;
	decVector pScale;
	
	bool pVisible;
	bool pXRay;
	
	decPointerList pShapes;
	
	deBaseGraphicDebugDrawer *pPeerGraphic;
	
	deWorld *pParentWorld;
	deDebugDrawer *pLLWorldPrev;
	deDebugDrawer *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new debug drawer object with the given resource manager. */
	deDebugDrawer( deDebugDrawerManager *manager );
	
protected:
	/**
	 * \brief Clean up debug drawer.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deDebugDrawer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Scale. */
	inline const decVector &GetScale() const{ return pScale; }
	
	/** \brief Set scale. */
	void SetScale( const decVector &scale );
	
	/** \brief Debug drawer is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** \brief Set if debug drawer is visible. */
	void SetVisible( bool visible );
	
	/** \brief Debug drawer is rendered in x-ray mode. */
	inline bool GetXRay() const{ return pXRay; }
	
	/** \brief Set if debug drawer is rendered in x-ray mode. */
	void SetXRay( bool xray );
	/*@}*/
	
	
	
	/** \name Shape Management */
	/*@{*/
	/** \brief Count of shapes. */
	int GetShapeCount() const;
	
	/** \brief Shape at the given index. */
	deDebugDrawerShape *GetShapeAt( int index ) const;
	
	/** \brief Retrieve index of the given shape. */
	int IndexOfShape( deDebugDrawerShape *shape ) const;
	
	/** \brief Shape exists. */
	bool HasShape( deDebugDrawerShape *shape ) const;
	
	/**
	 * \brief Adds a collision shape.
	 * 
	 * calls NotifyShapeLayoutChanged.
	 */
	void AddShape( deDebugDrawerShape *shape );
	
	/**
	 * \brief Removes the collision shape.
	 * 
	 * calls NotifyShapeLayoutChanged.
	 */
	void RemoveShape( deDebugDrawerShape *shape );
	
	/**
	 * \brief Removes the collision shape at the given index.
	 * 
	 * calls NotifyShapeLayoutChanged.
	 */
	void RemoveShapeFrom( int index );
	
	/**
	 * \brief Removes all collision shapes.
	 * 
	 * calls NotifyShapeLayoutChanged.
	 */
	void RemoveAllShapes();
	
	/** \brief Notify peers that shape edge or fill colors changed. */
	void NotifyShapeColorChanged();
	
	/** \brief Notify peers that shape position, orientation or scaling changed. */
	void NotifyShapeGeometryChanged();
	
	/** \brief Notify peers that shape shapes or faces changed. */
	void NotifyShapeContentChanged();
	
	/** \brief Notify peers that shapes have been added or removed. */
	void NotifyShapeLayoutChanged();
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphics system peer object. */
	inline deBaseGraphicDebugDrawer *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphics system peer object. */
	void SetPeerGraphic( deBaseGraphicDebugDrawer *peer );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld( deWorld *world );
	
	/** \brief Previous debug drawer in the parent world linked list. */
	inline deDebugDrawer *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next debug drawer in the parent world linked list. */
	void SetLLWorldPrev( deDebugDrawer *debugDrawer );
	
	/** \brief Next debug drawer in the parent world linked list. */
	inline deDebugDrawer *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next debug drawer in the parent world linked list. */
	void SetLLWorldNext( deDebugDrawer *debugDrawer );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
