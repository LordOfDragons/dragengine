/* 
 * Drag[en]gine OGG Vorbis Sound Module
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

#ifndef _DESOUNDOGG_H_
#define _DESOUNDOGG_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/sound/deBaseSoundModule.h>



/**
 * \brief OGG Vorbis sound module.
 */
class deSoundOGG : public deBaseSoundModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deSoundOGG( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deSoundOGG();
	/*@}*/
	
	
	
	/** \name Management */
	/** \brief Read sound information from file. */
	virtual void InitLoadSound( decBaseFileReader &reader, deBaseSoundInfo &info );
	
	/**
	 * \brief Save sound.
	 * 
	 * You can be sure that the file provided has been rewinded prior to this
	 * function call.
	 */
	virtual void SaveSound( decBaseFileWriter &writer, const deSound &sound );
	
	/** \brief Create sound decoder peer. */
	virtual deBaseSoundDecoder *CreateDecoder( decBaseFileReader *reader );
	/*@}*/
};

#endif
