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

#ifndef _DECXMLELEMENTTAG_H_
#define _DECXMLELEMENTTAG_H_

#include "decXmlContainer.h"
#include "../string/decString.h"


/**
 * \brief XML Element Tag.
 */
class decXmlElementTag : public decXmlContainer{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<decXmlElementTag> Ref;
	
	
	
private:
	decString pName;
	decString pNamespace;
	decString pLocalName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create xml element tag. */
	decXmlElementTag( const char *name );
	
protected:
	/** \brief Clean up xml element tag. */
	virtual ~decXmlElementTag();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Namespace part of the tag name. */
	inline const decString &GetNamespace() const{ return pNamespace; }
	
	/** \brief Local name part of the tag name. */
	inline const decString &GetLocalName() const{ return pLocalName; }
	/*@}*/
	
	
	
	/** \name Helper Functions */
	/*@{*/
	/** \brief First element tag beeing a decXmlCharacterData or NULL otherwise. */
	decXmlCharacterData *GetFirstData() const;
	
	/** \brief Index-th element tag beeing a decXmlElementTag or NULL otherwise. */
	decXmlElementTag *GetElementIfTag( int index ) const;
	
	/** \brief decXmlAttValue with the given name or NULL otherwise. */
	decXmlAttValue *FindAttribute( const char *name ) const;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	void Visit( decXmlVisitor &visitor );
	/*@}*/
	
	
	
	/** \name Casting */
	/*@{*/
	virtual bool CanCastToElementTag() const;
	virtual decXmlElementTag *CastToElementTag();
	/*@}*/
};

#endif
