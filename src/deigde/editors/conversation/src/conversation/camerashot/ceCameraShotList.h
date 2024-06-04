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

#ifndef _CECAMERASHOTLIST_H_
#define _CECAMERASHOTLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceCameraShot;



/**
 * \brief Camera Shot List.
 */
class ceCameraShotList{
private:
	decObjectOrderedSet pShots;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new camera shot list. */
	ceCameraShotList();
	/** Cleans up the camera shot list. */
	~ceCameraShotList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of camera shots. */
	int GetCount() const;
	/** Retrieves the camera shot at the given position. */
	ceCameraShot *GetAt( int index ) const;
	/** Retrieves the named camera shot or NULL if not found. */
	ceCameraShot *GetNamed( const char *name ) const;
	/** Retrieves the index of a camera shot or -1 if not found. */
	int IndexOf( ceCameraShot *cameraShot ) const;
	/** Retrieves the index of the named camera shot or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a camera shot exists. */
	bool Has( ceCameraShot *cameraShot ) const;
	/** Determines if a named camera shot exists. */
	bool HasNamed( const char *name ) const;
	/** Adds a camera shot. */
	void Add( ceCameraShot *cameraShot );
	/** Removes a camera shot. */
	void Remove( ceCameraShot *cameraShot );
	/** Removes all camera shots. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceCameraShotList &operator=( const ceCameraShotList &list );
	
	/** \brief Append camera shots if absent. */
	ceCameraShotList &operator+=( const ceCameraShotList &list );
	/*@}*/
};

#endif
