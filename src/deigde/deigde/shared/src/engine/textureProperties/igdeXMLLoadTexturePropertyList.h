/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEXMLLOADTEXTUREPROPERTYLIST_H_
#define _IGDEXMLLOADTEXTUREPROPERTYLIST_H_

#include "../../utils/igdeBaseXML.h"

class igdeTextureProperty;
class igdeTexturePropertyList;
class decBaseFileReader;



/**
 * \brief Load texture property list.
 */
class igdeXMLLoadTexturePropertyList : public igdeBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create loader. */
	igdeXMLLoadTexturePropertyList( deLogger *logger );
	
	/** \brief Clean up loader. */
	virtual ~igdeXMLLoadTexturePropertyList();
	/*@}*/
	
	
	
	/** \name Loading */
	/*@{*/
	/** \brief Load texture property list. */
	void ReadFromFile( igdeTexturePropertyList &list, decBaseFileReader &file );
	/*@}*/
	
	
	
private:
	void pReadList( const decXmlElementTag &root, igdeTexturePropertyList &list );
	void pReadProperty( const decXmlElementTag &root, igdeTexturePropertyList &list );
	void pReadDefault( const decXmlElementTag &root, igdeTextureProperty &property );
	void pReadAffects( const decXmlElementTag &root, igdeTextureProperty &property );
};

#endif
