/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEWDEBUGDRAWERSHAPE_H_
#define _IGDEWDEBUGDRAWERSHAPE_H_

#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerList.h>

class deNavigationSpace;
class deDebugDrawer;
class deDebugDrawerShape;
class deDebugDrawerShapeFace;
class deOcclusionMesh;



/**
 * \brief Debug drawer shape wrapper.
 * 
 * Manages a debug drawer shape for displaying editing information in the world.
 * Debug drawer shapes can be tricky to handle directly since they are direct
 * allocated with reference counting. This class stores the definition of a debug
 * drawer shape and creates, updates and destroys the engine debug drawer shape
 * according to this definition. This class is designed to greatly assist in
 * rendering editing information in the world without having to deal with the
 * low level tasks involved. The debug drawer shape is placed in the provided
 * debug drawer. If no debug drawer is set this equals to the debug drawer shape
 * being invisible. This class does not hold a reference to the provided debug
 * drawer. Therefore make sure the debug drawer is held as long as a debug drawer
 * wrapper object is using the debug drawer.
 */
class DE_DLL_EXPORT igdeWDebugDrawerShape{
private:
	deDebugDrawer *pEngDebugDrawer;
	deDebugDrawerShape *pEngDDShape;
	
	decVector pPosition;
	decQuaternion pOrientation;
	decVector pScale;
	
	decColor pColorEdge;
	decColor pColorFill;
	
	bool pVisible;
	
	decShapeList pShapes;
	
	decPointerList pFaces;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create wrapper. */
	igdeWDebugDrawerShape();
	
	/** \brief Clean up wrapper. */
	~igdeWDebugDrawerShape();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent debug drawer or NULL. */
	inline deDebugDrawer *GetParentDebugDrawer() const{ return pEngDebugDrawer; }
	
	/** \brief Set parent debug drawer or NULL. */
	void SetParentDebugDrawer( deDebugDrawer *debugDrawer );
	
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Scale. */
	inline const decVector &GetScale() const{ return pScale; }
	
	/** \brief Set scale. */
	void SetScale( const decVector &scale );
	
	/** \brief Edge color. */
	inline const decColor &GetEdgeColor() const{ return pColorEdge; }
	
	/** \brief Set edge color. */
	void SetEdgeColor( const decColor &color );
	
	/** \brief Fill color. */
	inline const decColor &GetFillColor() const{ return pColorFill; }
	
	/** \brief Set fill color. */
	void SetFillColor( const decColor &color );
	
	/** \brief Determines if the debug drawer shape is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** \brief Sets if the debug drawer shape is visible. */
	void SetVisible( bool visible );
	/*@}*/
	
	
	
	/** \name Shapes */
	/*@{*/
	/** \brief Add shape. */
	void AddShape( decShape *shape );
	
	/** \brief Add shapes. */
	void AddShapes( const decShapeList &shapes );
	
	/** \brief Adds a sphere shape. */
	void AddSphereShape( float radius, const decVector &position );
	
	/** \brief Adds a sphere shape. */
	void AddSphereShape( float radius, const decVector2 &axisScaling, const decVector &position );
	
	/** \brief Adds a box shape using the given parameters. */
	void AddBoxShape( const decVector &halfExtends, const decVector &position,
		const decQuaternion &orientation );
	
	/** \brief Adds a box shape using the given parameters. */
	void AddBoxShape( const decVector &halfExtends, const decVector2 &axiscaling,
		const decVector &position, const decQuaternion &orientation );
	
	/** \brief Adds a cylinder shape using the given parameters. */
	void AddCylinderShape( float halfHeight, float topRadius, float bottomRadius,
		const decVector &position, const decQuaternion &orientation );
	
	/** \brief Adds a cylinder shape using the given parameters. */
	void AddCylinderShape( float halfHeight, float topRadius, float bottomRadius,
		const decVector2 &topAxisScaling, const decVector2 &bottomAxisScaling,
		const decVector &position, const decQuaternion &orientation );
	
	/** \brief Adds a capsule shape using the given parameters. */
	void AddCapsuleShape( float halfHeight, float topRadius, float bottomRadius,
		const decVector &position, const decQuaternion &orientation );
	
	/** \brief Adds a capsule shape using the given parameters. */
	void AddCapsuleShape( float halfHeight, float topRadius, float bottomRadius,
		const decVector2 &topAxisScaling, const decVector2 &bottomAxisScaling,
		const decVector &position, const decQuaternion &orientation );
	
	/** \brief Add hull shape. */
	void AddHullShape( const decVector &position, const decQuaternion &orientation,
		int pointCount, const decVector *points );
	
	/** \brief Clears the list of shapes. */
	void RemoveAllShapes();
	/*@}*/
	
	
	
	/** \name Faces */
	/*@{*/
	/** \brief Adds a face. */
	void AddFace( deDebugDrawerShapeFace *face );
	
	/** \brief Adds faces from an occlusion mesh. */
	void AddOcclusionMeshFaces( const deOcclusionMesh &occlusionMesh );
	
	/** \brief Adds faces from a navigation space. */
	void AddNavSpaceFaces( const deNavigationSpace &navSpace );
	
	/** \brief Adds faces from a navigation space for a certain type only. */
	void AddNavSpaceFaces( const deNavigationSpace &navSpace, int type );
	
	/** \brief Removes all faces. */
	void RemoveAllFaces();
	/*@}*/
	
	
	
private:
	void pUpdateDDShape();
	void pRebuildShapes();
	void pBareRebuildShapes();
	void pRebuildFaces();
	void pBareRebuildFaces();
	
	void pAddNavGrid( const deNavigationSpace &navSpace, bool filterType, int type );
	void pAddNavMesh( const deNavigationSpace &navSpace, bool filterType, int type );
};

#endif
