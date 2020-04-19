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

#ifndef _DENETWORKVALUEVISITOR_H_
#define _DENETWORKVALUEVISITOR_H_

class deNetworkValue;
class deNetworkValueInteger;
class deNetworkValueFloat;
class deNetworkValueString;
class deNetworkValueData;
class deNetworkValuePoint2;
class deNetworkValuePoint3;
class deNetworkValueVector2;
class deNetworkValueVector3;
class deNetworkValueQuaternion;


/**
 * \brief Network Value Visitor Class.
 */
class deNetworkValueVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network value visitor object. */
	deNetworkValueVisitor();
	
	/** \brief Clean up network value visitor. */
	virtual ~deNetworkValueVisitor();
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
