/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEANIMATION_H_
#define _DEANIMATION_H_

#include "../deFileResource.h"

class deBaseAnimatorAnimation;
class deEngine;
class deAnimationManager;
class deAnimationBone;
class deAnimationMove;
class deModel;
class deBaseAnimatorAnimation;


/**
 * \brief Animation Resource.
 * 
 * Animation resources contain the states of bones for animation
 * purpose. The animation resource contains a list of moves which
 * describe the single movement of a riged object. Each move
 * contains a keyframe list for each bone of the animation. Those
 * keyframes contain the state of a bone in a particular point of
 * time. This information can be used by the engine and the game
 * scripts to derive the bone position for an arbitrary point in
 * time. The bones of an animation have to be mapped first to
 * the bones of a rig.
 */
class deAnimation : public deFileResource{
private:
	deAnimationBone **pBones;
	deAnimationMove **pMoves;
	int pBoneCount, pBoneSize;
	int pMoveCount, pMoveSize;
	
	deBaseAnimatorAnimation *pPeerAnimator;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animation resource with the given resource manager and filename. */
	deAnimation( deAnimationManager *resourceManager, deVirtualFileSystem *vfs,
		const char *filename, TIME_SYSTEM modificationTime );
	
protected:
	/**
	 * \brief Clean up resource.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deAnimation();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/**
	 * \brief Bones of an animation match to those of a model.
	 * \brief This function is depracted and will be removed soon.
	 */
	bool MatchesModel( deModel *model ) const;
	/*@}*/
	
	
	
	/** \name Bone Management */
	/*@{*/
	/** \brief Count of bones. */
	inline int GetBoneCount() const{ return pBoneCount; }
	
	/** \brief Bone at the given index. */
	deAnimationBone *GetBone( int index ) const;
	
	/** \brief Index for the bone with the given name or -1 if not found. */
	int FindBone( const char *name ) const;
	
	/** \brief Adds a new bone */
	void AddBone( deAnimationBone *bone );
	/*@}*/
	
	
	
	/** \name Move Management */
	/*@{*/
	/** \brief Count of moves. */
	inline int GetMoveCount() const{ return pMoveCount; }
	
	/** \brief Move at the given index. */
	deAnimationMove *GetMove( int index ) const;
	
	/** \brief Index of the move with the given name or -1 if not found. */
	int FindMove( const char *name ) const;
	
	/** \brief Adds the given move */
	void AddMove( deAnimationMove *move );
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Animator system peer object. */
	inline deBaseAnimatorAnimation *GetPeerAnimator() const{ return pPeerAnimator; }
	
	/** \brief Set animator system peer object. */
	void SetPeerAnimator( deBaseAnimatorAnimation *peer );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
