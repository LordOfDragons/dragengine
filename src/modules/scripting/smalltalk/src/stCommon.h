/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
