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

#ifndef _IGDEMETAPROPERTYSLIDERBOARD_H_
#define _IGDEMETAPROPERTYSLIDERBOARD_H_

#include "igdeMetaPropertyString.h"
#include "igdeMetaPropertyFloat.h"
#include "storage/igdeMetaPropertyStorageList.h"
#include "../igdeTMetaData.h"

#include <dragengine/common/collection/decTOrderedSet.h>


/**
 * \brief Slider board meta property.
 * 
 * Stores string name and float value property to represent the name and value of the sliders.
 * Value is a set of meta contexts representing the sliders to show. Slider values can be changed
 * which changes the float value of the respective meta context.
 */
class DE_DLL_EXPORT igdeMetaPropertySliderBoard : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertySliderBoard>;
	
	/** \brief List of sliders. */
	using List = decTObjectOrderedSet<igdeMetaContext>;
	
	/** \brief Slider type reference. */
	using SliderRef = deTObjectReference<igdeMetaContext>;
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertySliderBoard>{
	};
	
	
private:
	const igdeMetaPropertyString::Ref pPropertySliderName;
	const igdeMetaPropertyFloat::Ref pPropertySliderValue;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create slider board meta property. */
	igdeMetaPropertySliderBoard(const char *id, igdeMetaPropertyString *propertySliderName,
		igdeMetaPropertyFloat *propertySliderValue);
	
protected:
	/** \brief Clean up slider board meta property. */
	~igdeMetaPropertySliderBoard() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Get property name. */
	inline const igdeMetaPropertyString::Ref &GetPropertySliderName() const{ return pPropertySliderName; }
	
	/** \brief Get property value. */
	inline const igdeMetaPropertyFloat::Ref &GetPropertySliderValue() const{ return pPropertySliderValue; }
	
	/** \brief Listeners. */
	inline igdeTListenerList<Listener> &GetListeners(){ return pListeners; }
	inline const igdeTListenerList<Listener> &GetListeners() const{ return pListeners; }
	
	/** \brief Notify listeners about value changed. */
	void NotifyValueChanged(const ContextRef &context);
	
	
	/**
	 * \brief Property is valid.
	 *
	 * This means calling GetPropertyValue() does not throw an exception.
	 */
	virtual bool IsValid(const ContextRef &context) const = 0;
	
	/**
	 * \brief Get property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual List GetPropertyValue(const ContextRef &context) const = 0;
	
	
	/**
	 * \brief Create UI widget.
	 *
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	deTObjectReference<igdeMetaPropertyWidget> CreateWidget() override;
	/*@}*/
};


/**
 * \brief Slider board meta property with deObject type.
 */
template<typename T, typename L = decTObjectOrderedSet<T>>
class igdeMetaPropertySliderBoardType : public igdeMetaPropertySliderBoard{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertySliderBoardType<T>>;
	
	/** \brief List of sliders. */
	using ListType = L;
	
	/** \brief Slider type reference. */
	using SliderTypeRef = deTObjectReference<T>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create slider board meta property with label and description. */
	igdeMetaPropertySliderBoardType(const char *id, igdeMetaPropertyString *propertySliderName,
		igdeMetaPropertyFloat *propertySliderValue) :
	igdeMetaPropertySliderBoard(id, propertySliderName, propertySliderValue){}
	
protected:
	/** \brief Clean up slider board meta property. */
	~igdeMetaPropertySliderBoardType() override = default;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Convert list. */
	List ConvertList(const ListType &in) const{
		List out;
		in.Visit([&](const SliderTypeRef &slider){
			out.Add(slider.Pointer());
		});
		return out;
	}
	
	/** Convert list. */
	ListType ConvertList(const List &in) const{
		ListType out;
		in.Visit([&](const SliderRef &slider){
			out.Add(slider.DynamicCast<T>());
		});
		return out;
	}
	
	List GetPropertyValue(const ContextRef &context) const override{
		return ConvertList(GetPropertyValueType(context));
	}
	
	
	/**
	 * \brief Get property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual ListType GetPropertyValueType(const ContextRef &context) const = 0;
	/*@}*/
};


/**
 * \brief Slider board meta property using storage.
 */
template<typename T, typename L = decTObjectOrderedSet<T>>
class igdeMetaPropertySliderBoardStorage : public igdeMetaPropertySliderBoardType<T, L>{
public:
	/** \brief List type. */
	using ListType = L;
	
	/** \brief Storage type. */
	using Storage = igdeMetaPropertyStorageList<T, igdeMetaPropertySliderBoardStorage<T, L>>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create slider board meta property. */
	igdeMetaPropertySliderBoardStorage(const char *id, igdeMetaPropertyString *propertySliderName,
		igdeMetaPropertyFloat *propertySliderValue) :
	igdeMetaPropertySliderBoardType<T, L>(id, propertySliderName, propertySliderValue){}
	
protected:
	/** \brief Clean up slider board meta property. */
	~igdeMetaPropertySliderBoardStorage() override = default;
	
public:
	/*@}*/
	/** \brief Storage. */
	virtual Storage &GetStorage(const igdeMetaProperty::ContextRef &context) const = 0;
	
	ListType GetPropertyValueType(const igdeMetaProperty::ContextRef &context) const override{
		return GetStorage(context).GetValue();
	}
};

#endif
