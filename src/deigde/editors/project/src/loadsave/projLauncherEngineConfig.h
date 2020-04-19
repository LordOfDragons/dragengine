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

#ifndef _PROJLAUNCHERENGINECONFIG_H_
#define _PROJLAUNCHERENGINECONFIG_H_

#include <deigde/utils/igdeBaseXML.h>

class projTestRunner;
class projTRProfile;

class decBaseFileReader;
class decBaseFileWriter;
class decXmlWriter;
class decXmlElementTag;



/**
 * \brief Load launcher engine configuration.
 */
class projLauncherEngineConfig : public igdeBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create loader. */
	projLauncherEngineConfig( deLogger *logger, const char *loggerSource );
	
	/** \brief Clean up loader. */
	virtual ~projLauncherEngineConfig();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Read launcher engine config. */
	void ReadFromFile( decBaseFileReader &reader, projTestRunner &testRunner );
	/*@}*/
	
	
	
private:
	void pReadConfig( const decXmlElementTag &root, projTestRunner &testRunner );
	void pReadProfiles( const decXmlElementTag &root, projTestRunner &testRunner );
	void pReadProfile( const decXmlElementTag &root, projTestRunner &testRunner );
	void pReadProfileSystems( const decXmlElementTag &root, projTRProfile &profile );
	void pReadProfileModules( const decXmlElementTag &root, projTRProfile &profile );
	void pReadProfileModule( const decXmlElementTag &root, projTRProfile &profile );
	void pReadProfileModuleParameters( const decXmlElementTag &root,
		projTRProfile &profile, const char *module );
	void pReadProfileWindow( const decXmlElementTag &root, projTRProfile &profile );
};

#endif
