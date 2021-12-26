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

#ifndef _DECXMLCHARACTERDATA_H_
#define _DECXMLCHARACTERDATA_H_

#include "decXmlElement.h"
#include "../string/decString.h"


/**
 * \brief XML Character Data.
 */
class DE_DLL_EXPORT decXmlCharacterData : public decXmlElement{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<decXmlCharacterData> Ref;
	
	
	
private:
	decString pData;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create xml character data. */
	decXmlCharacterData( const char *data );
	
protected:
	/** \brief Clean up xml character data. */
	virtual ~decXmlCharacterData();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Data. */
	inline const decString &GetData() const{ return pData; }
	
	/** \brief Set data. */
	void SetData( const char *data );
	
	/** \brief Append data. */
	void AppendData( const char *data );
	
	/** \brief Append character. */
	void AppendCharacter( char character );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	void Visit( decXmlVisitor &visitor );
	/*@}*/
	
	
	
	/** \name Casting */
	/*@{*/
	virtual bool CanCastToCharacterData() const;
	virtual decXmlCharacterData *CastToCharacterData();
	/*@}*/
};

#endif
