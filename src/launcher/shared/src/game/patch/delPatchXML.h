/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DELPATCHXML_H_
#define _DELPATCHXML_H_

#include "../../utils/delBaseXML.h"

class delPatch;
class decBaseFileReader;


/**
 * \brief Load game patch XML.
 */
class DE_DLL_EXPORT delPatchXML : public delBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create loader. */
	delPatchXML( deLogger *logger, const char *loggerSource );
	
	/** \brief Clean up loader. */
	virtual ~delPatchXML();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Read from file. */
	void ReadFromFile( decBaseFileReader &reader, delPatch &patch );
	/*@}*/
	
	
	
private:
	void pReadPatch( const decXmlElementTag &root, delPatch &patch );
};

#endif
