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

#ifndef _IGDEMETAPROPERTYSTRING_H_
#define _IGDEMETAPROPERTYSTRING_H_

#include "igdeMetaProperty.h"
#include "storage/igdeMetaPropertyStorageString.h"
#include "../igdeTMetaData.h"
#include "../../clipboard/igdeClipboardData.h"
#include "../../gui/model/igdeListItem.h"

#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/string/decStringSet.h>

class igdeMetaPropertyStringUndo;


/**
 * \brief String meta property.
 */
class DE_DLL_EXPORT igdeMetaPropertyString : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyString>;
	
	/** \brief Present. */
	using Preset = TPreset<decString>;
	
	/** \brief Present list. */
	using PresetList = decTObjectOrderedSet<Preset>;
	
	
	/** \brief Clipboard data. */
	class DE_DLL_EXPORT ClipboardData : public igdeTClipboardData<decString>{
	public:
		using Ref = deTObjectReference<ClipboardData>;
		
		/** \brief Type name. */
		static constexpr const char *TypeName = "MetaProperty.String";
		
		explicit ClipboardData(const igdeMetaProperty &property, const decString &value) :
			igdeTClipboardData<decString>(property.GetClipboardDataTypeName(), value){}
		
		explicit ClipboardData(const igdeMetaProperty &property, decString &&value) :
			igdeTClipboardData<decString>(property.GetClipboardDataTypeName(), std::move(value)){}
		
		explicit ClipboardData(const char *typeName, const decString &value) :
			igdeTClipboardData<decString>(typeName, value){}
		
		explicit ClipboardData(const char *typeName, decString &&value) :
			igdeTClipboardData<decString>(typeName, std::move(value)){}
		
	protected:
		/** \brief Clean up object. */
		~ClipboardData() override = default;
	};
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyString>{
	public:
		/** \brief Allowed strings changed. */
		virtual void OnAllowedStringsChanged(igdeMetaPropertyString *property, const ContextRef &context);
	};
	
	
private:
	decString pDefaultValue;
	bool pEnableAllowed;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create string meta property with label and description. */
	igdeMetaPropertyString(const char *id, const char *name, const char *description);
	
	/** \brief Create string meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyString(const char *id, const char *translationTag);
	
protected:
	/** \brief Clean up string meta property. */
	~igdeMetaPropertyString() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Default value. */
	inline const decString &GetDefaultValue() const{ return pDefaultValue; }
	
	/** \brief Set default value. */
	void SetDefaultValue(const decString &value);
	
	/** \brief Enable allowed restriction. */
	inline bool GetEnableAllowed() const{ return pEnableAllowed; }
	
	/** \brief Set enable allowed restriction. */
	void SetEnableAllowed(bool enable);
	
	
	/** \brief Listeners. */
	inline igdeTListenerList<Listener> &GetListeners(){ return pListeners; }
	inline const igdeTListenerList<Listener> &GetListeners() const{ return pListeners; }
	
	/** \brief Notify listeners about value change. */
	void NotifyValueChanged(const ContextRef &context);
	
	/** \brief Notify listeners about allowed strings changed. */
	void NotifyAllowedStringsChanged(const ContextRef &context);
	
	
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
	virtual const decString &GetPropertyValue(const ContextRef &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 * 
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValue(const ContextRef &context, const decString &value) = 0;
	
	/**
	 * \brief Change property value matching context with undo support.
	 * 
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	virtual deTObjectReference<igdeMetaPropertyStringUndo> ChangePropertyValue(
		const ContextRef &context, const char *newValue,
		const char *undoInfo = nullptr, const char *undoInfoLong = nullptr);
	
	/** \brief Allowed strings. */
	virtual decStringSet GetPropertyAllowedStrings(const ContextRef &context) const;
	
	/** \brief Presets. */
	virtual PresetList GetPropertyPresets(const ContextRef &context) const;
	
	
	/**
	 * \brief Create UI widget.
	 * 
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	deTObjectReference<igdeMetaPropertyWidget> CreateWidget() override;
	/*@}*/
};


/**
 * \brief String meta property using storage.
 */
class DE_DLL_EXPORT igdeMetaPropertyStringStorage : public igdeMetaPropertyString{
public:
	/** \brief Storage type. */
	using Storage = igdeMetaPropertyStorageString<igdeMetaPropertyStringStorage>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create string meta property with label and description. */
	igdeMetaPropertyStringStorage(const char *id, const char *name, const char *description);
	
	/** \brief Create meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyStringStorage(const char *id, const char *translationTag);
	
protected:
	/** \brief Clean up string meta property. */
	~igdeMetaPropertyStringStorage() override;
	
public:
	/*@}*/
	/** \brief Storage. */
	virtual Storage &GetStorage(const ContextRef &context) const = 0;
	
	
	const decString &GetPropertyValue(const ContextRef &context) const override;
	void SetPropertyValue(const ContextRef &context, const decString &value) override;
};


#include "undo/igdeMetaPropertyStringUndo.h"

#endif
