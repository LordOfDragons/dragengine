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

#ifndef _DEALENGINECONFIGXML_H_
#define _DEALENGINECONFIGXML_H_

#include "../dealBaseXML.h"
#include "../common/string/decString.h"

class dealLauncher;
class dealGameProfile;
class dealGPModule;
class decBaseFileReader;
class decBaseFileWriter;



/**
 * \brief Load/Save Engine Config XML.
 */
class dealEngineConfigXML : public dealBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine config xml read/save. */
	dealEngineConfigXML( deLogger *logger, const char *loggerSource );
	
	/** \brief Clean up engine config xml read/save. */
	virtual ~dealEngineConfigXML();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Read from XML file. */
	void ReadFromFile( decBaseFileReader &reader, dealLauncher &launcher );
	
	/** \brief Write to XML file. */
	void WriteToFile( decBaseFileWriter &writer, const dealLauncher &launcher );
	/*@}*/
	
	
	
private:
	void pWriteConfig( decXmlWriter &writer, const dealLauncher &launcher );
	void pWriteProfiles( decXmlWriter &writer, const dealLauncher &launcher );
	void pWriteProfile( decXmlWriter &writer, const dealGameProfile &profile );
	void pWriteProfileSystems( decXmlWriter &writer, const dealGameProfile &profile );
	void pWriteProfileDisableModuleVersions( decXmlWriter &writer, const dealGameProfile &profile );
	void pWriteProfileModules( decXmlWriter &writer, const dealGameProfile &profile );
	void pWriteProfileModule( decXmlWriter &writer, const dealGPModule &module );
	void pWriteProfileModuleParameters( decXmlWriter &writer, const dealGPModule &module );
	void pWriteProfileWindow( decXmlWriter &writer, const dealGameProfile &profile );
	
	void pReadConfig( const decXmlElementTag &root, dealLauncher &launcher );
	void pReadProfiles( const decXmlElementTag &root, dealLauncher &launcher );
	void pReadProfile( const decXmlElementTag &root, dealLauncher &launcher );
	void pReadProfileSystems( const decXmlElementTag &root, dealGameProfile &profile );
	void pReadProfileDisableModuleVersions( const decXmlElementTag &root, dealGameProfile &profile );
	void pReadProfileModules( const decXmlElementTag &root, dealGameProfile &profile );
	void pReadProfileModule( const decXmlElementTag &root, dealGameProfile &profile );
	void pReadProfileModuleParameters( const decXmlElementTag &root, dealGPModule &module );
	void pReadProfileWindow( const decXmlElementTag &root, dealGameProfile &profile );
};

#endif
