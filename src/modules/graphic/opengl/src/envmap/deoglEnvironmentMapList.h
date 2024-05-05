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

#ifndef _DEOGLENVIRONMENTMAPLIST_H_
#define _DEOGLENVIRONMENTMAPLIST_H_

#include <dragengine/common/collection/decPointerList.h>

class deoglEnvironmentMap;



/**
 * List of pointers to environment maps. The ordering of the environment map in the list is
 * not defined and can change when environment maps are removed. This allows to remove
 * elements with constant time requirements.
 */
class deoglEnvironmentMapList{
private:
	decPointerList pEnvMaps;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create environment map list. */
	deoglEnvironmentMapList();
	
	/** Clean up environment map list. */
	~deoglEnvironmentMapList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of environment maps. */
	int GetCount() const;
	
	/** Index of environment map or -1 if absent. */
	int IndexOf( deoglEnvironmentMap *envmap ) const;
	
	/** Environment map is present. */
	bool Has( deoglEnvironmentMap *envmap ) const;
	
	/** Environment map at index. */
	deoglEnvironmentMap *GetAt( int index ) const;
	
	/** Add environment map. */
	void Add( deoglEnvironmentMap *envmap );
	
	/**
	 * Add environment map if absent.
	 * \returns true if added.
	 */
	bool AddIfMissing( deoglEnvironmentMap *envmap );
	
	/** Remove environment map. */
	void Remove( deoglEnvironmentMap *envmap );
	
	/**
	 * Remove environment map if present.
	 * \returns true if removed.
	 */
	bool RemoveIfExisting( deoglEnvironmentMap *envmap );
	
	/** Remove environment map from the index. */
	void RemoveFrom( int index );
	
	/** Removes all environment maps. */
	void RemoveAll();
	/*@}*/
};

#endif
