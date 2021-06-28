/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEIGDEREALAPPLICATION_H_
#define _IGDEIGDEREALAPPLICATION_H_

#include "../environment/igdeEnvironmentIGDE.h"

#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeWidgetReference.h>


/**
 * \brief Create DEIGDE application.
 */
class igdeRealApplication : public igdeApplication{
private:
	igdeEnvironmentIGDE pEnvironment;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create DEIGDE application. */
	igdeRealApplication();
	
	/** \brief Clean up DEIGDE application. */
	virtual ~igdeRealApplication();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironmentIGDE &GetEnvironment(){ return pEnvironment; }
	inline const igdeEnvironmentIGDE &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Main window. */
	igdeWindowMain &GetWindowMain() const;
	
	
	
protected:
	/**
	 * \brief Initialize application.
	 * \returns true to run application or false to abord.
	 */
	virtual bool Initialize( const decUnicodeStringList &arguments );
	
	/** \brief Clean up application. */
	virtual void CleanUp();
};

#endif
