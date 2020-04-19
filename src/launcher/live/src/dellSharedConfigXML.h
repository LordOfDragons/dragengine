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

#ifndef _DELLSHAREDCONFIGXML_H_
#define _DELLSHAREDCONFIGXML_H_

#include "dellBaseXML.h"

class dellGameProfile;
class dellGPModule;



/**
 * \brief Shared Load/Save Config XML.
 */
class dellSharedConfigXML : public dellBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shared config xml read/save. */
	dellSharedConfigXML( deLogger *logger, const char *loggingSource );
	
	/** \brief Clean up shared config xml read/save. */
	virtual ~dellSharedConfigXML();
	/*@}*/
	
	
	
protected:
	void pWriteProfile( decXmlWriter &writer, const dellGameProfile &profile, const char *tagName = "profile" );
	void pWriteProfileSystems( decXmlWriter &writer, const dellGameProfile &profile );
	void pWriteProfileDisableModuleVersions( decXmlWriter &writer, const dellGameProfile &profile );
	void pWriteProfileModules( decXmlWriter &writer, const dellGameProfile &profile );
	void pWriteProfileModule( decXmlWriter &writer, const dellGPModule &module );
	void pWriteProfileModuleParameters( decXmlWriter &writer, const dellGPModule &module );
	void pWriteProfileWindow( decXmlWriter &writer, const dellGameProfile &profile );
	
	void pReadProfile( const decXmlElementTag &root, dellGameProfile &profile );
	void pReadProfileSystems( const decXmlElementTag &root, dellGameProfile &profile );
	void pReadProfileDisableModuleVersions( const decXmlElementTag &root, dellGameProfile &profile );
	void pReadProfileModules( const decXmlElementTag &root, dellGameProfile &profile );
	void pReadProfileModule( const decXmlElementTag &root, dellGameProfile &profile );
	void pReadProfileModuleParameters( const decXmlElementTag &root, dellGPModule &module );
	void pReadProfileWindow( const decXmlElementTag &root, dellGameProfile &profile );
};

#endif
