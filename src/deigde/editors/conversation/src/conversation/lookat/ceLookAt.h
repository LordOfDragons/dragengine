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

#ifndef _CELOOKAT_H_
#define _CELOOKAT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class ceConversation;



/**
 * \brief Look At.
 * Defines where the body, head or eyes of an actor are looking at. Uses a
 * target as the look at point and optionally orientation if required.
 */
class ceLookAt : public deObject{
private:
	ceConversation *pConversation;
	
	decString pName;
	decString pTarget;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new target. */
	ceLookAt( const char *name = "Look-At" );
	/** Creates a new target. */
	ceLookAt( const ceLookAt &target );
	/** Cleans up the target. */
	virtual ~ceLookAt();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the conversation or NULL if not set. */
	inline ceConversation *GetConversation() const{ return pConversation; }
	/** Sets the conversation or NULL if not set. */
	void SetConversation( ceConversation *conversation );
	
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name. */
	void SetName( const char *name );
	
	/** Retrieves the target or an empty string if not set. */
	inline const decString &GetTarget() const{ return pTarget; }
	/** Sets the target or an empty string if not set. */
	void SetTarget( const char *target );
	/*@}*/
};

#endif
