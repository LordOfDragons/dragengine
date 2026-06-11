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

#ifndef _IGDEMETAPROPERTY_H_
#define _IGDEMETAPROPERTY_H_

#include <functional>

#include "../igdeMetaContext.h"
#include "../../gui/menu/igdeMenuCascade.h"
#include "../../utils/igdeFilter.h"
#include "../../utils/igdeTListenerList.h"

#include <dragengine/common/string/decString.h>


class igdeMetaPropertyWidget;
class igdeWidget;


/**
 * \brief Meta property.
 */
class DE_DLL_EXPORT igdeMetaProperty : public deObject{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaProperty>;
	
	/** \brief List of properties. */
	using List = decTObjectOrderedSet<igdeMetaProperty>;
	
	
protected:
	/** \brief Listener template. */
	template <typename P>
	class DE_DLL_EXPORT TListener : public deObject{
	public:
		/** \brief Reference type. */
		using Ref = deTObjectReference<TListener<P>>;
		
		/** \brief Create listener. */
		TListener() = default;
		
	protected:
		/** \brief Destructor. */
		virtual ~TListener() = default;
		
	public:
		/** \brief Value changed. */
		virtual void OnValueChanged(P *property, const igdeMetaContext::Ref &context){}
	};
	
	
private:
	decString pId, pLabel, pDescription, pFilter, pUndoInfo;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/**
	 * \brief Create meta property with label and description.
	 */
	igdeMetaProperty(const char *id, const char *name, const char *description);
	
protected:
	/** \brief Clean up meta property. */
	~igdeMetaProperty() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Identifier. */
	inline const decString &GetId() const{ return pId; }
	
	/** \brief Label shown in UI Forms for this property. usually '@translation'. */
	inline const decString &GetLabel() const{ return pLabel; }
	
	/** \brief Set label shown in UI Forms for this property. usually '@translation'. */
	void SetLabel(const char *label);
	
	/** \brief Description to show in UI as tooltip. usually '@translation'. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description to show in UI as tooltip. usually '@translation'. */
	void SetDescription(const char *description);
	
	/** \brief Filter string or empty string to use label. */
	inline const decString &GetFilter() const{ return pFilter; }
	
	/** \brief Set filter string or empty string to use label. */
	void SetFilter(const char *filter);
	
	/** \brief Undo info string or empty string to use label. */
	inline const decString &GetUndoInfo() const{ return pUndoInfo; }
	
	/** \brief Set undo info string or empty string to use label. */
	void SetUndoInfo(const char *undoInfo);
	
	/**
	 * \brief Undo info string or empty to use label.
	 * 
	 * Returns undo info string if not empty or label if undo info string is empty.
	 */
	const decString &GetUndoInfoOrLabel() const;
	
	/**
	 * \brief Create UI widget.
	 * 
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	virtual deTObjectReference<igdeMetaPropertyWidget> CreateWidget(const igdeMetaContext::Ref &context) = 0;
	
	/**
	 * \brief Add context menu entries.
	 * 
	 * Subclasses can override this method to add custom context menu entries.
	 */
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu,
		const igdeMetaContext::Ref &context, igdeWidget &owner);
	/*@}*/
};

#endif
