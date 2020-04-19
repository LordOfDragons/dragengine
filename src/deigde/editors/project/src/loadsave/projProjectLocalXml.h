/* 
 * Drag[en]gine IGDE Project Editor
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

#ifndef _PROJDISTRIBUTORLOCALXML_H_
#define _PROJDISTRIBUTORLOCALXML_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/string/decString.h>

class projProject;

class decBaseFileReader;
class decBaseFileWriter;
class decXmlWriter;
class decXmlElementTag;



/**
 * \brief Load/Save XML Project Local.
 */
class projProjectLocalXml : public igdeBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create xml project. */
	projProjectLocalXml( deLogger *logger, const char *loggerSource );
	
	/** \brief Clean up xml project. */
	virtual ~projProjectLocalXml();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Read configuration from xml file. */
	void ReadFromFile( decBaseFileReader &reader, projProject &project );
	
	/** \brief Write configuration to xml file. */
	void WriteToFile( decBaseFileWriter &writer, const projProject &project );
	/*@}*/
	
	
	
private:
	void pWriteProjectLocal( decXmlWriter &writer, const projProject &project );
	
	void pReadProjectLocal( const decXmlElementTag &root, projProject &project );
};

#endif
