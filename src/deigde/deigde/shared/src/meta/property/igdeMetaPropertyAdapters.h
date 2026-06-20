/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDEMETAPROPERTYADAPTERS_H_
#define _IGDEMETAPROPERTYADAPTERS_H_

#include "igdeMetaPropertyList.h"
#include "igdeMetaPropertyObject.h"
#include "igdeMetaPropertyString.h"
#include "igdeMetaPropertyStringSet.h"
#include "../igdeMetaContext.h"


/**
 * \brief Adapter for meta properties displaying information using another meta property.
 */
class DE_DLL_EXPORT igdeMetaPropertyAdapter{
public:
	class DE_DLL_EXPORT StringOnListChanged : public igdeMetaPropertyList::Listener{
		igdeMetaPropertyString &pProperty;
		
	public:
		using Ref = deTObjectReference<StringOnListChanged>;
		StringOnListChanged(igdeMetaPropertyString &property);
		void OnValueChanged(igdeMetaPropertyList *property, const igdeMetaContext::Ref &context) override;
		void OnObjectItemInfoChanged(igdeMetaPropertyList *property, const igdeMetaContext::Ref &context) override;
		
	protected:
		~StringOnListChanged() override;
	};
	
	
	class DE_DLL_EXPORT ObjectOnListChanged : public igdeMetaPropertyList::Listener{
		igdeMetaPropertyObject &pProperty;
		
	public:
		using Ref = deTObjectReference<ObjectOnListChanged>;
		ObjectOnListChanged(igdeMetaPropertyObject &property);
		void OnValueChanged(igdeMetaPropertyList *property, const igdeMetaContext::Ref &context) override;
		void OnObjectItemInfoChanged(igdeMetaPropertyList *property, const igdeMetaContext::Ref &context) override;
		
	protected:
		~ObjectOnListChanged() override;
	};
	
	
	class DE_DLL_EXPORT StringOnStringSetChanged : public igdeMetaPropertyStringSet::Listener{
		igdeMetaPropertyString &pProperty;
		
	public:
		using Ref = deTObjectReference<StringOnStringSetChanged>;
		StringOnStringSetChanged(igdeMetaPropertyString &property);
		void OnValueChanged(igdeMetaPropertyStringSet *property, const igdeMetaContext::Ref &context) override;
		
	protected:
		~StringOnStringSetChanged() override;
	};
	
	
	class DE_DLL_EXPORT ListOnStringChanged : public igdeMetaPropertyString::Listener{
		igdeMetaPropertyList &pProperty;
		
	public:
		using Ref = deTObjectReference<ListOnStringChanged>;
		ListOnStringChanged(igdeMetaPropertyList &property);
		void OnValueChanged(igdeMetaPropertyString *property, const igdeMetaContext::Ref &context) override;
		
	protected:
		~ListOnStringChanged() override;
	};
	
	
private:
	igdeMetaPropertyAdapter();
	
public:
	/** \brief Watch property and notify target on changes. */
	static void OnChanged(igdeMetaPropertyList &watch, igdeMetaPropertyString &target);
	static void OnChanged(igdeMetaPropertyList &watch, igdeMetaPropertyObject &target);
	static void OnChanged(igdeMetaPropertyStringSet &watch, igdeMetaPropertyString &target);
	static void OnChanged(igdeMetaPropertyString &watch, igdeMetaPropertyList &target);
};



#endif
