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

#ifndef _IGDEMETAPROPERTYTEXTAREA_H_
#define _IGDEMETAPROPERTYTEXTAREA_H_

#include "igdeMetaProperty.h"
#include "storage/igdeMetaPropertyStorageString.h"

class igdeMetaPropertyTextAreaUndo;


/**
 * \brief Text area meta property.
 */
class DE_DLL_EXPORT igdeMetaPropertyTextArea : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyTextArea>;
	
	/** \brief Present. */
	using Preset = TPreset<decString>;
	
	/** \brief Present list. */
	using PresetList = decTObjectOrderedSet<Preset>;
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyTextArea>{
	};
	
	
private:
	decString pDefaultValue;
	int pRows;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create text area meta property with label and description. */
	igdeMetaPropertyTextArea(const char *id, const char *name, const char *description, int rows);
	
	/** \brief Create text area meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyTextArea(const char *id, const char *translationTag, int rows);
	
protected:
	/** \brief Clean up text area meta property. */
	~igdeMetaPropertyTextArea() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Default value. */
	inline const decString &GetDefaultValue() const{ return pDefaultValue; }
	
	/** \brief Set default value. */
	void SetDefaultValue(const decString &value);
	
	/** \brief Rows. */
	inline int GetRows() const{ return pRows; }
	
	/** \brief Set rows. */
	void SetRows(int rows);
	
	
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
	virtual deTObjectReference<igdeMetaPropertyTextAreaUndo> ChangePropertyValue(
		const ContextRef &context, const char *newValue,
		const char *undoInfo = nullptr, const char *undoInfoLong = nullptr);
	
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
 * \brief Text area meta property using storage.
 */
class DE_DLL_EXPORT igdeMetaPropertyTextAreaStorage : public igdeMetaPropertyTextArea{
public:
	/** \brief Storage type. */
	using Storage = igdeMetaPropertyStorageString<igdeMetaPropertyTextAreaStorage>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create text area meta property with label and description. */
	igdeMetaPropertyTextAreaStorage(const char *id, const char *name, const char *description, int rows);
	
	/** \brief Create meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyTextAreaStorage(const char *id, const char *translationTag, int rows);
	
protected:
	/** \brief Clean up text area meta property. */
	~igdeMetaPropertyTextAreaStorage() override;
	
public:
	/*@}*/
	/** \brief Storage. */
	virtual Storage &GetStorage(const ContextRef &context) const = 0;
	
	
	const decString &GetPropertyValue(const ContextRef &context) const override;
	void SetPropertyValue(const ContextRef &context, const decString &value) override;
};


#include "undo/igdeMetaPropertyTextAreaUndo.h"

#endif
