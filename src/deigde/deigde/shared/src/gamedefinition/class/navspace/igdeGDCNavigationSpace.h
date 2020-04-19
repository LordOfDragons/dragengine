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

#ifndef _IGDEGDCNAVIGATIONSPACE_H_
#define _IGDEGDCNAVIGATIONSPACE_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/deObject.h>



/**
 * \brief Game definition class navigation space.
 */
class igdeGDCNavigationSpace : public deObject{
public:
	/** \brief Properties. */
	enum eProperties{
		epPath,
		epBlockerShape,
		epLayer,
		epBlockingPriority,
		epSnapDistance,
		epSnapAngle,
		epAttachPosition,
		epAttachRotation
	};
	
	
	
private:
	decString pPath;
	decVector pPosition;
	decQuaternion pOrientation;
	decString pBoneName;
	int pLayer;
	float pSnapDistance;
	float pSnapAngle;
	
	int pBlockingPriority;
	decShapeList pBlockerShapeList;
	
	decString pPropertyNames[ epAttachRotation + 1 ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class navigation space. */
	igdeGDCNavigationSpace();
	
	/** \brief Create copy of a game definition class navigation space. */
	igdeGDCNavigationSpace( const igdeGDCNavigationSpace &speaker );
	
protected:
	/** \brief Clean up class navigation space. */
	virtual ~igdeGDCNavigationSpace();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Path to use. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set path to use. */
	void SetPath( const char *path );
	
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Name of the bone to attach to or empty string. */
	inline const decString &GetBoneName() const{ return pBoneName; }
	
	/** \brief Set name of bone to attach to or empty string. */
	void SetBoneName( const char *boneName );
	
	/** \brief Layer number. */
	inline int GetLayer() const{ return pLayer; }
	
	/** \brief Set layer number. */
	void SetLayer( int layer );
	
	/**
	 * \brief Blocking priority.
	 * 
	 * Navigation meshes is blocked by all blockers with an equal or larger blocking priority.
	 * If a blocker shape exists it blocks all navigation meshes with equal or less blocking priority.
	 */
	inline int GetBlockingPriority() const{ return pBlockingPriority; }
	
	/**
	 * \brief Set blocking priority.
	 * 
	 * Navigation meshes is blocked by all blockers with an equal or larger blocking priority.
	 * If a blocker shape exists it blocks all navigation meshes with equal or less blocking priority.
	 */
	void SetBlockingPriority( int priority );
	
	/** \brief Blocker shape list. */
	inline const decShapeList &GetBlockerShapeList() const{ return pBlockerShapeList; }
	
	/** \brief Set blocker shape list. */
	void SetBlockerShapeList( const decShapeList &shapeList );
	
	/** \brief Snap distance. */
	inline float GetSnapDistance() const{ return pSnapDistance; }
	
	/** \brief Set snap distance. */
	void SetSnapDistance( float distance );
	
	/** \brief Snap angle. */
	inline float GetSnapAngle() const{ return pSnapAngle; }
	
	/** \brief Set snap angle. */
	void SetSnapAngle( float angle );
	
	/** \brief Determines if a property name is set or not (empty string). */
	bool IsPropertySet( eProperties property ) const;
	
	/** \brief Name of the given property. */
	const decString &GetPropertyName( eProperties property ) const;
	
	/** \brief Set name of the given property. */
	void SetPropertyName( eProperties property, const char *name );
	
	/** \brief Determines if one or more properties use a name. */
	bool HasPropertyWithName( const char *name ) const;
	/*@}*/
};

#endif
