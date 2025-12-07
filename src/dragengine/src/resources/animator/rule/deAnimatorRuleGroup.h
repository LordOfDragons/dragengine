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

#ifndef _DEANIMATORRULEGROUP_H_
#define _DEANIMATORRULEGROUP_H_

#include "deAnimatorRule.h"
#include "../../../common/collection/decObjectOrderedSet.h"


/**
 * \brief Animator group rule.
 * 
 * The group rule stores a list of rules and applies them in
 * order like the animator itself but does not apply the
 * individual rules to the intermediate bone states but the
 * result of the entire group of rules. Usefule to compose a
 * state using various rules and to apply them as a whole.
 */
class DE_DLL_EXPORT deAnimatorRuleGroup : public deAnimatorRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deAnimatorRuleGroup> Ref;
	
	
public:
	/** \brief Application types. */
	enum eApplicationTypes{
		/**
		 * \brief Apply the result of all child rules.
		 * 
		 * Applies first all child rules one by one to a zero state. The result
		 * state is then applied as the group rule state. This is the best
		 * choice for building a complex animation state to apply as a whole.
		 * This is the default for newly created group rules.
		 */
		eatAll,
		
		/**
		 * \brief Use selection target to apply the blend between two selected rules.
		 * 
		 * The child rules are considered a list of animations states to blend
		 * between. They are mapped to the 0 to 1 range of the selection target
		 * mapping the first rule to the 0 selection value, all following rules
		 * evenly spaced across the range to the last rule mapping to the 1
		 * selection value. The mapping is controlled by the link curve applied
		 * to the selection target, This is the best choice for blending between
		 * a set of similar animation states using a controller.
		 */
		eatSelect,
	};
	
	
	
private:
	decObjectOrderedSet pRules;
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	bool pEnableVertexPositionSet;
	
	bool pUseCurrentState;
	eApplicationTypes pApplicationType;
	
	deAnimatorControllerTarget pTargetSelect;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animator rule. */
	deAnimatorRuleGroup();
	
protected:
	/**
	 * \brief Clean up animator rule.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deAnimatorRuleGroup();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Number of rules. */
	int GetRuleCount() const;
	
	/** \brief Rule at the given index. */
	deAnimatorRule *GetRuleAt(int index) const;
	
	/** \brief Index of the given rule or -1 if not found. */
	int IndexOfRule(deAnimatorRule *rule) const;
	
	/** \brief Given rule exists. */
	bool HasRule(deAnimatorRule *rule) const;
	
	/** \brief Adds a rule. */
	void AddRule(deAnimatorRule *rule);
	
	/** \brief Removes the given rule. */
	void RemoveRule(deAnimatorRule *rule);
	
	/** \brief Removes all rules. */
	void RemoveAllRules();
	
	/** \brief Determines if position manipulation is enabled. */
	inline bool GetEnablePosition() const{return pEnablePosition;}
	
	/** \brief Set if position manipulation is enabled. */
	void SetEnablePosition(bool enabled);
	
	/** \brief Determines if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{return pEnableOrientation;}
	
	/** \brief Set if orientation manipulation is enabled. */
	void SetEnableOrientation(bool enabled);
	
	/** \brief Determines if size manipulation is enabled. */
	inline bool GetEnableSize() const{return pEnableSize;}
	
	/** \brief Set if size manipulation is enabled. */
	void SetEnableSize(bool enabled);
	
	/** \brief Vertex position sets are enabled. */
	inline bool GetEnableVertexPositionSet() const{return pEnableVertexPositionSet;}
	
	/** \brief Set if vertex position sets are enabled. */
	void SetEnableVertexPositionSet(bool enabled);
	
	/** \brief Use current animation state instead of empty state. */
	inline bool GetUseCurrentState() const{return pUseCurrentState;}
	
	/** \brief Set if current animation state is used instead of empty state. */
	void SetUseCurrentState(bool useCurrentState);
	
	/** \brief Rule application type. */
	inline eApplicationTypes GetApplicationType() const{return pApplicationType;}
	
	/** \brief Set the rule application type. */
	void SetApplicationType(eApplicationTypes type);
	
	/** \brief Select target. */
	inline deAnimatorControllerTarget &GetTargetSelect(){return pTargetSelect;}
	inline const deAnimatorControllerTarget &GetTargetSelect() const{return pTargetSelect;}
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the rule. */
	virtual void Visit(deAnimatorRuleVisitor &visitor);
	/*@}*/
};

#endif
