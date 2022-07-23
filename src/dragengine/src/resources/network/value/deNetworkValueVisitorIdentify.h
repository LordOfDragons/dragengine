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

#ifndef _DENETWORKSTATEVISITORIDENTIFY_H_
#define _DENETWORKSTATEVISITORIDENTIFY_H_

#include "deNetworkValueVisitor.h"


/**
 * \brief Network Value Visitor Identify.
 *
 * Network Value visitor providing value identification. After visiting a
 * collidder the visitor can be asked what kind of value has been visited.
 * Useful to identify values and to carry out actions only for certain
 * value types. Furthermore save casting is provided. If the cast is not
 * valid an exception is raised.
 */
class DE_DLL_EXPORT deNetworkValueVisitorIdentify : public deNetworkValueVisitor{
public:
	enum eNetworkValues{
		envtInteger,
		envtFloat,
		envtString,
		envtData,
		envtPoint2,
		envtPoint3,
		envtVector2,
		envtVector3,
		envtQuaternion,
		envtUnknown = -1,
	};
	
	
	
private:
	deNetworkValue *pValue;
	eNetworkValues pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create value visitor identify object. */
	deNetworkValueVisitorIdentify();
	
	/** \brief Clean up value visitor identify. */
	virtual ~deNetworkValueVisitorIdentify();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Vvalue type. */
	inline eNetworkValues GetType() const{ return pType; }
	
	/** \brief Is unknown value. */
	inline bool IsUnknown() const{ return pType == envtUnknown; }
	
	/** \brief Is integer value. */
	inline bool IsInteger() const{ return pType == envtInteger; }
	
	/** \brief Is float value. */
	inline bool IsFloat() const{ return pType == envtFloat; }
	
	/** \brief Is string value. */
	inline bool IsString() const{ return pType == envtString; }
	
	/** \brief Is data value. */
	inline bool IsData() const{ return pType == envtData; }
	
	/** \brief Is point-2 value. */
	inline bool IsPoint2() const{ return pType == envtPoint2; }
	
	/** \brief Is point-3 value. */
	inline bool IsPoint3() const{ return pType == envtPoint3; }
	
	/** \brief Is vector-2 value. */
	inline bool IsVector2() const{ return pType == envtVector2; }
	
	/** \brief Is vector-3 value. */
	inline bool IsVector3() const{ return pType == envtVector3; }
	
	/** \brief Is quaternion value. */
	inline bool IsQuaternion() const{ return pType == envtQuaternion; }
	
	/** \brief Cast to integer network value. */
	deNetworkValueInteger *CastToInteger() const;
	
	/** \brief Cast to float network value. */
	deNetworkValueFloat *CastToFloat() const;
	
	/** \brief Cast to string network value. */
	deNetworkValueString *CastToString() const;
	
	/** \brief Cast to data network value. */
	deNetworkValueData *CastToData() const;
	
	/** \brief Cast to point-2 network value. */
	deNetworkValuePoint2 *CastToPoint2() const;
	
	/** \brief Cast to point-3 network value. */
	deNetworkValuePoint3 *CastToPoint3() const;
	
	/** \brief Cast to vector-2 network value. */
	deNetworkValueVector2 *CastToVector2() const;
	
	/** \brief Cast to vector-3 network value. */
	deNetworkValueVector3 *CastToVector3() const;
	
	/** \brief Cast to quaternion network value. */
	deNetworkValueQuaternion *CastToQuaternion() const;
	
	/** \brief Reset visitor. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit network value. */
	virtual void VisitValue( deNetworkValue *value );
	
	/** \brief Visit integer network value. */
	virtual void VisitInteger( deNetworkValueInteger *value );
	
	/** \brief Visit float network value. */
	virtual void VisitFloat( deNetworkValueFloat *value );
	
	/** \brief Visit string network value. */
	virtual void VisitString( deNetworkValueString *value );
	
	/** \brief Visit data network value. */
	virtual void VisitData( deNetworkValueData *value );
	
	/** \brief Visit point-2 network value. */
	virtual void VisitPoint2( deNetworkValuePoint2 *value );
	
	/** \brief Visit point-3 network value. */
	virtual void VisitPoint3( deNetworkValuePoint3 *value );
	
	/** \brief Visit vector-2 network value. */
	virtual void VisitVector2( deNetworkValueVector2 *value );
	
	/** \brief Visit vector-3 network value. */
	virtual void VisitVector3( deNetworkValueVector3 *value );
	
	/** \brief Visit quaternion network value. */
	virtual void VisitQuaternion( deNetworkValueQuaternion *value );
	/*@}*/
};

#endif
