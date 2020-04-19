/* 
 * Drag[en]gine DELGA Archive Module
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

#ifndef _DEARCHIVEDELGA_H_
#define _DEARCHIVEDELGA_H_

#include <dragengine/systems/modules/archive/deBaseArchiveModule.h>



/**
 * \brief DELGA archive module.
 */
class deArchiveDelga : public deBaseArchiveModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deArchiveDelga( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deArchiveDelga();
	/*@}*/
	
	
	
	/*@{*/
	/** \name Management */
	/** \brief Create container peer. */
	virtual deBaseArchiveContainer *CreateContainer( decBaseFileReader *reader );
	/*@}*/
};

#endif
