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

#ifndef _IGDEGDCSNAPPOINT_H_
#define _IGDEGDCSNAPPOINT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Object class snap point.
 */
class DE_DLL_EXPORT igdeGDCSnapPoint : public deObject{
private:
	decString pName;
	
	decVector pPosition;
	decVector pRotation;
	
	float pSnapDistance;
	bool pSnapToRotation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create snap point. */
	igdeGDCSnapPoint();
	
	/** \brief Create copy of snap point. */
	igdeGDCSnapPoint( const igdeGDCSnapPoint &snapPoint );
	
protected:
	/** \brief Clean up snap point. */
	virtual ~igdeGDCSnapPoint();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Display name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set display name. */
	void SetName( const char *name );
	
	
	
	/** \brief Snap position relative to parent object. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set snap position relative to parent object. */
	void SetPosition( const decVector &position );
	
	/** \brief Snap orientation relative to parent object. */
	inline const decVector &GetRotation() const{ return pRotation; }
	
	/** \brief Set snap orientation relative to parent object. */
	void SetRotation( const decVector &orientation );
	
	
	
	/** \brief Snap distance in meters. */
	inline float GetSnapDistance() const{ return pSnapDistance; }
	
	/** \brief Set snap distance in meters. */
	void SetSnapDistance( float distance );
	
	/** \brief Snap to orientation. */
	inline bool GetSnapToRotation() const{ return pSnapToRotation; }
	
	/** \brief Set snap to orientation. */
	void SetSnapToRotation( bool snapToRotation );
	/*@}*/
};

#endif
