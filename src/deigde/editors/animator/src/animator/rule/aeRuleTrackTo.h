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

#ifndef _AERULETRACKTO_H_
#define _AERULETRACKTO_H_

#include "aeRule.h"
#include <dragengine/resources/animator/rule/deAnimatorRuleTrackTo.h>



/**
 * Animator rule track to.
 */
class aeRuleTrackTo : public aeRule{
public:
	typedef deTObjectReference<aeRuleTrackTo> Ref;
	
	
private:
	decString pTrackBone;
	deAnimatorRuleTrackTo::eTrackAxis pTrackAxis;
	deAnimatorRuleTrackTo::eTrackAxis pUpAxis;
	deAnimatorRuleTrackTo::eUpTarget pUpTarget;
	deAnimatorRuleTrackTo::eLockedAxis pLockedAxis;
	
	aeControllerTarget pTargetPosition;
	aeControllerTarget pTargetUp;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new track to rule. */
	aeRuleTrackTo();
	/** Create a copy of a track to rule. */
	aeRuleTrackTo(const aeRuleTrackTo &copy);
	/** Clean up the animator rule. */
	virtual ~aeRuleTrackTo();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the name of the track bone or empty string to use none. */
	inline const decString &GetTrackBone() const{return pTrackBone;}
	/** Set the name of the track bone or empty string to use none. */
	void SetTrackBone(const char *boneName);
	/** Retrieve the track axis. */
	inline deAnimatorRuleTrackTo::eTrackAxis GetTrackAxis() const{return pTrackAxis;}
	/** Set the track axis. */
	void SetTrackAxis(deAnimatorRuleTrackTo::eTrackAxis axis);
	/** Retrieve the up axis. */
	inline deAnimatorRuleTrackTo::eTrackAxis GetUpAxis() const{return pUpAxis;}
	/** Set the up axis. */
	void SetUpAxis(deAnimatorRuleTrackTo::eTrackAxis axis);
	/** Retrieve the up target. */
	inline deAnimatorRuleTrackTo::eUpTarget GetUpTarget() const{return pUpTarget;}
	/** Set the up target. */
	void SetUpTarget(deAnimatorRuleTrackTo::eUpTarget target);
	/** Retrieve the locked axis. */
	inline deAnimatorRuleTrackTo::eLockedAxis GetLockedAxis() const{return pLockedAxis;}
	/** Set the locked axis. */
	void SetLockedAxis(deAnimatorRuleTrackTo::eLockedAxis axis);
	
	/** Retrieve the position target. */
	inline aeControllerTarget &GetTargetPosition(){return pTargetPosition;}
	inline const aeControllerTarget &GetTargetPosition() const{return pTargetPosition;}
	
	/** Retrieve the up target. */
	inline aeControllerTarget &GetTargetUp(){return pTargetUp;}
	inline const aeControllerTarget &GetTargetUp() const{return pTargetUp;}
	
	/** Create an engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule();
	/** Update targets. */
	virtual void UpdateTargets();
	/** Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage(aeLink *link) const;
	/** Remove a link from all targets using it. */
	virtual void RemoveLinkFromTargets(aeLink *link);
	/** Remove all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** Create a copy of this rule. */
	virtual aeRule *CreateCopy() const;
	
	/** List all links of all rule targets. */
	virtual void ListLinks(aeLinkList& list);
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Copy another track to rule to this track to rule. */
	virtual aeRuleTrackTo &operator=(const aeRuleTrackTo &copy);
	/*@}*/
};

#endif
