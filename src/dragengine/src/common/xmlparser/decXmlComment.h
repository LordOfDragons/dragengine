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

#ifndef _DECXMLCOMMENT_H_
#define _DECXMLCOMMENT_H_

#include "decXmlElement.h"
#include "../string/decString.h"


/**
 * \brief XML Comment.
 */
class DE_DLL_EXPORT decXmlComment : public decXmlElement{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<decXmlComment> Ref;
	
	
	
private:
	decString pComment;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create xml comment. */
	decXmlComment( const char *comment );
	
protected:
	/** \brief Clean up xml comment. */
	virtual ~decXmlComment();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Comment.*/
	inline const decString &GetComment() const{ return pComment; }
	
	/** \brief Set comment. */
	void SetComment( const char *comment );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	void Visit( decXmlVisitor &visitor );
	/*@}*/
	
	
	
	/** \name Casting */
	/*@{*/
	virtual bool CanCastToComment() const;
	virtual decXmlComment *CastToComment();
	/*@}*/
};

#endif
