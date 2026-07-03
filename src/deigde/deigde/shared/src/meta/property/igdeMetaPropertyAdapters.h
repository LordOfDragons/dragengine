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
#include "igdeMetaPropertySet.h"
#include "igdeMetaPropertyString.h"
#include "igdeMetaPropertyStringSet.h"
#include "igdeMetaPropertyTreeList.h"
#include "igdeMetaPropertySelection.h"
#include "../igdeMetaContext.h"


/**
 * \brief Adapter for meta properties displaying information using another meta property.
 */
class DE_DLL_EXPORT igdeMetaPropertyAdapter{
public:
	/**
	 * \brief Watch property value changed.
	 * 
	 * Can be used on meta properties with only OnValueChanged event.
	 */
	template<typename T>
	class WatchValueChanged : public T::Listener{
	public:
		using Ref = deTObjectReference<WatchValueChanged>;
		WatchValueChanged() = default;
		void OnValueChanged(T*, const igdeMetaContext::Ref& context) override{ ValueChanged(context); }
		virtual void ValueChanged(const igdeMetaContext::Ref &context) = 0;
		virtual void StructureChanged(const igdeMetaContext::Ref &context){};
		
	protected:
		~WatchValueChanged() override = default;
	};
	
	/**
	 * \brief Watch property value and object information changed.
	 * 
	 * Can be used on meta properties with OnValueChanged and OnObjectItemInfoChanged events.
	 */
	template<typename T>
	class WatchValueObjectItemInfoChanged : public T::Listener{
	public:
		using Ref = deTObjectReference<WatchValueObjectItemInfoChanged>;
		WatchValueObjectItemInfoChanged() = default;
		void OnValueChanged(T*, const igdeMetaContext::Ref& context) override{ StructureChanged(context); }
		void OnObjectItemInfoChanged(T*, const igdeMetaContext::Ref& context) override{ ValueChanged(context); }
		virtual void ValueChanged(const igdeMetaContext::Ref &context) = 0;
		virtual void StructureChanged(const igdeMetaContext::Ref &context){ ValueChanged(context); }
		
	protected:
		~WatchValueObjectItemInfoChanged() override = default;
	};
	
	
	/**
	 * \brief Notify list on property changed.
	 * 
	 * Can be used on meta properties of list type:
	 * - igdeMetaPropertyList
	 * - igdeMetaPropertySet
	 */
	template<typename T, typename W>
	class NotifyListOnChanged : public W{
		T &pProperty;
		
	public:
		using Ref = deTObjectReference<NotifyListOnChanged>;
		NotifyListOnChanged(T &property) : pProperty(property){}
		void ValueChanged(const igdeMetaContext::Ref&) override{
			pProperty.NotifyObjectItemInfoChanged({});
		}
		
	protected:
		~NotifyListOnChanged() override = default;
	};
	
	/**
	 * \brief Notify string on property changed.
	 * 
	 * Can be used on meta properties of string type:
	 * - igdeMetaPropertyString
	 */
	template<typename T, typename W>
	class NotifyStringOnChanged : public W{
		T &pProperty;
		
	public:
		using Ref = deTObjectReference<NotifyStringOnChanged>;
		NotifyStringOnChanged(T &property) : pProperty(property){}
		void ValueChanged(const igdeMetaContext::Ref&) override{
			pProperty.NotifyAllowedStringsChanged({});
		}
		
	protected:
		~NotifyStringOnChanged() override = default;
	};
	
	/**
	 * \brief Notify object on property changed.
	 * 
	 * Can be used on meta properties of object:
	 * - igdeMetaPropertyObject
	 */
	template<typename T, typename W>
	class NotifyObjectOnChanged : public W{
		T &pProperty;
		
	public:
		using Ref = deTObjectReference<NotifyObjectOnChanged>;
		NotifyObjectOnChanged(T &property) : pProperty(property){}
		void ValueChanged(const igdeMetaContext::Ref&) override{
			pProperty.NotifyAllowedObjectsChanged({});
		}
		
	protected:
		~NotifyObjectOnChanged() override = default;
	};
	
	/**
	 * \brief Notify tree list on property changed.
	 * 
	 * Can be used on meta properties of tree list type:
	 * - igdeMetaPropertyTreeList
	 */
	template<typename T, typename W>
	class NotifyTreeListOnChanged : public W{
		T &pProperty;
		
	public:
		using Ref = deTObjectReference<NotifyTreeListOnChanged>;
		NotifyTreeListOnChanged(T &property) : pProperty(property){}
		void ValueChanged(const igdeMetaContext::Ref&) override{
			pProperty.NotifyObjectItemInfoChanged({});
		}
		void StructureChanged(const igdeMetaContext::Ref&) override{
			pProperty.NotifyValueChanged({});
		}
		
	protected:
		~NotifyTreeListOnChanged() override = default;
	};
	
	
private:
	igdeMetaPropertyAdapter();
	
public:
	/** \brief Watch list property and notify target on changes. */
	static void OnChanged(igdeMetaPropertyList &watch, igdeMetaPropertyString &target){
		watch.GetListeners().Add(NotifyStringOnChanged<igdeMetaPropertyString,
			WatchValueObjectItemInfoChanged<igdeMetaPropertyList>>::Ref::New(target));
	}
	
	static void OnChanged(igdeMetaPropertyList &watch, igdeMetaPropertyObject &target){
		watch.GetListeners().Add(NotifyObjectOnChanged<igdeMetaPropertyObject,
			WatchValueObjectItemInfoChanged<igdeMetaPropertyList>>::Ref::New(target));
	}
	
	static void OnChanged(igdeMetaPropertyList &watch, igdeMetaPropertyTreeList &target){
		watch.GetListeners().Add(NotifyTreeListOnChanged<igdeMetaPropertyTreeList,
			WatchValueObjectItemInfoChanged<igdeMetaPropertyList>>::Ref::New(target));
	}
	
	
	/** \brief Watch set and notify target on changes. */
	static void OnChanged(igdeMetaPropertySet &watch, igdeMetaPropertyString &target){
		watch.GetListeners().Add(NotifyStringOnChanged<igdeMetaPropertyString,
			WatchValueObjectItemInfoChanged<igdeMetaPropertySet>>::Ref::New(target));
	}
	
	static void OnChanged(igdeMetaPropertySet &watch, igdeMetaPropertyObject &target){
		watch.GetListeners().Add(NotifyObjectOnChanged<igdeMetaPropertyObject,
			WatchValueObjectItemInfoChanged<igdeMetaPropertySet>>::Ref::New(target));
	}
	
	static void OnChanged(igdeMetaPropertySet &watch, igdeMetaPropertyTreeList &target){
		watch.GetListeners().Add(NotifyTreeListOnChanged<igdeMetaPropertyTreeList,
			WatchValueObjectItemInfoChanged<igdeMetaPropertySet>>::Ref::New(target));
	}
	
	
	/** \brief Watch string set and notify target on changes. */
	static void OnChanged(igdeMetaPropertyStringSet &watch, igdeMetaPropertyString &target){
		watch.GetListeners().Add(NotifyStringOnChanged<igdeMetaPropertyString,
			WatchValueChanged<igdeMetaPropertyStringSet>>::Ref::New(target));
	}
	
	static void OnChanged(igdeMetaPropertyStringSet &watch, igdeMetaPropertyObject &target){
		watch.GetListeners().Add(NotifyObjectOnChanged<igdeMetaPropertyObject,
			WatchValueChanged<igdeMetaPropertyStringSet>>::Ref::New(target));
	}
	
	static void OnChanged(igdeMetaPropertyStringSet &watch, igdeMetaPropertyTreeList &target){
		watch.GetListeners().Add(NotifyTreeListOnChanged<igdeMetaPropertyTreeList,
			WatchValueChanged<igdeMetaPropertyStringSet>>::Ref::New(target));
	}
	
	
	/** \brief Watch property and notify target on changes. */
	static void OnChanged(igdeMetaPropertyString &watch, igdeMetaPropertyList &target){
		watch.GetListeners().Add(NotifyListOnChanged<igdeMetaPropertyList,
			WatchValueChanged<igdeMetaPropertyString>>::Ref::New(target));
	}
	
	static void OnChanged(igdeMetaPropertyString &watch, igdeMetaPropertySet &target){
		watch.GetListeners().Add(NotifyListOnChanged<igdeMetaPropertySet,
			WatchValueChanged<igdeMetaPropertyString>>::Ref::New(target));
	}
	
	static void OnChanged(igdeMetaPropertyString &watch, igdeMetaPropertyTreeList &target){
		watch.GetListeners().Add(NotifyTreeListOnChanged<igdeMetaPropertyTreeList,
			WatchValueChanged<igdeMetaPropertyString>>::Ref::New(target));
	}
	
	
	/** \brief Watch property and notify target on changes. */
	template<typename T>
	static void OnChangedUsing(T &watch, igdeMetaPropertyList &target){
		watch.GetListeners().Add(NotifyListOnChanged<igdeMetaPropertyList,
			WatchValueChanged<T>>::Ref::New(target));
	}
	
	template<typename T>
	static void OnChangedUsing(T &watch, igdeMetaPropertySet &target){
		watch.GetListeners().Add(NotifyListOnChanged<igdeMetaPropertySet,
			WatchValueChanged<T>>::Ref::New(target));
	}
	
	template<typename T>
	static void OnChangedUsing(T &watch, igdeMetaPropertyTreeList &target){
		watch.GetListeners().Add(NotifyTreeListOnChanged<igdeMetaPropertyTreeList,
			WatchValueChanged<T>>::Ref::New(target));
	}
};



#endif
