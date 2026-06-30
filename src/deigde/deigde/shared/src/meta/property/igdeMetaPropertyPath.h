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

#ifndef _IGDEMETAPROPERTYPATH_H_
#define _IGDEMETAPROPERTYPATH_H_

#include "igdeMetaProperty.h"
#include "storage/igdeMetaPropertyStorageString.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gui/filedialog/igdeFilePattern.h"

#include <dragengine/common/string/decStringDictionary.h>

class igdeMetaPropertyPathUndo;


/**
 * \brief Path meta property.
 */
class DE_DLL_EXPORT igdeMetaPropertyPath : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyPath>;
	
	/** \brief Present. */
	using Preset = TPreset<decString>;
	
	/** \brief Present list. */
	using PresetList = decTObjectOrderedSet<Preset>;
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyPath>{
	public:
		/** \brief Base path changed. */
		virtual void OnBasePathChanged(igdeMetaPropertyPath *property, const ContextRef &context);
	};
	
	
private:
	igdeEnvironment::eFilePatternListTypes pResourceType;
	igdeFilePattern::List pCustomPatternList;
	decString pDefaultValue;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create path meta property with label and description. */
	igdeMetaPropertyPath(const char *id, const char *name, const char *description,
		igdeEnvironment::eFilePatternListTypes resourceType);
	
	/** \brief Create path meta property with label, description and custom file pattern list. */
	igdeMetaPropertyPath(const char *id, const char *name, const char *description,
		const igdeFilePattern::List &customPatternList);
	
	/** \brief Create path meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyPath(const char *id, const char *translationTag,
		igdeEnvironment::eFilePatternListTypes resourceType);
	
	/** \brief Create path meta property with label, description, filter and undo info set from translation tag and custom file pattern list. */
	igdeMetaPropertyPath(const char *id, const char *translationTag,
		const igdeFilePattern::List &customPatternList);
	
protected:
	/** \brief Clean up path meta property. */
	~igdeMetaPropertyPath() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Resource type. */
	inline igdeEnvironment::eFilePatternListTypes GetResourceType() const{ return pResourceType; }
	
	/** \brief Custom pattern list. */
	inline const igdeFilePattern::List &GetCustomPatternList() const{ return pCustomPatternList; }
	
	/** \brief Default value. */
	inline const decString &GetDefaultValue() const{ return pDefaultValue; }
	
	/** \brief Set default value. */
	void SetDefaultValue(const decString &value);
	
	
	/** \brief Listeners. */
	inline igdeTListenerList<Listener> &GetListeners(){ return pListeners; }
	inline const igdeTListenerList<Listener> &GetListeners() const{ return pListeners; }
	
	/** \brief Notify listeners about value change. */
	void NotifyValueChanged(const ContextRef &context);
	
	/** \brief Notify listeners about base path changed. */
	void NotifyBasePathChanged(const ContextRef &context);
	
	
	/**
	 * \brief Create UI widget.
	 * 
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	deTObjectReference<igdeMetaPropertyWidget> CreateWidget() override;
	
	
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
	virtual deTObjectReference<igdeMetaPropertyPathUndo> ChangePropertyValue(
		const ContextRef &context, const char *newValue,
		const char *undoInfo = nullptr, const char *undoInfoLong = nullptr);
	
	/**
	 * \brief Get property base path matching context.
	 * 
	 * Implemented by subclass.
	 */
	virtual decString GetPropertyBasePath(const ContextRef &context) const;
	
	/** \brief Presets. */
	virtual PresetList GetPropertyPresets(const ContextRef &context) const;
	
	/**
	 * \brief Additional file resource information.
	 * 
	 * If supported by subclass the file has to be read and useful information about the file
	 * content returned. Errors have to be ignored.
	 * 
	 * The path meta property produces already file content information for resource files
	 * supported by the game engine. Subclass has to provide information for custom file formats.
	 */
	virtual void FileContentInformation(const ContextRef &context,
		const decString &path, decStringDictionary &info) const;
	/*@}*/
};


/**
 * \brief Path meta property using storage.
 */
class DE_DLL_EXPORT igdeMetaPropertyPathStorage : public igdeMetaPropertyPath{
public:
	/** \brief Storage type. */
	using Storage = igdeMetaPropertyStorageString<igdeMetaPropertyPathStorage>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create path meta property with label and description. */
	igdeMetaPropertyPathStorage(const char *id, const char *name, const char *description,
		igdeEnvironment::eFilePatternListTypes resourceType);
	
	/** \brief Create path meta property with label, description and custom file pattern list. */
	igdeMetaPropertyPathStorage(const char *id, const char *name, const char *description,
		const igdeFilePattern::List &customPatternList);
	
	/** \brief Create path meta property with label and description. */
	igdeMetaPropertyPathStorage(const char *id, const char *translationTag,
		igdeEnvironment::eFilePatternListTypes resourceType);
	
	/** \brief Create path meta property with label, description and custom file pattern list. */
	igdeMetaPropertyPathStorage(const char *id, const char *translationTag,
		const igdeFilePattern::List &customPatternList);
	
protected:
	/** \brief Clean up path meta property. */
	~igdeMetaPropertyPathStorage() override;
	
public:
	/*@}*/
	/** \brief Storage. */
	virtual Storage &GetStorage(const ContextRef &context) const = 0;
	
	
	const decString &GetPropertyValue(const ContextRef &context) const override;
	void SetPropertyValue(const ContextRef &context, const decString &value) override;
};


#include "undo/igdeMetaPropertyPathUndo.h"

#endif
