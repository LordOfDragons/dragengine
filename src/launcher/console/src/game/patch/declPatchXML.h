/* 
 * Drag[en]gine Console Launcher
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

#ifndef _DECLPATCHXML_H_
#define _DECLPATCHXML_H_

#include "../../declBaseXML.h"

class declPatch;
class decBaseFileReader;



/**
 * \brief Load game patch XML.
 */
class declPatchXML : public declBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create loader. */
	declPatchXML( deLogger *logger, const char *loggerSource );
	
	/** \brief Clean up loader. */
	virtual ~declPatchXML();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Read from file. */
	void ReadFromFile( decBaseFileReader &reader, declPatch &patch );
	/*@}*/
	
	
	
private:
	void pReadPatch( const decXmlElementTag &root, declPatch &patch );
};

#endif
