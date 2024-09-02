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

#ifndef _CEACTORPOSELIST_H_
#define _CEACTORPOSELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceActorPose;



/**
 * \brief Actor pose list.
 */
class ceActorPoseList{
private:
	decObjectOrderedSet pGestures;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create pose list. */
	ceActorPoseList();
	
	/** \brief Create pose list. */
	ceActorPoseList( const ceActorPoseList &pose );
	
	/** \brief Clean up pose list. */
	~ceActorPoseList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of poses. */
	int GetCount() const;
	
	/** \brief Gesture at index. */
	ceActorPose *GetAt( int index ) const;
	
	/** \brief Named pose or \em NULL if absent. */
	ceActorPose *GetNamed( const char *name ) const;
	
	/** \brief Index of pose or -1 if absent. */
	int IndexOf( ceActorPose *pose ) const;
	
	/** \brief Index of named pose or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Gesture is present. */
	bool Has( ceActorPose *pose ) const;
	
	/** \brief Named pose is present. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Add pose. */
	void Add( ceActorPose *pose );
	
	/** \brief Remove pose. */
	void Remove( ceActorPose *pose );
	
	/** \brief Remove all poses. */
	void RemoveAll();
	
	/** \brief Set from another list. */
	ceActorPoseList &operator=( const ceActorPoseList &list );
	/*@}*/
};

#endif
