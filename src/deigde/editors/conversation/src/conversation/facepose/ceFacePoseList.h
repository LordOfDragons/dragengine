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

#ifndef _CEFACEPOSELIST_H_
#define _CEFACEPOSELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceFacePose;



/**
 * \brief Face Pose List.
 */
class ceFacePoseList{
private:
	decObjectOrderedSet pFacePoses;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new face pose list. */
	ceFacePoseList();
	/** Cleans up the face pose list. */
	~ceFacePoseList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of face poses. */
	int GetCount() const;
	/** Retrieves the face pose at the given position. */
	ceFacePose *GetAt( int index ) const;
	/** Retrieves the named face pose or NULL if not found. */
	ceFacePose *GetNamed( const char *name ) const;
	/** Retrieves the index of a face pose or -1 if not found. */
	int IndexOf( ceFacePose *facePose ) const;
	/** Retrieves the index of the named face pose or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a face pose exists. */
	bool Has( ceFacePose *facePose ) const;
	/** Determines if a named face pose exists. */
	bool HasNamed( const char *name ) const;
	/** Adds a face pose. */
	void Add( ceFacePose *facePose );
	/** Removes a face pose. */
	void Remove( ceFacePose *facePose );
	/** Removes all face poses. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceFacePoseList &operator=( const ceFacePoseList &list );
	
	/** \brief Append face poses if absent. */
	ceFacePoseList &operator+=( const ceFacePoseList &list );
	/*@}*/
};

#endif
