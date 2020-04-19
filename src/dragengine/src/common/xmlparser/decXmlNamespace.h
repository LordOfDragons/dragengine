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

#ifndef _DECXMLNAMESPACE_H_
#define _DECXMLNAMESPACE_H_

#include "decXmlElement.h"
#include "../string/decString.h"


/**
 * \brief XML Namespace.
 */
class decXmlNamespace : public decXmlElement{
private:
	decString pName;
	decString pURL;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create xml namespace with the given name and url. */
	decXmlNamespace( const char *name, const char *url );
	
	/** \brief Create xml namespace with the given name and an empty url. */
	decXmlNamespace( const char *name );
	
protected:
	/** \brief Clean up xml namespace. */
	virtual ~decXmlNamespace();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief URL. */
	inline const decString &GetURL() const{ return pURL; }
	
	/** \brief Set URL. */
	void SetURL( const char *url );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	void Visit( decXmlVisitor &visitor );
	/*@}*/
	
	
	
	/** \name Casting */
	/*@{*/
	virtual bool CanCastToNamespace() const;
	virtual decXmlNamespace *CastToNamespace();
	/*@}*/
};

#endif
