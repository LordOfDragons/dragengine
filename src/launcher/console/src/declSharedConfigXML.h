/* 
 * Drag[en]gine GUI Launcher
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

#ifndef _DECLSHAREDCONFIGXML_H_
#define _DECLSHAREDCONFIGXML_H_

#include "declBaseXML.h"

class declGameProfile;
class declGPModule;



/**
 * \brief Shared Load/Save Config XML.
 */
class declSharedConfigXML : public declBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shared config xml read/save. */
	declSharedConfigXML( deLogger *logger, const char *loggingSource );
	
	/** \brief Clean up shared config xml read/save. */
	virtual ~declSharedConfigXML();
	/*@}*/
	
	
	
protected:
	void pWriteProfile( decXmlWriter &writer, const declGameProfile &profile, const char *tagName = "profile" );
	void pWriteProfileSystems( decXmlWriter &writer, const declGameProfile &profile );
	void pWriteProfileDisableModuleVersions( decXmlWriter &writer, const declGameProfile &profile );
	void pWriteProfileModules( decXmlWriter &writer, const declGameProfile &profile );
	void pWriteProfileModule( decXmlWriter &writer, const declGPModule &module );
	void pWriteProfileModuleParameters( decXmlWriter &writer, const declGPModule &module );
	void pWriteProfileWindow( decXmlWriter &writer, const declGameProfile &profile );
	
	void pReadProfile( const decXmlElementTag &root, declGameProfile &profile );
	void pReadProfileSystems( const decXmlElementTag &root, declGameProfile &profile );
	void pReadProfileDisableModuleVersions( const decXmlElementTag &root, declGameProfile &profile );
	void pReadProfileModules( const decXmlElementTag &root, declGameProfile &profile );
	void pReadProfileModule( const decXmlElementTag &root, declGameProfile &profile );
	void pReadProfileModuleParameters( const decXmlElementTag &root, declGPModule &module );
	void pReadProfileWindow( const decXmlElementTag &root, declGameProfile &profile );
};

#endif
