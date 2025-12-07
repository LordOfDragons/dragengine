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

#ifndef _AERULESTATEMANIPULATOR_H_
#define _AERULESTATEMANIPULATOR_H_

#include "aeRule.h"



/**
 * Animator rule state manipulator.
 */
class aeRuleStateManipulator : public aeRule{
public:
	typedef deTObjectReference<aeRuleStateManipulator> Ref;
	
	
private:
	decVector pMinPosition;
	decVector pMaxPosition;
	decVector pMinRotation;
	decVector pMaxRotation;
	decVector pMinSize;
	decVector pMaxSize;
	float pMinVertexPositionSet;
	float pMaxVertexPositionSet;
	bool pEnablePosition;
	bool pEnableRotation;
	bool pEnableSize;
	bool pEnableVertexPositionSet;
	
	aeControllerTarget pTargetPosition;
	aeControllerTarget pTargetRotation;
	aeControllerTarget pTargetSize;
	aeControllerTarget pTargetVertexPositionSet;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create rule. */
	aeRuleStateManipulator();
	
	/** Create copy of rule. */
	aeRuleStateManipulator(const aeRuleStateManipulator &copy);
	
	/** Clean up rule. */
	virtual ~aeRuleStateManipulator();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Minimum position. */
	inline const decVector &GetMinimumPosition() const{return pMinPosition;}
	
	/** Set minimum position. */
	void SetMinimumPosition(const decVector &position);
	
	/** Maximum position. */
	inline const decVector &GetMaximumPosition() const{return pMaxPosition;}
	
	/** Set maximum position. */
	void SetMaximumPosition(const decVector &position);
	
	/** Minimum rotation. */
	inline const decVector &GetMinimumRotation() const{return pMinRotation;}
	
	/** Set minimum rotation. */
	void SetMinimumRotation(const decVector &rotation);
	
	/** Maximum rotation. */
	inline const decVector &GetMaximumRotation() const{return pMaxRotation;}
	
	/** Set maximum rotation. */
	void SetMaximumRotation(const decVector &rotation);
	
	/** Minimum size. */
	inline const decVector &GetMinimumSize() const{return pMinSize;}
	
	/** Set minimum size. */
	void SetMinimumSize(const decVector &size);
	
	/** Maximum size. */
	inline const decVector &GetMaximumSize() const{return pMaxSize;}
	
	/** Set maximum size. */
	void SetMaximumSize(const decVector &size);
	
	/** Minimum vertex position set. */
	inline float GetMinimumVertexPositionSet() const{return pMinVertexPositionSet;}
	
	/** Set minimum vertex position set. */
	void SetMinimumVertexPositionSet(float weight);
	
	/** Maximum vertex position set. */
	inline float GetMaximumVertexPositionSet() const{return pMaxVertexPositionSet;}
	
	/** Set maximum vertex position set. */
	void SetMaximumVertexPositionSet(float weight);
	
	
	
	/** Position manipulation is enabled. */
	inline bool GetEnablePosition() const{return pEnablePosition;}
	
	/** Set if position manipulation is enabled. */
	void SetEnablePosition(bool enabled);
	
	/** Rotation manipulation is enabled. */
	inline bool GetEnableOrientation() const{return pEnableRotation;}
	
	/** Set if rotation manipulation is enabled. */
	void SetEnableRotation(bool enabled);
	
	/** Size manipulation is enabled. */
	inline bool GetEnableSize() const{return pEnableSize;}
	
	/** Set if size manipulation is enabled. */
	void SetEnableSize(bool enabled);
	
	/** Vertex position set manipulation is enabled. */
	inline bool GetEnableVertexPositionSet() const{return pEnableVertexPositionSet;}
	
	/** Set if vertex position set manipulation is enabled. */
	void SetEnableVertexPositionSet(bool enabled);
	
	
	
	/** Position target. */
	inline aeControllerTarget &GetTargetPosition(){return pTargetPosition;}
	inline const aeControllerTarget &GetTargetPosition() const{return pTargetPosition;}
	
	/** Rotation target. */
	inline aeControllerTarget &GetTargetRotation(){return pTargetRotation;}
	inline const aeControllerTarget &GetTargetRotation() const{return pTargetRotation;}
	
	/** Size target. */
	inline aeControllerTarget &GetTargetSize(){return pTargetSize;}
	inline const aeControllerTarget &GetTargetSize() const{return pTargetSize;}
	
	/** Vertex position set target. */
	inline aeControllerTarget &GetTargetVertexPositionSet(){return pTargetVertexPositionSet;}
	inline const aeControllerTarget &GetTargetVertexPositionSet() const{return pTargetVertexPositionSet;}
	
	
	
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
	/** Copy another state manipulator rule to this state manipulator rule. */
	virtual aeRuleStateManipulator &operator=(const aeRuleStateManipulator &copy);
	/*@}*/
};

#endif
