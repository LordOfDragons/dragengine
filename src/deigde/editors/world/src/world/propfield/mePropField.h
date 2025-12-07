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
#ifndef _MEPROPFIELD_H_
#define _MEPROPFIELD_H_

// includes
#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/src/resources/propfield/dePropField.h>

// predefinitions
class mePropFieldType;
class meWorld;

class deEngine;



/**
 * @brief Prop Field.
 *
 * Defines a prop field.
 */
class mePropField{
private:
	deEngine *pEngine;
	
	meWorld *pWorld;
	dePropField::Ref pEngPF;
	
	decDVector pPosition;
	
	mePropFieldType **pTypes;
	int pTypeCount;
	int pTypeSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a object. */
	mePropField(deEngine *engine);
	/** Cleans up the object. */
	~mePropField();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the parent world or NULL. */
	inline meWorld *GetWorld() const{ return pWorld; }
	/** Sets the parent world or NULL. */
	void SetWorld(meWorld *world);
	
	/** Retrieves the engine height terrain. */
	inline const dePropField::Ref &GetEnginePropField() const{ return pEngPF; }
	
	/** Retrieves the position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	/** Sets the position. */
	void SetPosition(const decDVector &position);
	
	/** Sets the world changed. */
	void SetWorldChanged();
	/*@}*/
	
	/** \name Types */
	/*@{*/
	/** Retrieves the number of types. */
	inline int GetTypeCount() const{ return pTypeCount; }
	/** Retrieves the type at the given index. */
	mePropFieldType *GetTypeAt(int index) const;
	/** Retrieves the index of the given type or -1 if not found. */
	int IndexOfType(mePropFieldType *type) const;
	/** Adds a new type. */
	void AddType(mePropFieldType *type);
	/** Removes a type. */
	void RemoveType(mePropFieldType *type);
	/** Removes all types. */
	void RemoveAllTypes();
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
