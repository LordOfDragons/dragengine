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

#include "widget/state/igdeMetaPropertyWidgetState.h"
#include "../../utils/igdeFilter.h"
#include "../../utils/igdeTListenerList.h"
#include "../../gui/event/igdeAction.h"
#include "../../gui/menu/igdeMenuCascade.h"

#include <dragengine/common/string/decString.h>


class igdeMetaContext;
class igdeMetaPropertyWidget;
class igdeWidget;
class igdeTranslationManager;

/**
 * \brief Meta property.
 */
class DE_DLL_EXPORT igdeMetaProperty : public deObject{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaProperty>;
	
	/** \brief List of properties. */
	using List = decTObjectOrderedSet<igdeMetaProperty>;
	
	/** \brief Context reference. */
	using ContextRef = deTObjectReference<igdeMetaContext>;
	
	
	/** \brief Listener template. */
	template <typename P>
	class TListener : public deObject{
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
		virtual void OnValueChanged(P *property, const ContextRef &context){}
	};
	
	/** \brief Base action storing owner and dynamic context. */
	class DE_DLL_EXPORT Action : public igdeAction{
	public:
		/** \brief Reference type. */
		using Ref = deTObjectReference<Action>;
		
	private:
		igdeWidget &pOwner;
		ContextRef pContext;
		
	public:
		/** \brief Create action. */
		template <typename... A>
		Action(igdeWidget &owner, const ContextRef &context, A&&... args) :
		igdeAction(std::forward<A>(args)...),
		pOwner(owner), pContext(context){
		}
		
	protected:
		/** \brief Destructor. */
		~Action() override;
		
	public:
		/** \brief Context. */
		inline const ContextRef &GetContext() const{ return pContext; }
		
		/** \brief Set context. */
		void SetContext(const ContextRef &context);
		
		/** \brief Owner widget. */
		inline igdeWidget &GetOwner() const{ return pOwner; }
		
		/** \brief Environment. */
		igdeEnvironment &GetEnvironment() const;
		
		/** \brief Translation manager. */
		igdeTranslationManager &GetTranslationManager() const;
		
		/** \brief Translation for entry name entry name itself if absent. */
		decUnicodeString Translate(const decString &entryName) const;
		decUnicodeString Translate(const char *entryName) const;
		
		/** \brief Translate text if it starts with translation character '@'. */
		decUnicodeString TranslateIf(const decString &text) const;
		decUnicodeString TranslateIf(const char *text) const;
		
		/** \brief Build undo info. */
		decString BuildUndoInfo(const igdeMetaProperty &property) const;
	};
	
	/** \brief Help action. */
	class DE_DLL_EXPORT ActionHelp : public igdeAction{
	public:
		/** \brief Reference type. */
		using Ref = deTObjectReference<ActionHelp>;
		
	private:
		igdeEnvironment &pEnvironment;
		const igdeMetaProperty &pProperty;
		
	public:
		/** \brief Create action. */
		ActionHelp(igdeEnvironment &environment, const igdeMetaProperty &property);
		
	protected:
		/** \brief Destructor. */
		~ActionHelp() override;
		
	public:
		/** \brief Property. */
		inline const igdeMetaProperty &GetProperty() const{ return pProperty; }
		
		/** \brief Environment. */
		inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
		
		/** \brief Run action. */
		void OnAction() override;
	};
	
	
private:
	decString pId, pLabel, pDescription, pFilter, pUndoInfo, pClipboardDataTypeName, pHelpUrl;
	bool pHideLabel = false;
	bool pCanHideGroup = true;
	igdeMetaPropertyWidgetState::Ref pWidgetState;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/**
	 * \brief Create meta property with label and description.
	 */
	igdeMetaProperty(const char *id, const char *name, const char *description);
	
	/**
	 * \brief Create meta property with label, description, filter and undo info set from translation tag.
	 */
	igdeMetaProperty(const char *id, const char *translationTag);
	
protected:
	/** \brief Clean up meta property. */
	~igdeMetaProperty() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Identifier. */
	inline const decString &GetId() const{ return pId; }
	
	/** \brief Label shown in UI Forms for this property (usually '@translation'). */
	inline const decString &GetLabel() const{ return pLabel; }
	
	/** \brief Set label shown in UI Forms for this property (usually '@translation'). */
	void SetLabel(const char *label);
	
	/** \brief Description to show in UI as tooltip (usually '@translation'). */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description to show in UI as tooltip (usually '@translation'). */
	void SetDescription(const char *description);
	
	/**
	 * \brief Filter string or empty string to use label (usually '@translation').
	 * 
	 * If '@translation' is used the filter string is considered empty if the translation
	 * can not be found. This allows translators to provide a filter translation if they
	 * need to abbreviate the label without needing to provide it all the time.
	 */
	inline const decString &GetFilter() const{ return pFilter; }
	
	/** \brief Set filter string or empty string to use label (usually '@translation'). */
	void SetFilter(const char *filter);
	
	/**
	 * \brief Undo info string or empty string to use label (usually '@translation').
	 * 
	 * If '@translation' is used the undo info string is considered empty if the translation
	 * can not be found. This allows translators to provide an undo info translation if they
	 * need to abbreviate the label without needing to provide it all the time.
	 */
	inline const decString &GetUndoInfo() const{ return pUndoInfo; }
	
	/** \brief Set undo info string or empty string to use label (usually '@translation'). */
	void SetUndoInfo(const char *undoInfo);
	
	/** \brief Hide label in UI Forms. */
	inline bool GetHideLabel() const{ return pHideLabel; }
	
	/** \brief Set hide label in UI Forms. */
	void SetHideLabel(bool hideLabel);
	
	/** \brief Can hide group. */
	inline bool GetCanHideGroup() const{ return pCanHideGroup; }
	
	/** \brief Set can hide group. */
	void SetCanHideGroup(bool canHideGroup);
	
	/** \brief Clipboard data type name or empty string if not supported. */
	inline const decString &GetClipboardDataTypeName() const{ return pClipboardDataTypeName; }
	
	/** \brief Set clipboard data type name or empty string if not supported. */
	void SetClipboardDataTypeName(const char *name);
	
	/**
	 * \brief Help URL or empty string.
	 * 
	 * URL can be absolute or relative to IGDE wiki URL.
	 */
	inline const decString &GetHelpUrl() const{ return pHelpUrl; }
	
	/**
	 * \brief Set help URL or empty string if not supported.
	 * 
	 * URL can be absolute or relative to IGDE wiki URL.
	 */
	void SetHelpUrl(const char *url);
	
	/**
	 * \brief Real filter string.
	 * 
	 * Returns GetFilter() or GetLabel(). See GetFilter() for details.
	 */
	decString RealFilter(const igdeMetaContext &context) const;
	
	/**
	 * \brief Undo info string or empty to use label.
	 * 
	 * Returns GetUndoInfo() or GetLabel(). See GetUndoInfo() for details.
	 */
	decString RealUndoInfo(const igdeMetaContext &context) const;
	
	/** \brief Create action undo info in the form "<label>: <action.name>". */
	decString RealUndoInfo(const igdeMetaContext &context, const igdeAction &action) const;
	
	/** \brief Create action undo info in the form "<label>: <text>". */
	decString RealUndoInfo(const igdeMetaContext &context, const char *text) const;
	
	
	/** \brief Widget state. */
	inline const igdeMetaPropertyWidgetState::Ref &GetWidgetState() const{ return pWidgetState; }
	
	/** \brief Set widget state. */
	void SetWidgetState(const igdeMetaPropertyWidgetState::Ref &widgetState);
	
	
	/**
	 * \brief Create UI widget.
	 * 
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	virtual deTObjectReference<igdeMetaPropertyWidget> CreateWidget() = 0;
	
	/**
	 * \brief Add context menu entries.
	 * 
	 * Subclasses can override this method to add custom context menu entries.
	 */
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu,
		const ContextRef &context, igdeWidget &owner);
	/*@}*/
};

#endif
