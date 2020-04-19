/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DECXMLENTITYREFERENCE_H_
#define _DECXMLENTITYREFERENCE_H_

#include "decXmlElement.h"
#include "../string/decString.h"



/**
 * \brief XML Entity Reference.
 */
class decXmlEntityReference : public decXmlElement{
private:
	decString pName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create xml entity reference. */
	decXmlEntityReference( const char *name );
	
protected:
	/** \brief Clean up xml entity refernece. */
	virtual ~decXmlEntityReference();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	void Visit( decXmlVisitor &visitor );
	/*@}*/
	
	/** \name Casting */
	/*@{*/
	virtual bool CanCastToEntityReference() const;
	virtual decXmlEntityReference *CastToEntityReference();
	/*@}*/
};

#endif
