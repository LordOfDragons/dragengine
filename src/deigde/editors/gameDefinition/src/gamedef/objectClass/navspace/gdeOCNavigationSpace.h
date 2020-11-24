/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEOCNAVIGATIONSPACE_H_
#define _GDEOCNAVIGATIONSPACE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>



/**
 * \brief Object class navigation space.
 */
class gdeOCNavigationSpace : public deObject{
public:
	/** \brief Properties. */
	enum eProperties{
		/** \brief Path to navigation space file. */
		epPath,
		
		/** \brief Blocker shape. */
		epBlockerShape,
		
		/** \brief Layer. */
		epLayer,
		
		/** \brief Blocking priority. */
		epBlockingPriority,
		
		/** \brief Snap distance. */
		epSnapDistance,
		
		/** \brief Snap angle. */
		epSnapAngle,
		
		/** \brief Attach position. */
		epAttachPosition,
		
		/** \brief Attach rotation. */
		epAttachRotation
	};
	
	
	
private:
	decString pPath;
	decVector pPosition;
	decVector pRotation;
	decString pBoneName;
	int pLayer;
	float pSnapDistance;
	float pSnapAngle;
	
	deNavigationSpace::eSpaceTypes pType;
	int pBlockingPriority;
	decShapeList pBlockerShapeList;
	
	decString pPropertyNames[ epAttachRotation + 1 ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create navigation space. */
	gdeOCNavigationSpace();
	
	/** \brief Create copy of navigation space. */
	gdeOCNavigationSpace( const gdeOCNavigationSpace &space );
	
	/** \brief Clean up navigation space. */
	virtual ~gdeOCNavigationSpace();
	/*@}*/
	
	
	
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
	
	/** \brief Rotation. */
	inline const decVector &GetRotation() const{ return pRotation; }
	
	/** \brief Set orientation. */
	void SetRotation( const decVector &orientation );
	
	/** \brief Name of the bone to attach to or empty string. */
	inline const decString &GetBoneName() const{ return pBoneName; }
	
	/** \brief Set name of the bone to attach to or empty string. */
	void SetBoneName( const char *boneName );
	
	/** \brief Layer number. */
	inline int GetLayer() const{ return pLayer; }
	
	/** \brief Set layer number. */
	void SetLayer( int layer );
	
	/** \brief Space type. */
	inline deNavigationSpace::eSpaceTypes GetType() const{ return pType; }
	
	/** \brief Set space type. */
	void SetType( deNavigationSpace::eSpaceTypes type );
	
	/**
	 * \brief Blocking priority.
	 * \details Navigation meshes is blocked by all blockers with an equal or
	 *          larger blocking priority. If a blocker shape exists it blocks
	 *          all navigation meshes with equal or less blocking priority.
	 */
	inline int GetBlockingPriority() const{ return pBlockingPriority; }
	
	/**
	 * \brief Set blocking priority.
	 * \details Navigation meshes is blocked by all blockers with an equal or
	 *          larger blocking priority. If a blocker shape exists it blocks
	 *          all navigation meshes with equal or less blocking priority.
	 */
	void SetBlockingPriority( int priority );
	
	/** \brief Blocker shape list. */
	inline decShapeList &GetBlockerShapeList(){ return pBlockerShapeList; }
	inline const decShapeList &GetBlockerShapeList() const{ return pBlockerShapeList; }
	
	/** \brief Snap distance. */
	inline float GetSnapDistance() const{ return pSnapDistance; }
	
	/** \brief Set snap distance. */
	void SetSnapDistance( float distance );
	
	/** \brief Snap angle. */
	inline float GetSnapAngle() const{ return pSnapAngle; }
	
	/** \brief Set snap angle. */
	void SetSnapAngle( float angle );
	
	
	
	
	/** \brief Property name is set. */
	bool IsPropertySet( eProperties property ) const;
	
	/** \brief Name of property. */
	const decString &GetPropertyName( eProperties property ) const;
	
	/** \brief Set name of property. */
	void SetPropertyName( eProperties property, const char *name );
	
	/** \brief One or more properties use a name. */
	bool HasPropertyWithName( const char *name ) const;
	/*@}*/
};

#endif
