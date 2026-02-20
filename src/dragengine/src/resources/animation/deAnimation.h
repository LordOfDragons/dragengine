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

#ifndef _DEANIMATION_H_
#define _DEANIMATION_H_

#include "deAnimationBone.h"
#include "deAnimationMove.h"

#include "../deFileResource.h"
#include "../../common/string/decStringList.h"
#include "../../common/collection/decTUniqueList.h"

class deBaseAnimatorAnimation;
class deEngine;
class deAnimationManager;
class deAnimationVertexPositionSet;
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
class DE_DLL_EXPORT deAnimation : public deFileResource{
public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<deAnimation>;
	
	
private:
	deAnimationBone::List pBones;
	
	decStringList pVertexPositionSets;
	
	deAnimationMove::List pMoves;
	
	deBaseAnimatorAnimation *pPeerAnimator;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animation resource with the given resource manager and filename. */
	deAnimation(deAnimationManager *resourceManager, deVirtualFileSystem *vfs,
		const char *filename, TIME_SYSTEM modificationTime);
	
protected:
	/**
	 * \brief Clean up resource.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deAnimation() override;
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/**
	 * \brief Bones of an animation match to those of a model.
	 * \brief This function is depracted and will be removed soon.
	 */
	bool MatchesModel(deModel *model) const;
	/*@}*/
	
	
	
	/** \name Bone Management */
	/*@{*/
	/** \brief Bones. */
	inline const deAnimationBone::List &GetBones() const{ return pBones; }
	
	/** \brief Count of bones. */
	inline int GetBoneCount() const{ return pBones.GetCount(); }
	
	/** \brief Bone at the given index. */
	const deAnimationBone::Ref &GetBone(int index) const;
	
	/** \brief Index for the bone with the given name or -1 if not found. */
	int FindBone(const char *name) const;
	
	/** \brief Adds a new bone */
	void AddBone(deAnimationBone::Ref &&bone);
	/*@}*/
	
	
	
	/**
	 * \brief Vertex position sets.
	 * \version 1.17
	 */
	inline decStringList &GetVertexPositionSets(){ return pVertexPositionSets; }
	inline const decStringList &GetVertexPositionSets() const{ return pVertexPositionSets; }
	/*@}*/
	
	
	
	/** \name Move Management */
	/*@{*/
	/** \brief Moves. */
	inline const deAnimationMove::List &GetMoves() const{ return pMoves; }
	
	/** \brief Count of moves. */
	inline int GetMoveCount() const{ return pMoves.GetCount(); }
	
	/** \brief Move at the given index. */
	const deAnimationMove::Ref &GetMove(int index) const;
	
	/** \brief Index of the move with the given name or -1 if not found. */
	int FindMove(const char *name) const;
	
	/** \brief Adds the given move */
	void AddMove(deAnimationMove::Ref &&move);
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Animator system peer object. */
	inline deBaseAnimatorAnimation *GetPeerAnimator() const{ return pPeerAnimator; }
	
	/** \brief Set animator system peer object. */
	void SetPeerAnimator(deBaseAnimatorAnimation *peer);
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
