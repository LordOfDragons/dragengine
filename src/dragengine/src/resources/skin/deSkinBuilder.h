/* 
 * Drag[en]gine Game Engine
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

#ifndef _DESKINBUILDER_H_
#define _DESKINBUILDER_H_

#include "../../dragengine_export.h"

class deSkin;


/**
 * \brief Skin Builder.
 * 
 * The skin builder is provided for applications requiring to produce
 * skin resources not from a file but from data somewhere in memory.
 * Usually used by editors to create skin resources from an in-editor
 * representation of a skin. The default implementation creates an
 * empty skin.
 */
class DE_DLL_EXPORT deSkinBuilder{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new skin builder. */
	deSkinBuilder();
	
	/** \brief Clean up skin builder. */
	virtual ~deSkinBuilder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Build skin into the provided skin object. */
	virtual void BuildSkin( deSkin *skin );
	/*@}*/
};

#endif
