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

#ifndef _DEGLSHAREDCONFIGXML_H_
#define _DEGLSHAREDCONFIGXML_H_

#include "deglBaseXML.h"

class deglGameProfile;
class deglGPModule;



/**
 * \brief Shared Load/Save Config XML.
 */
class deglSharedConfigXML : public deglBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shared config xml read/save. */
	deglSharedConfigXML( deLogger *logger, const char *loggingSource );
	
	/** \brief Clean up shared config xml read/save. */
	virtual ~deglSharedConfigXML();
	/*@}*/
	
	
	
protected:
	void pWriteProfile( decXmlWriter &writer, const deglGameProfile &profile, const char *tagName = "profile" );
	void pWriteProfileSystems( decXmlWriter &writer, const deglGameProfile &profile );
	void pWriteProfileDisableModuleVersions( decXmlWriter &writer, const deglGameProfile &profile );
	void pWriteProfileModules( decXmlWriter &writer, const deglGameProfile &profile );
	void pWriteProfileModule( decXmlWriter &writer, const deglGPModule &module );
	void pWriteProfileModuleParameters( decXmlWriter &writer, const deglGPModule &module );
	void pWriteProfileWindow( decXmlWriter &writer, const deglGameProfile &profile );
	
	void pReadProfile( const decXmlElementTag &root, deglGameProfile &profile );
	void pReadProfileSystems( const decXmlElementTag &root, deglGameProfile &profile );
	void pReadProfileDisableModuleVersions( const decXmlElementTag &root, deglGameProfile &profile );
	void pReadProfileModules( const decXmlElementTag &root, deglGameProfile &profile );
	void pReadProfileModule( const decXmlElementTag &root, deglGameProfile &profile );
	void pReadProfileModuleParameters( const decXmlElementTag &root, deglGPModule &module );
	void pReadProfileWindow( const decXmlElementTag &root, deglGameProfile &profile );
};

#endif
