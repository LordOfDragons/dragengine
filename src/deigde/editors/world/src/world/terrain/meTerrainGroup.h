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
#ifndef _METERRAINGROUP_H_
#define _METERRAINGROUP_H_

#include <dragengine/common/string/decString.h>

// predefinitions
class meObject;


// class meTerrainGroup
class meTerrainGroup{
private:
	decString pName;
	meObject **pObjects;
	int pObjectCount, pObjectSize;
public:
	// constructor, destructor
	meTerrainGroup( const char *name );
	~meTerrainGroup();
	// management
	inline const decString &GetName() const{ return pName; }
	void SetName( const char *name );
	// object management
	inline int GetObjectCount() const{ return pObjectCount; }
	meObject *GetObject( int index ) const;
	int FindObject( meObject *object ) const;
	bool HasObject( meObject *object ) const;
	void AddObject( meObject *object );
	void RemoveObject( meObject *object );
	void RemoveAllObjects();
private:
	void pCleanUp();
};

// end of include only once
#endif
