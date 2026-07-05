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

#ifndef _AERULEMIRROR_H_
#define _AERULEMIRROR_H_

#include "aeRule.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyList.h>
#include <deigde/meta/property/igdeMetaPropertyContext.h>

#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleMirror.h>

/**
 * Mirror rule.
 */
class aeRuleMirror : public aeRule{
public:
	using Ref = deTObjectReference<aeRuleMirror>;
	
	
	class MatchName : public deObject{
	public:
		using Ref = deTObjectReference<MatchName>;
		
		using MetaContext = igdeMetaContextType<MatchName>;
		static MetaContext::Ref CreateMetaContext(aeWindowMain &windowMain, MatchName *matchName);
		
		template<typename T>
		using MetaProperty = igdeMetaPropertyMCT<T, MetaContext>;
		
		aeWindowMain &windowMain;
		MetaContext::Ref metaContext;
		igdeMetaPropertyStringStorage::Storage mpFirst, mpSecond;
		igdeMetaPropertySelectionEnumStorage<deAnimatorRuleMirror::eMatchNameType>::Storage mpType;
		
		explicit MatchName(aeWindowMain &windowMain);
		explicit MatchName(aeWindowMain &windowMain, const char *first, const char *second,
			deAnimatorRuleMirror::eMatchNameType type);
		MatchName(const MatchName &copy);
		
		bool operator==(const MatchName &matchName) const;
		
		igdeEnvironment &GetEnvironment() const;
		igdeUndoSystem *GetUndoSystem() const;
		
	protected:
		~MatchName() override;
	};
	
	
	using MetaContext = igdeMetaContextTypeInherit<aeRuleMirror, aeRule>;
	static MetaContext::Ref CreateMetaContext(aeWindowMain &windowMain, aeRuleMirror *rule);
	
	template<typename T>
	using MetaProperty = igdeMetaPropertyMCT<T, MetaContext>;
	
	
private:
	aeWindowMain &pWindowMain;
	
public:
	igdeMetaPropertySelectionEnumStorage<deAnimatorRuleMirror::eMirrorAxis>::Storage mpMirrorAxis;
	igdeMetaPropertyStringStorage::Storage mpMirrorBone;
	igdeMetaPropertyBooleanStorage::Storage mpEnablePosition;
	igdeMetaPropertyBooleanStorage::Storage mpEnableOrientation;
	igdeMetaPropertyBooleanStorage::Storage mpEnableSize;
	igdeMetaPropertyBooleanStorage::Storage mpEnableVertexPositionSet;
	igdeMetaPropertyListStorage<MatchName>::Storage mpMatchNames;
	igdeMetaPropertyContextStorage::Storage mpMatchName;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRuleMirror() = delete;
	
	/** Create rule. */
	aeRuleMirror(aeWindowMain &windowMain, const char *name);
	
	/** Create copy of rule. */
	aeRuleMirror(const aeRuleMirror &copy);
	
	/** Create rule with default settings. */
	static aeRuleMirror::Ref CreateDefault(aeWindowMain &windowMain, const char *name);
	
protected:
	/** Clean up rule. */
	~aeRuleMirror() override;
	
private:
	aeRuleMirror(aeWindowMain &windowMain, const char *name, const MetaContext::Ref &metaContext);
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	inline aeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** Create engine animator rule. */
	deAnimatorRule::Ref CreateEngineRule() override;
	
	/** Create copy of rule. */
	aeRule::Ref CreateCopy() const override;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	aeRuleMirror &operator=(const aeRuleMirror &copy) = delete;
	/*@}*/
	
	
	
private:
	void pUpdateMatchNames();
};

#endif
