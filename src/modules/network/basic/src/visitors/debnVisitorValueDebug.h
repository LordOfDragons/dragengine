/* 
 * Drag[en]gine Basic Network Module
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

#ifndef _DEBNVISITORVALUEDEBUG_H_
#define _DEBNVISITORVALUEDEBUG_H_

#include <dragengine/resources/network/value/deNetworkValueVisitor.h>

class deNetworkBasic;



/**
 * @brief Value Debug Visitor.
 * Debug visitor printing out a value on the console.
 */
class debnVisitorValueDebug : public deNetworkValueVisitor{
private:
	deNetworkBasic *pModule;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new visitor. */
	debnVisitorValueDebug( deNetworkBasic *module );
	/** Cleans up the visitor. */
	virtual ~debnVisitorValueDebug();
	/*@}*/
	
	/** @name Visiting */
	/*@{*/
	/** Visits a network value. */
	virtual void VisitValue( deNetworkValue *value );
	/** Visits an integer network value. */
	virtual void VisitInteger( deNetworkValueInteger *value );
	/** Visits a float network value. */
	virtual void VisitFloat( deNetworkValueFloat *value );
	/** Visits a string network value. */
	virtual void VisitString( deNetworkValueString *value );
	/** Visits a data network value. */
	virtual void VisitData( deNetworkValueData *value );
	/** Visits a point-2 network value. */
	virtual void VisitPoint2( deNetworkValuePoint2 *value );
	/** Visits a point-3 network value. */
	virtual void VisitPoint3( deNetworkValuePoint3 *value );
	/** Visits a vector-2 network value. */
	virtual void VisitVector2( deNetworkValueVector2 *value );
	/** Visits a vector-3 network value. */
	virtual void VisitVector3( deNetworkValueVector3 *value );
	/** Visits a quaternion network value. */
	virtual void VisitQuaternion( deNetworkValueQuaternion *value );
	/*@}*/
};

// end of include only once
#endif
