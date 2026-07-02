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

#ifndef _AERULESTATESNAPSHOT_H_
#define _AERULESTATESNAPSHOT_H_

#include "aeRule.h"

#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyInteger.h>

/**
 * Animator rule state snapshot.
 */
class aeRuleStateSnapshot : public aeRule{
public:
	using Ref = deTObjectReference<aeRuleStateSnapshot>;
	
	using MetaContext = igdeMetaContextTypeInherit<aeRuleStateSnapshot, aeRule>;
	static MetaContext::Ref CreateMetaContext(aeWindowMain &windowMain, aeRuleStateSnapshot *rule);
	
	template<typename T>
	using MetaProperty = igdeMetaPropertyMCT<T, MetaContext>;
	
private:
	igdeMetaPropertyBooleanStorage::Storage pMPUseLastState;
	igdeMetaPropertyIntegerStorage::Storage pMPId;
	igdeMetaPropertyBooleanStorage::Storage pMPEnablePosition;
	igdeMetaPropertyBooleanStorage::Storage pMPEnableOrientation;
	igdeMetaPropertyBooleanStorage::Storage pMPEnableSize;
	igdeMetaPropertyBooleanStorage::Storage pMPEnableVertexPositionSet;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRuleStateSnapshot() = delete;
	aeRuleStateSnapshot(const aeRuleStateSnapshot&) = delete;
	
	/** Create a new state snapshot rule. */
	aeRuleStateSnapshot(aeWindowMain &windowMain, const char *name);
	/** Create a copy of a state snapshot rule. */
	aeRuleStateSnapshot(aeWindowMain &windowMain, const aeRuleStateSnapshot &copy);
	/** Clean up the animator rule. */
protected:
	~aeRuleStateSnapshot() override;
private:
	aeRuleStateSnapshot(aeWindowMain &windowMain, const char *name, const MetaContext::Ref &metaContext);
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPUseLastState(){ return pMPUseLastState; }
	inline igdeMetaPropertyIntegerStorage::Storage &GetMPId(){ return pMPId; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnablePosition(){ return pMPEnablePosition; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnableOrientation(){ return pMPEnableOrientation; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnableSize(){ return pMPEnableSize; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnableVertexPositionSet(){ return pMPEnableVertexPositionSet; }
	
	
	/** Determine if the last state of the component is used or only the stored state. */
	inline bool GetUseLastState() const{ return pMPUseLastState; }
	/** Set if the last state of the component is used or only the stored state. */
	void SetUseLastState(bool value);
	
	/** Identifier to snapshot state using animator instance. */
	inline int GetID() const{ return pMPId; }
	
	/** Set identifier to snapshot state using animator instance. */
	void SetID(int value);
	
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
	
	/** Create a copy of this rule. */
	aeRule::Ref CreateCopy(aeWindowMain &windowMain) const override;
	
	/** List all links of all rule targets. */
	void ListLinks(aeLink::List& list) override;
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	aeRuleStateSnapshot &operator=(const aeRuleStateSnapshot &copy) = delete;
	/*@}*/
};

#endif
