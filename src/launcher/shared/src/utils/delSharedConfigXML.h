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

#ifndef _DELSHAREDCONFIGXML_H_
#define _DELSHAREDCONFIGXML_H_

#include "delBaseXML.h"

class delGameProfile;
class delGPModule;


/**
 * \brief Shared Load/Save Config XML.
 */
class delSharedConfigXML : public delBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shared config xml read/save. */
	delSharedConfigXML( deLogger *logger, const char *loggingSource );
	
	/** \brief Clean up shared config xml read/save. */
	virtual ~delSharedConfigXML();
	/*@}*/
	
	
	
protected:
	void WriteProfile( decXmlWriter &writer, const delGameProfile &profile, const char *tagName = "profile" );
	void WriteProfileSystems( decXmlWriter &writer, const delGameProfile &profile );
	void WriteProfileDisableModuleVersions( decXmlWriter &writer, const delGameProfile &profile );
	void WriteProfileModules( decXmlWriter &writer, const delGameProfile &profile );
	void WriteProfileModule( decXmlWriter &writer, const delGPModule &module );
	void WriteProfileModuleParameters( decXmlWriter &writer, const delGPModule &module );
	void WriteProfileWindow( decXmlWriter &writer, const delGameProfile &profile );
	
	void ReadProfile( const decXmlElementTag &root, delGameProfile &profile );
	void ReadProfileSystems( const decXmlElementTag &root, delGameProfile &profile );
	void ReadProfileDisableModuleVersions( const decXmlElementTag &root, delGameProfile &profile );
	void ReadProfileModules( const decXmlElementTag &root, delGameProfile &profile );
	void ReadProfileModule( const decXmlElementTag &root, delGameProfile &profile );
	void ReadProfileModuleParameters( const decXmlElementTag &root, delGPModule &module );
	void ReadProfileWindow( const decXmlElementTag &root, delGameProfile &profile );
};

#endif
