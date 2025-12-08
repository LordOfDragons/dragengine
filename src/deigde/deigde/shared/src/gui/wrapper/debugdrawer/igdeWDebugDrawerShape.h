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

#ifndef _IGDEWDEBUGDRAWERSHAPE_H_
#define _IGDEWDEBUGDRAWERSHAPE_H_

#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerList.h>

class deNavigationSpace;
#include <dragengine/resources/debug/deDebugDrawer.h>
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
	deDebugDrawer::Ref pEngDebugDrawer;
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
	inline const deDebugDrawer::Ref &GetParentDebugDrawer() const{ return pEngDebugDrawer; }
	
	/** \brief Set parent debug drawer or NULL. */
	void SetParentDebugDrawer(deDebugDrawer *debugDrawer);
	
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition(const decVector &position);
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation(const decQuaternion &orientation);
	
	/** \brief Scale. */
	inline const decVector &GetScale() const{ return pScale; }
	
	/** \brief Set scale. */
	void SetScale(const decVector &scale);
	
	/** \brief Edge color. */
	inline const decColor &GetEdgeColor() const{ return pColorEdge; }
	
	/** \brief Set edge color. */
	void SetEdgeColor(const decColor &color);
	
	/** \brief Fill color. */
	inline const decColor &GetFillColor() const{ return pColorFill; }
	
	/** \brief Set fill color. */
	void SetFillColor(const decColor &color);
	
	/** \brief Determines if the debug drawer shape is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** \brief Sets if the debug drawer shape is visible. */
	void SetVisible(bool visible);
	/*@}*/
	
	
	
	/** \name Shapes */
	/*@{*/
	/** \brief Add shape. */
	void AddShape(decShape *shape);
	
	/** \brief Add shapes. */
	void AddShapes(const decShapeList &shapes);
	
	/** \brief Adds a sphere shape. */
	void AddSphereShape(float radius, const decVector &position);
	
	/** \brief Adds a sphere shape. */
	void AddSphereShape(float radius, const decVector2 &axisScaling, const decVector &position);
	
	/** \brief Adds a box shape using the given parameters. */
	void AddBoxShape(const decVector &halfExtends, const decVector &position,
		const decQuaternion &orientation);
	
	/** \brief Adds a box shape using the given parameters. */
	void AddBoxShape(const decVector &halfExtends, const decVector2 &axiscaling,
		const decVector &position, const decQuaternion &orientation);
	
	/** \brief Adds a cylinder shape using the given parameters. */
	void AddCylinderShape(float halfHeight, float topRadius, float bottomRadius,
		const decVector &position, const decQuaternion &orientation);
	
	/** \brief Adds a cylinder shape using the given parameters. */
	void AddCylinderShape(float halfHeight, float topRadius, float bottomRadius,
		const decVector2 &topAxisScaling, const decVector2 &bottomAxisScaling,
		const decVector &position, const decQuaternion &orientation);
	
	/** \brief Adds a capsule shape using the given parameters. */
	void AddCapsuleShape(float halfHeight, float topRadius, float bottomRadius,
		const decVector &position, const decQuaternion &orientation);
	
	/** \brief Adds a capsule shape using the given parameters. */
	void AddCapsuleShape(float halfHeight, float topRadius, float bottomRadius,
		const decVector2 &topAxisScaling, const decVector2 &bottomAxisScaling,
		const decVector &position, const decQuaternion &orientation);
	
	/** \brief Add hull shape. */
	void AddHullShape(const decVector &position, const decQuaternion &orientation,
		int pointCount, const decVector *points);
	
	/** \brief Clears the list of shapes. */
	void RemoveAllShapes();
	/*@}*/
	
	
	
	/** \name Faces */
	/*@{*/
	/** \brief Adds a face. */
	void AddFace(deDebugDrawerShapeFace *face);
	
	/** \brief Adds faces from an occlusion mesh. */
	void AddOcclusionMeshFaces(const deOcclusionMesh &occlusionMesh);
	
	/** \brief Adds faces from a navigation space. */
	void AddNavSpaceFaces(const deNavigationSpace &navSpace);
	
	/** \brief Adds faces from a navigation space for a certain type only. */
	void AddNavSpaceFaces(const deNavigationSpace &navSpace, int type);
	
	/** \brief Removes all faces. */
	void RemoveAllFaces();
	/*@}*/
	
	
	
private:
	void pUpdateDDShape();
	void pRebuildShapes();
	void pBareRebuildShapes();
	void pRebuildFaces();
	void pBareRebuildFaces();
	
	void pAddNavGrid(const deNavigationSpace &navSpace, bool filterType, int type);
	void pAddNavMesh(const deNavigationSpace &navSpace, bool filterType, int type);
};

#endif
