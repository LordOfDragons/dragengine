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

#ifndef _IGDEMETAPROPERTYPOINT3_H_
#define _IGDEMETAPROPERTYPOINT3_H_

#include "igdeMetaProperty.h"

#include <dragengine/common/math/decMath.h>


/**
 * \brief Point3 meta property.
 */
class DE_DLL_EXPORT igdeMetaPropertyPoint3 : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyPoint3>;
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyPoint3>{
	};
	
	
private:
	decPoint3 pDefaultValue;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create point3 meta property with label and description. */
	igdeMetaPropertyPoint3(const char *id, const char *name, const char *description);
	
protected:
	/** \brief Clean up point3 meta property. */
	~igdeMetaPropertyPoint3() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Default value. */
	inline const decPoint3 &GetDefaultValue() const{ return pDefaultValue; }
	
	/** \brief Set default value. */
	void SetDefaultValue(const decPoint3 &value);
	
	
	/** \brief Listeners. */
	inline igdeTListenerList<Listener> &GetListeners(){ return pListeners; }
	inline const igdeTListenerList<Listener> &GetListeners() const{ return pListeners; }
	
	/** \brief Notify listeners about value change. */
	void NotifyValueChanged(const igdeMetaContext::Ref &context);
	
	
	/**
	 * \brief Capture context.
	 *
	 * This returns an immutable context always returning the necessary state for getting or setting the property.
	 */
	virtual igdeMetaContext::Ref Capture(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Property is valid.
	 *
	 * This means calling GetPropertyValue() nor SetPropertyValue() throws an exception.
	 */
	virtual bool IsValid(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Get property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual const decPoint3 &GetPropertyValue(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValue(const igdeMetaContext::Ref &context, const decPoint3 &value) = 0;
	
	
	/**
	 * \brief Create UI widget.
	 *
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	deTObjectReference<igdeMetaPropertyWidget> CreateWidget(const igdeMetaContext::Ref &context) override;
	/*@}*/
};

#endif
