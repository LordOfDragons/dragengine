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

#ifndef _IGDEMETACONTEXT_H_
#define _IGDEMETACONTEXT_H_

#include "igdeTMetaData.h"
#include "../gui/resources/igdeIcon.h"
#include "../utils/igdeTListenerList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/decString.h>

class igdeMetaProperty;
class igdeUndoSystem;


/**
 * \brief Meta context.
 * 
 * Meta contexts contain properties of some object or part of an editor. These properties are
 * typically shown in a property panel in the UI with unified look. Editors can provide multiple
 * meta contexts at the same time. Each context is shown as a separate section in the property
 * panel. Contexts use an identifier to track which section is the active one for each editor.
 */
class DE_DLL_EXPORT igdeMetaContext : public deObject{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaContext>;
	
	/** \brief Context data. */
	using Data = igdeTMetaData<decTObjectOrderedSet<igdeMetaContext>>;
	
	/** \brief List of properties. */
	using PropertyList = decTObjectOrderedSet<igdeMetaProperty>;
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public deObject{
	public:
		/** \brief Reference type. */
		using Ref = deTObjectReference<Listener>;
		
		/** \brief Create listener. */
		Listener();
		
	protected:
		/** \brief Destructor. */
		virtual ~Listener();
		
	public:
		/** \brief Properties changed. */
		virtual void OnPropertiesChanged(igdeMetaContext *context);
	};
	
	
private:
	decString pIdentifier, pLabel, pDescription;
	igdeIcon::Ref pIcon;
	PropertyList pProperties;
	igdeUndoSystem *pUndoSystem;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create meta context. */
	igdeMetaContext(const char *identifier);
	
protected:
	/** \brief Clean up meta context. */
	virtual ~igdeMetaContext() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Get identifier. */
	inline const decString &GetIdentifier() const{ return pIdentifier; }
	
	/** \brief Get label. */
	inline const decString &GetLabel() const{ return pLabel; }
	
	/** \brief Set label. */
	void SetLabel(const char *label);
	
	/** \brief Get description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription(const char *description);
	
	/** \brief Get icon. */
	inline const igdeIcon::Ref &GetIcon() const{ return pIcon; }
	
	/** \brief Set icon. */
	void SetIcon(const igdeIcon::Ref &icon);
	
	/**
	 * \brief Properties.
	 * 
	 * After changing list of properties call NotifyPropertiesChanged().
	 */
	inline PropertyList &GetProperties(){ return pProperties; }
	inline const PropertyList &GetProperties() const{ return pProperties; }
	
	/** \brief Get undo system or nullptr to apply actions immediately. */
	inline igdeUndoSystem *GetUndoSystem() const{ return pUndoSystem; }
	
	/** \brief Set undo system or nullptr to apply actions immediately. */
	void SetUndoSystem(igdeUndoSystem *undoSystem);
	
	
	/** \brief Listeners. */
	inline igdeTListenerList<Listener> &GetListeners(){ return pListeners; }
	inline const igdeTListenerList<Listener> &GetListeners() const{ return pListeners; }
	
	/** \brief Notify listeners about properties change. */
	void NotifyPropertiesChanged();
	/*@}*/
};

#endif
