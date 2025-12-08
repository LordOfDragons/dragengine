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

#ifndef _SPTYPEWORLD_H_
#define _SPTYPEWORLD_H_

#include "../spBaseType.h"

class deWorld;



/**
 * \brief Type World.
 */
class spTypeWorld : public spBaseType{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new module. */
	spTypeWorld(ScriptingPython &sp);
	/** \brief Cleans up the module. */
	~spTypeWorld() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the world from an object. */
	deWorld *WorldFromObject(PyObject *object);
	/** \brief Create an object from a world. */
	PyObject *ObjectFromWorld(deWorld::Ref world);
	
	/** Constructor. */
	virtual void Constructor(PyObject *myself, PyObject *args, PyObject *kwds);
	/** Destructor. */
	virtual void Destructor(PyObject *myself);
	/*@}*/
	
	/** \name Functions */
	/*@{*/
	static PyObject *cfUpdate(PyObject *myself, PyObject *args);
	/*@}*/
};

#endif
