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

#ifndef _DEANIMATIONMOVE_H_
#define _DEANIMATIONMOVE_H_

#include "../../common/string/decString.h"
#include "../../common/math/decMath.h"

class deAnimation;
class deAnimationKeyframeList;
class deAnimationKeyframeVertexPositionSetList;


/**
 * \brief Animation Move.
 *
 */
class DE_DLL_EXPORT deAnimationMove{
private:
	decString pName;
	float pPlaytime;
	float pFPS;
	
	deAnimationKeyframeList **pLists;
	int pListCount;
	int pListSize;
	
	deAnimationKeyframeVertexPositionSetList **pVertexPositionSetLists;
	int pVertexPositionSetListCount;
	int pVertexPositionSetListSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animation move without name. */
	deAnimationMove();
	
	/** \brief Clean up animation move. */
	~deAnimationMove();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name of the move. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name of the move */
	void SetName( const char *name );
	
	/** \brief Playtime of the move which is the largest time of all keyframes. */
	inline float GetPlaytime() const{ return pPlaytime; }
	
	/** \brief Set playtime for the move which is the largest time of all keyframes. */
	void SetPlaytime( float playtime );
	
	/**
	 * \brief Frames per second.
	 * \version 1.6
	 * 
	 * Used for saving only if animation file format requires sampling.
	 */
	inline float GetFPS() const{ return pFPS; }
	
	/**
	 * \brief Set frames per second.
	 * \version 1.6
	 * 
	 * Used for saving only if animation file format requires sampling.
	 */
	void SetFPS( float fps );
	
	/** \brief Count of keyframe lists. */
	inline int GetKeyframeListCount() const{ return pListCount; }
	
	/** \brief Keyframe at the given index. */
	deAnimationKeyframeList *GetKeyframeList( int index ) const;
	
	/** \brief Adds a keyframe list */
	void AddKeyframeList( deAnimationKeyframeList *list );
	
	/**
	 * \brief Count of keyframe lists.
	 * \version 1.17
	 */
	inline int GetVertexPositionSetKeyframeListCount() const{ return pVertexPositionSetListCount; }
	
	/**
	 * \brief Keyframe at the given index.
	 * \version 1.17
	 */
	deAnimationKeyframeVertexPositionSetList *GetVertexPositionSetKeyframeList( int index ) const;
	
	/**
	 * \brief Adds a keyframe list.
	 * \version 1.17
	 */
	void AddVertexPositionSetKeyframeList( deAnimationKeyframeVertexPositionSetList *list );
	/*@}*/
};

#endif
