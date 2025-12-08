/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDEWIDGET_H_
#define _IGDEWIDGET_H_

#include "theme/igdeGuiTheme.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class igdeEnvironment;
class igdeContainer;
class igdeEngineController;
class igdeGameDefinition;
class igdeGameProject;
class igdeWindow;

class deEngine;
class deLogger;


/**
 * \brief Base class for IGDE UI Widgets.
 */
class DE_DLL_EXPORT igdeWidget : public deObject{
public:
	/** \brief Strong reference. */
	typedef deTObjectReference<igdeWidget> Ref;
	
	
private:
	igdeEnvironment &pEnvironment;
	void *pNativeWidget;
	igdeContainer *pParent;
	bool pVisible;
	igdeGuiTheme::Ref pGuiTheme;
	decString pGuiThemeName;
	
	
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	igdeWidget(igdeEnvironment &environment);
	
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	~igdeWidget() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Parent of widget or NULL. */
	inline igdeContainer *GetParent() const{ return pParent; }
	
	/** \brief Widget is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** \brief Set if widget is visible. */
	void SetVisible(bool visible);
	
	/** \brief Engine controller. */
	igdeEngineController &GetEngineController() const;
	
	/** \brief Active game project. */
	igdeGameProject *GetGameProject() const;
	
	/** \brief Game definition of the active project. */
	igdeGameDefinition *GetGameDefinition() const;
	
	/** \brief Engine. */
	deEngine *GetEngine() const;
	
	/** \brief Logger. */
	deLogger *GetLogger() const;
	
	
	/** \brief Widget specific GuiTheme or NULL to use parent GuiTheme. */
	inline const igdeGuiTheme::Ref &GetWidgetGuiTheme() const{ return pGuiTheme; }
	
	/** \brief Set widget specific GuiTheme or NULL to use parent GuiTheme. */
	void SetWidgetGuiTheme(igdeGuiTheme *guitheme);
	
	/** \brief Widget specific GuiTheme name or empty string to use parent GuiTheme. */
	inline const decString &GetWidgetGuiThemeName() const{ return pGuiThemeName; }
	
	/** \brief Set widget specific GuiTheme name or empty string to use parent GuiTheme. */
	void SetWidgetGuiThemeName(const char *guitheme);
	
	/**
	 * \brief GuiTheme to use.
	 * 
	 * Returns the first GuiTheme which is not NULL:
	 * - Widget specific GuiTheme
	 * - Named widget specific GuiTheme
	 * - Parent GuiThemes (walking up the parent chain)
	 * - Environment GuiTheme.
	 */
	igdeGuiTheme *GetGuiTheme() const;
	
	
	/** \brief Widget size changed. */
	virtual void OnResize();
	
	
	/** \brief Convert widget position to screen position. */
	virtual decPoint WidgetToScreen(const decPoint &position) const;
	
	/** \brief Parent window. */
	virtual igdeWindow *GetParentWindow();
	/*@}*/
	
	
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Set parent.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	void SetParent(igdeContainer *parent);
	
	/**
	 * \brief Create native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void CreateNativeWidget() = 0;
	
	/**
	 * \brief Destroy native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DestroyNativeWidget() = 0;
	
	/**
	 * \brief Drop native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DropNativeWidget();
	
	/**
	 * \brief Get native widget pointer.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	inline void *GetNativeWidget() const{ return pNativeWidget; }
	
	/** \brief GuiTheme changed. */
	virtual void OnGuiThemeChanged();
	
	
protected:
	/** \brief Visible changed. */
	virtual void OnVisibleChanged();
	
	/**
	 * \brief Set native widget pointer.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	void SetNativeWidget(void *nativeWidget);
	/*@}*/
};

#endif
