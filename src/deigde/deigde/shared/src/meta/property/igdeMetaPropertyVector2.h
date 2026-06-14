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

#ifndef _IGDEMETAPROPERTYVECTOR2_H_
#define _IGDEMETAPROPERTYVECTOR2_H_

#include "igdeMetaProperty.h"
#include "storage/igdeMetaPropertyStorageComplexIsEqualTo.h"
#include "../../clipboard/igdeClipboardData.h"
#include <dragengine/common/math/decMath.h>

class igdeMetaPropertyVector2Undo;


/**
 * \brief Vector2 meta property.
 */
class DE_DLL_EXPORT igdeMetaPropertyVector2 : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyVector2>;
	
	
	/** \brief Clipboard data. */
	class DE_DLL_EXPORT ClipboardData : public igdeTClipboardData<decVector2>{
	public:
		using Ref = deTObjectReference<ClipboardData>;
		
		/** \brief Type name. */
		static constexpr const char *TypeName = "MetaProperty.Vector2";
		
		explicit inline ClipboardData(const decVector2 &value) : igdeTClipboardData<decVector2>(TypeName, value){}
		explicit inline ClipboardData(decVector2 &&value) : igdeTClipboardData<decVector2>(TypeName, value){}
		
	protected:
		/** \brief Clean up object. */
		~ClipboardData() override = default;
	};
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyVector2>{
	};
	
	
private:
	decVector2 pDefaultValue;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create vector2 meta property with label and description. */
	igdeMetaPropertyVector2(const char *id, const char *name, const char *description);
	
protected:
	/** \brief Clean up vector2 meta property. */
	~igdeMetaPropertyVector2() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Default value. */
	inline const decVector2 &GetDefaultValue() const{ return pDefaultValue; }
	
	/** \brief Set default value. */
	void SetDefaultValue(const decVector2 &value);
	
	
	/** \brief Listeners. */
	inline igdeTListenerList<Listener> &GetListeners(){ return pListeners; }
	inline const igdeTListenerList<Listener> &GetListeners() const{ return pListeners; }
	
	/** \brief Notify listeners about value change. */
	void NotifyValueChanged(const ContextRef &context);
	
	
	/**
	 * \brief Capture context.
	 * 
	 * This returns an immutable context always returning the necessary state for getting or setting the property.
	 */
	virtual ContextRef Capture(const ContextRef &context) const = 0;
	
	/**
	 * \brief Property is valid.
	 * 
	 * This means calling GetPropertyValue() nor SetPropertyValue() throws an exception.
	 */
	virtual bool IsValid(const ContextRef &context) const = 0;
	
	/**
	 * \brief Get property value matching context.
	 * 
	 * Implemented by subclass.
	 */
	virtual const decVector2 &GetPropertyValue(const ContextRef &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 * 
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValue(const ContextRef &context, const decVector2 &value) = 0;
	
	/**
	 * \brief Change property value matching context with undo support.
	 * 
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	virtual deTObjectReference<igdeMetaPropertyVector2Undo> ChangePropertyValue(
		const ContextRef &context, const decVector2 &newValue,
		const char *undoInfo = nullptr, const char *undoInfoLong = nullptr);
	
	
	/**
	 * \brief Create UI widget.
	 * 
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	deTObjectReference<igdeMetaPropertyWidget> CreateWidget(const ContextRef &context) override;
	/*@}*/
};


/**
 * \brief Vector2 meta property using storage.
 */
class DE_DLL_EXPORT igdeMetaPropertyVector2Storage : public igdeMetaPropertyVector2{
public:
	/** \brief Storage type. */
	using Storage = igdeMetaPropertyStorageComplexIsEqualTo<decVector2, igdeMetaPropertyVector2Storage>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create vector2 meta property with label and description. */
	igdeMetaPropertyVector2Storage(const char *id, const char *name, const char *description);
	
protected:
	/** \brief Clean up vector2 meta property. */
	~igdeMetaPropertyVector2Storage() override;
	
public:
	/*@}*/
	/** \brief Storage. */
	virtual Storage &GetStorage(const ContextRef &context) const = 0;
	
	
	const decVector2 &GetPropertyValue(const ContextRef &context) const override;
	void SetPropertyValue(const ContextRef &context, const decVector2 &value) override;
};

#endif
