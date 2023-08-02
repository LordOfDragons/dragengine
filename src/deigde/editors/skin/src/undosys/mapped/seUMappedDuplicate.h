/* 
 * Drag[en]gine IGDE Skin Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _SEUMAPPEDDUPLICATE_H_
#define _SEUMAPPEDDUPLICATE_H_

#include "seUMappedAdd.h"


/**
 * Undo action duplicate mapped.
 */
class seUMappedDuplicate : public seUMappedAdd{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo. */
	seUMappedDuplicate( seSkin *skin, const seMapped &mapped );
	/*@}*/
};

#endif
