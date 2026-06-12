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

#ifndef _IGDEMETAPROPERTYWIDGET_H_
#define _IGDEMETAPROPERTYWIDGET_H_

#include <functional>

#include "../igdeMetaProperty.h"
#include "../../igdeMetaContext.h"
#include "../../../gui/igdeButton.h"
#include "../../../gui/igdeLabel.h"
#include "../../../gui/event/igdeActionContextMenu.h"
#include "../../../gui/menu/igdeMenuCascade.h"

class igdeContainer;
class igdeUIHelper;
class igdeFilter;


/**
 * \brief Meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertyWidget : public deObject{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyWidget>;
	
	/** \brief List of property widgets. */
	using List = decTObjectOrderedSet<igdeMetaPropertyWidget>;
	
	
private:
	const igdeMetaProperty::Ref pProperty;
	const igdeMetaContext::Ref pContext;
	igdeLabel::Ref pLabel;
	igdeContainer::Ref pEditContainer;
	igdeButton::Ref pButtonContextMenu;
	igdeFilter::Matchable pMatchable;
	bool pFilteredOut = false;
	bool pPreventUpdate = false;
	
	
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property and context.
	 */
	igdeMetaPropertyWidget(igdeMetaProperty &property, const igdeMetaContext::Ref &context);
	
	/** \brief Clean up widget. */
	~igdeMetaPropertyWidget();
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Meta property associated with this widget. */
	inline const igdeMetaProperty::Ref &GetProperty() const{return pProperty;}
	
	/** \brief Meta context or nullptr. */
	inline const igdeMetaContext::Ref &GetContext() const{return pContext;}
	
	/** \brief Widget is filtered out. */
	inline bool GetFilteredOut() const{ return pFilteredOut; }
	
	/** \brief Set widget is filtered out. */
	void SetFilteredOut(bool filteredOut);
	
	/** \brief Filter widget. */
	virtual void Filter(const igdeFilter &filter);
	
	/** \brief Matchable. */
	inline const igdeFilter::Matchable &GetMatchable() const{ return pMatchable; }
	
	/** \brief Update matchable. */
	void UpdateMatchable(igdeContainer &container);
	
	/** \brief Create UI widgets adding them to container. */
	virtual void Create(igdeContainer &container, igdeUIHelper &helper) = 0;
	
	/** \brief Drop UI widgets. */
	virtual void Drop();
	
	/** \brief Update UI widgets with current property values. */
	virtual void Update() = 0;
	
	
	/** \brief Label or nullptr. */
	inline const igdeLabel::Ref &GetLabel() const{ return pLabel; }
	
	/** \brief Context menu button or nullptr. */
	inline const igdeButton::Ref &GetButtonContextMenu() const{ return pButtonContextMenu; }
	
	
	/** \brief Prevent create undo or update value when widget changes. */
	inline bool GetPreventUpdate() const{ return pPreventUpdate; }
	
	/** \brief Run function while preventing create undo or update value when widget changes. */
	void RunWithPreventUpdate(const std::function<void()> &function);
	
	
	/**
	 * \brief Add context menu entries.
	 * 
	 * Calls igdeMetaProperty::AddContextMenuEntries(). Subclass has to super call before
	 * adding menu entries to ensure special menu entries are located last in the menu.
	 */
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu);
	/*@}*/
	
	
protected:
	/** \brief Create label and context menu button wrapping edit widget. */
	void WrapEditWidget(igdeContainer &container, igdeUIHelper &helper,
		igdeWidget *widget, igdeWidget *sideWidget = nullptr);
	
	/** \brief Update filtered out. */
	virtual void UpdateFilteredOut();
};

#endif
