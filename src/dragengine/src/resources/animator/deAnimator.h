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

#ifndef _DEANIMATOR_H_
#define _DEANIMATOR_H_

#include "../deResource.h"
#include "../animation/deAnimation.h"
#include "../rig/deRig.h"
#include "../../common/math/decMath.h"
#include "../../common/string/decStringSet.h"
#include "../../common/collection/decObjectOrderedSet.h"

class deAnimatorRule;
class deAnimatorLink;
class deAnimatorManager;
class deAnimatorController;
class deBaseAnimatorAnimator;


/**
 * \brief Animator Class.
 * 
 * Animators are objects capable of producing an animation state for a
 * component. Every animator composes of a set of rules and controllers
 * which indicate how the final animation state is produced from a set
 * animation. Some rules can also produce animations without the help of
 * the animation but external information like for example where another
 * object is located or a reference state captured before. Controllers provide
 * the values required by rules and one controller can be attached to more
 * than one rule. The animator object has peers into the animator module.
 * This way the creation of animations can be accelerated if possible.
 */
class DE_DLL_EXPORT deAnimator : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deAnimator> Ref;
	
	
private:
	deRig::Ref pRig;
	deAnimation::Ref pAnimation;
	
	deAnimatorController **pControllers;
	int pControllerCount;
	int pControllerSize;
	
	deAnimatorLink **pLinks;
	int pLinkCount;
	int pLinkSize;
	
	decObjectOrderedSet pRules;
	
	decStringSet pListBones;
	decStringSet pListVertexPositionSets;
	
	deBaseAnimatorAnimator *pPeerAnimator;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create animator. */
	deAnimator( deAnimatorManager *manager );
	
protected:
	/**
	 * \brief Clean up animator.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deAnimator();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Rig or NULL. */
	inline deRig *GetRig() const{ return pRig; }
	
	/** \brief Set rig or NULL. */
	void SetRig( deRig *rig );
	
	/** \brief Animation or NULL. */
	inline deAnimation *GetAnimation() const{ return pAnimation; }
	
	/** \brief Set animation or NULL. */
	void SetAnimation( deAnimation *animation );
	
	/** \brief Bones. */
	inline decStringSet &GetListBones(){ return pListBones; }
	inline const decStringSet &GetListBones() const{ return pListBones; }
	
	/** \brief Notify peers list of bones changed. */
	void NotifyBonesChanged();
	
	/** \brief Vertex position sets. */
	inline decStringSet &GetListVertexPositionSets(){ return pListVertexPositionSets; }
	inline const decStringSet &GetListVertexPositionSets() const{ return pListVertexPositionSets; }
	
	/** \brief Notify peers list of vertex position sets changed. */
	void NotifyVertexPositionSetsChanged();
	/*@}*/
	
	
	
	/** \name Controller Management */
	/*@{*/
	/** \brief Count of controllers. */
	inline int GetControllerCount() const{ return pControllerCount; }
	
	/** \brief Controller at index. */
	deAnimatorController *GetControllerAt( int index ) const;
	
	/** \brief Index of controller or -1 if absent. */
	int IndexOfController( deAnimatorController *controller ) const;
	
	/** \brief Index of named controller or -1 if absent. */
	int IndexOfControllerNamed( const char *controller ) const;
	
	/** \brief Controller is present. */
	bool HasController( deAnimatorController *controller ) const;
	
	/** \brief Add controller. */
	void AddController( deAnimatorController *controller );
	
	/** \brief Remove controller. */
	void RemoveController( deAnimatorController *controller );
	
	/** \brief Remove all controllers. */
	void RemoveAllControllers();
	
	/** \brief Notify peers controller changed. */
	void NotifyControllerChangedAt( int index );
	/*@}*/
	
	
	
	/** \name Link Management */
	/*@{*/
	/** \brief Count of links. */
	inline int GetLinkCount() const{ return pLinkCount; }
	
	/** \brief Link at index. */
	deAnimatorLink *GetLinkAt( int index ) const;
	
	/** \brief Index of link or -1 if absent. */
	int IndexOfLink( deAnimatorLink *link ) const;
	
	/** \brief Link is present. */
	bool HasLink( deAnimatorLink *link ) const;
	
	/** \brief Add link. */
	void AddLink( deAnimatorLink *link );
	
	/** \brief Remove link. */
	void RemoveLink( deAnimatorLink *link );
	
	/** \brief Remove all links. */
	void RemoveAllLinks();
	
	/** \brief Notify peers link changed. */
	void NotifyLinkChangedAt( int index );
	/*@}*/
	
	
	
	/** \name Rule Management */
	/*@{*/
	/** \brief Count of rules. */
	int GetRuleCount() const;
	
	/** \brief Rule at index. */
	deAnimatorRule *GetRuleAt( int index ) const;
	
	/** \brief Index of rule or -1 if absent. */
	int IndexOfRule( deAnimatorRule *rule ) const;
	
	/** \brief Rule is present. */
	bool HasRule( deAnimatorRule *rule ) const;
	
	/** \brief Add rule. */
	void AddRule( deAnimatorRule *rule );
	
	/** \brief Remove rule. */
	void RemoveRule( deAnimatorRule *rule );
	
	/** \brief Remove all rules. */
	void RemoveAllRules();
	
	/** \brief Notify peers one or more rules changed. */
	void NotifyRulesChanged();
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Animator system peer. */
	inline deBaseAnimatorAnimator *GetPeerAnimator() const{ return pPeerAnimator; }
	
	/** \brief Set animator system peer. */
	void SetPeerAnimator( deBaseAnimatorAnimator *peer );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
