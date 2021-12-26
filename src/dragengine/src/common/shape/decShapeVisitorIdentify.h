/* 
 * Drag[en]gine Game Engine
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
	virtual void VisitShape( decShape &shape );
	
	/** \brief Visit sphere shape. */
	virtual void VisitShapeSphere( decShapeSphere &sphere );
	
	/** \brief Visit box shape. */
	virtual void VisitShapeBox( decShapeBox &box );
	
	/** \brief Visit cylinder shape. */
	virtual void VisitShapeCylinder( decShapeCylinder &cylinder );
	
	/** \brief Visit capsule shape. */
	virtual void VisitShapeCapsule( decShapeCapsule &capsule );
	
	/** \brief Visit hull shape. */
	virtual void VisitShapeHull( decShapeHull &hull );
	/*@}*/
	
};

#endif
