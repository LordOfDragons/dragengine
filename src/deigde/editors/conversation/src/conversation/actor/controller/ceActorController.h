/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CEACTORCONTROLLER_H_
#define _CEACTORCONTROLLER_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Controller in a conversation actor animator instance.
 */
class ceActorController : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceActorController> Ref;
	
	
	
public:
	/** \brief Update types. */
	enum eUpdateTypes{
		/** \brief Set controller to a fixed value. */
		eutConstant,
		
		/** \brief Update controller using the elapsed time. */
		eutElapsedTime,
		
		/** \brief Set controller to the head turning left and right value. */
		eutHeadLeftRight,
		
		/** \brief Set controller to the head looking up and down value. */
		eutHeadUpDown,
		
		/** \brief Set controller to the eyes looking left and right value. */
		eutEyesLeftRight,
		
		/** \brief Set controller to the eyes looking up and down value. */
		eutEyesUpDown
	};
	
	
	
private:
	decString pName;
	eUpdateTypes pUpdateType;
	float pValue;
	decVector pVector;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create actor controller. */
	ceActorController();
	
	/** \brief Create copy of controller. */
	ceActorController( const ceActorController &controller );
	
protected:
	/** \brief Clean up actor controller. */
	virtual ~ceActorController();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Update type. */
	inline eUpdateTypes GetUpdateType() const{ return pUpdateType; }
	
	/** \brief Set update type. */
	void SetUpdateType( eUpdateTypes type );
	
	/** \brief Constant value. */
	inline float GetValue() const{ return pValue; }
	
	/** \brief Set constant value. */
	void SetValue( float value );
	
	/** \brief Constant vector. */
	inline const decVector &GetVector() const{ return pVector; }
	
	/** \brief Set constant vector. */
	void SetVector( const decVector &vector );
	/*@}*/
};

#endif
