/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#ifndef _SECLIPBOARDDATASOURCE_H_
#define _SECLIPBOARDDATASOURCE_H_

#include "../synthesizer/source/seSourceList.h"

#include <deigde/clipboard/igdeClipboardData.h>



/**
 * \brief Source clipboard data.
 */
class seClipboardDataSource : public igdeClipboardData{
public:
	/** \brief Type name. */
	static const char * const TYPE_NAME;
	
	
	
private:
	seSourceList pSources;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create clipboard data. */
	seClipboardDataSource( seSource *source );
	
	/** \brief Create clipboard data. */
	seClipboardDataSource( const seSourceList &sources );
	
protected:
	/**
	 * \brief Clean up object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	/** \brief Clean up clipboard data. */
	virtual ~seClipboardDataSource();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Sources. */
	inline const seSourceList &GetSources() const{ return pSources; }
	/*@}*/
};

#endif
