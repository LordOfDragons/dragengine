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

#ifndef _AERULESUBANIMATOR_H_
#define _AERULESUBANIMATOR_H_

#include "aeRule.h"
#include "../controller/aeController.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyPath.h>

#include <dragengine/common/collection/decTList.h>
#include <dragengine/resources/animator/deAnimator.h>

class aeLoadSaveSystem;
class deAnimatorRuleSubAnimator;


/**
 * Animator rule sub animator.
 */
class aeRuleSubAnimator : public aeRule{
public:
	using Ref = deTObjectReference<aeRuleSubAnimator>;
	using ConnectionList = decTObjectList<aeController>;
	
private:
	deAnimator::Ref pSubAnimator;
	
	ConnectionList pConnections;
	
	igdeMetaPropertyPathStorage::Storage pMPPathSubAnimator;
	igdeMetaPropertyBooleanStorage::Storage pMPEnablePosition;
	igdeMetaPropertyBooleanStorage::Storage pMPEnableOrientation;
	igdeMetaPropertyBooleanStorage::Storage pMPEnableSize;
	igdeMetaPropertyBooleanStorage::Storage pMPEnableVertexPositionSet;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRuleSubAnimator() = delete;
	aeRuleSubAnimator(const aeRuleSubAnimator&) = delete;
	
	/** Create a new sub animator rule. */
	aeRuleSubAnimator(aeWindowMain &windowMain, const char *name);
	/** Create a copy of a sub animator rule. */
	aeRuleSubAnimator(aeWindowMain &windowMain, const aeRuleSubAnimator &copy);
	/** Clean up the sub animator rule. */
protected:
	~aeRuleSubAnimator() override;
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	inline igdeMetaPropertyPathStorage::Storage &GetMPPathSubAnimator(){ return pMPPathSubAnimator; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnablePosition(){ return pMPEnablePosition; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnableOrientation(){ return pMPEnableOrientation; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnableSize(){ return pMPEnableSize; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnableVertexPositionSet(){ return pMPEnableVertexPositionSet; }
	
	
	/** Retrieve the path to the sub animator. */
	inline const decString &GetPathSubAnimator() const{ return pMPPathSubAnimator; }
	/** Set the path to the sub animator. */
	void SetPathSubAnimator(const char *value);
	/** Retrieve the sub animator or nullptr if not existing. */
	inline const deAnimator::Ref &GetSubAnimator() const{ return pSubAnimator; }
	/** Load the sub animator using the stored path. */
	void LoadSubAnimator();
	
	/** Connections. */
	inline const ConnectionList &GetConnections() const{ return pConnections; }
	
	/** Set controller for target controller or \em nullptr. */
	void SetControllerAt(int position, aeController *controller);
	
	/** Determine if position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pMPEnablePosition; }
	/** Set if position manipulation is enabled. */
	void SetEnablePosition(bool value);
	/** Determine if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pMPEnableOrientation; }
	/** Set if orientation manipulation is enabled. */
	void SetEnableOrientation(bool value);
	
	/** Determine if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pMPEnableSize; }
	
	/** Set if size manipulation is enabled. */
	void SetEnableSize(bool value);
	
	/** Vertex position set manipulation is enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pMPEnableVertexPositionSet; }
	
	/** Set if vertex position set manipulation is enabled. */
	void SetEnableVertexPositionSet(bool value);
	
	/** Create an engine animator rule. */
	deAnimatorRule::Ref CreateEngineRule() override;
	
	/** Update Component and Animation. */
	void UpdateCompAnim() override;
	
	/** Create a copy of this rule. */
	aeRule::Ref CreateCopy(aeWindowMain &windowMain) const override;
	
	/** List all links of all rule targets. */
	void ListLinks(aeLink::List& list) override;
	
	/** Parent animator changed. */
	void OnParentAnimatorChanged() override;
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	aeRuleSubAnimator &operator=(const aeRuleSubAnimator &copy) = delete;
	/*@}*/
	
private:
	void pUpdateConnections(deAnimatorRuleSubAnimator &rule) const;
};

#endif
