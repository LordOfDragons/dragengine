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

#ifndef _GDEOCNAVIGATIONBLOCKER_H_
#define _GDEOCNAVIGATIONBLOCKER_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>



/**
 * \brief Object class navigation blocker.
 */
class gdeOCNavigationBlocker : public deObject{
public:
	/** \brief Properties. */
	enum eProperties{
		/** \brief Enabled. */
		epEnabled,
		
		/** \brief Shape. */
		epShape,
		
		/** \brief Layer. */
		epLayer,
		
		/** \brief Blocking priority. */
		epBlockingPriority,
		
		/** \brief Attach position. */
		epAttachPosition,
		
		/** \brief Attach rotation. */
		epAttachRotation
	};
	
	
	
private:
	decVector pPosition;
	decVector pRotation;
	decVector pScaling;
	decString pBoneName;
	bool pEnabled;
	int pLayer;
	deNavigationSpace::eSpaceTypes pType;
	int pBlockingPriority;
	
	decShapeList pShapeList;
	
	decString pPropertyNames[ epAttachRotation + 1 ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create navigation blocker. */
	gdeOCNavigationBlocker();
	
	/** \brief Create copy of navigation blocker. */
	gdeOCNavigationBlocker( const gdeOCNavigationBlocker &blocker );
	
	/** \brief Clean up navigation blocker. */
	virtual ~gdeOCNavigationBlocker();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Rotation. */
	inline const decVector &GetRotation() const{ return pRotation; }
	
	/** \brief Set orientation. */
	void SetRotation( const decVector &orientation );
	
	/** \brief Scaling. */
	inline const decVector &GetScaling() const{ return pScaling; }
	
	/** \brief Set scaling. */
	void SetScaling( const decVector &scaling );
	
	/** \brief Name of the bone to attach to or empty string. */
	inline const decString &GetBoneName() const{ return pBoneName; }
	
	/** \brief Set name of the bone to attach to or empty string. */
	void SetBoneName( const char *boneName );
	
	/** \brief Blocker is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if blocker is enabled. */
	void SetEnabled( bool enabled );
	
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
	 * \details Blocks navigation meshes with a smaller or equal blocking priority.
	 */
	inline int GetBlockingPriority() const{ return pBlockingPriority; }
	
	/**
	 * \brief Set blocking priority.
	 * \details Blocks navigation meshes with a smaller or equal blocking priority.
	 */
	void SetBlockingPriority( int priority );
	
	/** \brief Shape list. */
	inline decShapeList &GetShapeList(){ return pShapeList; }
	inline const decShapeList &GetShapeList() const{ return pShapeList; }
	
	
	
	
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
