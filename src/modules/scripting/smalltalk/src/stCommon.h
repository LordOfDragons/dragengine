/* 
 * Drag[en]gine SmallTalk Script Module
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
#ifndef _STCOMMON_H_
#define _STCOMMON_H_

// includes
#include <stdint.h>
#include <gstpub.h>

// definitions

// Smalltalk provides a macro to populate a struct with the instance
// variables of the object class. now for the class side this does
// not exist. hence a new macro has to be created which provides this
// class side attachment. using a proper pseudo subclassing of structs
// makes though more sense hence for both cases a proper struct is
// provided which can be properly subclasses by any derived class
// struct.
struct csObject{
	OBJ_HEADER;
};

struct csClassObject : public csObject{
	OOP superclass;
	OOP subClasses;
	OOP methodDictionary;
	OOP instanceSpec;
	OOP instanceVariables;
	OOP name;
	OOP comment;
	OOP category;
	OOP environment;
	OOP classVariables;
	OOP sharedPools;
	OOP securityPolicy;
	OOP pragmaHandlers;
};



// end of include only once
#endif
