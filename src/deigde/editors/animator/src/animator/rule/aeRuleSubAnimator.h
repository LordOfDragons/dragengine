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
	
	
	class Connection : public deObject{
	public:
		using Ref = deTObjectReference<Connection>;
		
		using MetaContext = igdeMetaContextType<Connection>;
		static MetaContext::Ref CreateMetaContext(aeWindowMain &windowMain, Connection *connection);
		
		template<typename T>
		using MetaProperty = igdeMetaPropertyMCT<T, MetaContext>;
		
		aeWindowMain &windowMain;
		MetaContext::Ref metaContext;
		igdeMetaPropertyStringStorage::Storage mpTarget;
		igdeMetaPropertyObjectStorage<aeController>::Storage mpController;
		aeRuleSubAnimator *parentRule = nullptr;
		int targetIndex = -1;
		
		explicit Connection(aeWindowMain &windowMain);
		Connection(const Connection &copy);
		
		igdeEnvironment &GetEnvironment() const;
		igdeUndoSystem *GetUndoSystem() const;
		
	protected:
		~Connection() override;
	};
	
	
	using MetaContext = igdeMetaContextTypeInherit<aeRuleSubAnimator, aeRule>;
	static MetaContext::Ref CreateMetaContext(aeWindowMain &windowMain, aeRuleSubAnimator *rule);
	
	template<typename T>
	using MetaProperty = igdeMetaPropertyMCT<T, MetaContext>;
	
private:
	deAnimator::Ref pSubAnimator;
	
public:
	igdeMetaPropertyPathStorage::Storage mpPathSubAnimator;
	igdeMetaPropertyBooleanStorage::Storage mpEnablePosition;
	igdeMetaPropertyBooleanStorage::Storage mpEnableOrientation;
	igdeMetaPropertyBooleanStorage::Storage mpEnableSize;
	igdeMetaPropertyBooleanStorage::Storage mpEnableVertexPositionSet;
	igdeMetaPropertyListStorage<Connection>::Storage mpConnections;
	igdeMetaPropertyContextStorage::Storage mpConnection;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRuleSubAnimator() = delete;
	
	/** Create a new sub animator rule. */
	aeRuleSubAnimator(aeWindowMain &windowMain, const char *name);
	/** Create a copy of a sub animator rule. */
	aeRuleSubAnimator(const aeRuleSubAnimator &copy);
	/** Clean up the sub animator rule. */
protected:
	~aeRuleSubAnimator() override;
private:
	aeRuleSubAnimator(aeWindowMain &windowMain, const char *name, const MetaContext::Ref &metaContext);
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the sub animator or nullptr if not existing. */
	inline const deAnimator::Ref &GetSubAnimator() const{ return pSubAnimator; }
	
	/** Load the sub animator using the stored path. */
	void LoadSubAnimator();
	
	/** Create an engine animator rule. */
	deAnimatorRule::Ref CreateEngineRule() override;
	
	/** Update Component and Animation. */
	void UpdateCompAnim() override;
	
	/** Create a copy of this rule. */
	aeRule::Ref CreateCopy() const override;
	
	/** Parent animator changed. */
	void OnParentAnimatorChanged() override;
	
	/** Update engine connections. */
	void UpdateEngineConnections() const;
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	aeRuleSubAnimator &operator=(const aeRuleSubAnimator &copy) = delete;
	/*@}*/
	
private:
	void pUpdateEngineConnections(deAnimatorRuleSubAnimator &rule) const;
};

#endif
