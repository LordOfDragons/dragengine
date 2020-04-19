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

#ifndef _DEALCONFIGXML_H_
#define _DEALCONFIGXML_H_

#include "../dealBaseXML.h"

#include "../common/string/decString.h"

class dealConfigWindow;
class decBaseFileReader;
class decBaseFileWriter;
class dealConfiguration;
class decXmlWriter;
class decXmlElementTag;



/**
 * \brief Load/Save Configuration XML.
 */
class dealConfigXML : public dealBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create configuration xml read/save. */
	dealConfigXML( deLogger *logger, const char *loggerSource );
	
	/** \brief Clean up configuration xml read/save. */
	virtual ~dealConfigXML();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Read from XML file. */
	void ReadFromFile( decBaseFileReader &reader, dealConfiguration &config );
	
	/** \brief Write to XML file. */
	void WriteToFile( decBaseFileWriter &writer, const dealConfiguration &config );
	/*@}*/
	
	
	
private:
	void pWriteConfig( decXmlWriter &writer, const dealConfiguration &config );
	void pWriteWindow( decXmlWriter &writer, const dealConfigWindow &window, const char *tagName );
	
	void pReadConfig( const decXmlElementTag &root, dealConfiguration &config );
	void pReadWindow( const decXmlElementTag &root, dealConfigWindow &window );
};

#endif
