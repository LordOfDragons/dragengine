/* 
 * Drag[en]gine IGDE World Editor
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

// include only once
#ifndef _MEFILTEROBJECTS_H_
#define _MEFILTEROBJECTS_H_

// includes

// predefinitions
class meObject;



/**
 * @brief Filter Objects.
 *
 * Base class for object filters. An object filter checks if objects
 * are accepted for further processing or dropped. This class is
 * rather generic in that any kind of process can simply ask an
 * instance of this class if a given object is accepted or rejected.
 */
class meFilterObjects{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new filter. */
	meFilterObjects();
	/** Cleans up the filter. */
	virtual ~meFilterObjects();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/**
	 * Determines if the given object is accepted by the filter.
	 * The default implementation accepts all objects.
	 */
	virtual bool AcceptObject( meObject *object ) const;
	/*@}*/
};

// end of include only once
#endif
