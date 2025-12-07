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

#ifndef _DECSHAPEVISITORIDENTIFY_H_
#define _DECSHAPEVISITORIDENTIFY_H_

#include "decShapeVisitor.h"


/**
 * \brief Visitor for identifying analytic shapes.
 */
class DE_DLL_EXPORT decShapeVisitorIdentify : public decShapeVisitor{
public:
	/** \brief Shape types. */
	enum eShapeTypes{
		/** \brief Unknown type. */
		estUnknown,
		
		/** \brief Sphere shape. */
		estSphere,
		
		/** \brief Box shape. */
		estBox,
		
		/** \brief Cylinder shape. */
		estCylinder,
		
		/** \brief Capsule shape. */
		estCapsule,
		
		/** \brief Hull shape. */
		estHull
	};
	
	
	
private:
	decShape *pShape;
	eShapeTypes pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shape visitor. */
	decShapeVisitorIdentify();
	
	/** \brief Clean up shape visitor. */
	virtual ~decShapeVisitorIdentify();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Shape type. */
	inline eShapeTypes GetType() const{ return pType; }
	
	
	
	/** \brief Visited shape is an unknown shape. */
	inline bool IsUnknown() const{ return pType == estUnknown; }
	
	/** \brief Visited shape is a sphere shape. */
	inline bool IsSphere() const{ return pType == estSphere; }
	
	/** \brief Visited shape is a box shape. */
	inline bool IsBox() const{ return pType == estBox; }
	
	/** \brief Visited shape is cylinder shape. */
	inline bool IsCylinder() const{ return pType == estCylinder; }
	
	/** \brief Visited shape is capsule shape. */
	inline bool IsCapsule() const{ return pType == estCapsule; }
	
	/** \brief Visited shape is hull shape. */
	inline bool IsHull() const{ return pType == estHull; }
	
	
	
	/**
	 * \brief Cast to sphere shape.
	 * \throws deeInvalidParam Visited shape is not a sphere shape.
	 */
	decShapeSphere &CastToSphere() const;
	
	/**
	 * \brief Cast to box shape.
	 * \throws deeInvalidParam Visited shape is not a box shape.
	 */
	decShapeBox &CastToBox() const;
	
	/**
	 * \brief Cast to cylinder shape.
	 * \throws deeInvalidParam Visited shape is not a cylinder shape.
	 */
	decShapeCylinder &CastToCylinder() const;
	
	/**
	 * \brief Cast to capsule shape.
	 * \throws deeInvalidParam Visited shape is not a capsule shape.
	 */
	decShapeCapsule &CastToCapsule() const;
	
	/**
	 * \brief Cast to hull shape.
	 * \throws deeInvalidParam Visited shape is not a hull shape.
	 */
	decShapeHull &CastToHull() const;
	
	
	
	/** \brief Reset visitor. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit shape. */
	virtual void VisitShape(decShape &shape);
	
	/** \brief Visit sphere shape. */
	virtual void VisitShapeSphere(decShapeSphere &sphere);
	
	/** \brief Visit box shape. */
	virtual void VisitShapeBox(decShapeBox &box);
	
	/** \brief Visit cylinder shape. */
	virtual void VisitShapeCylinder(decShapeCylinder &cylinder);
	
	/** \brief Visit capsule shape. */
	virtual void VisitShapeCapsule(decShapeCapsule &capsule);
	
	/** \brief Visit hull shape. */
	virtual void VisitShapeHull(decShapeHull &hull);
	/*@}*/
	
};

#endif
