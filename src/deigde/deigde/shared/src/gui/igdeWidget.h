/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _IGDEWIDGET_H_
#define _IGDEWIDGET_H_

#include "theme/igdeGuiThemeReference.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class igdeEnvironment;
class igdeContainer;
class igdeEngineController;
class igdeGameDefinition;
class igdeGameProject;

class deEngine;
class deLogger;



/**
 * \brief Base class for IGDE UI Widgets.
 */
class igdeWidget : public deObject{
private:
	igdeEnvironment &pEnvironment;
	void *pNativeWidget;
	igdeContainer *pParent;
	bool pVisible;
	igdeGuiThemeReference pGuiTheme;
	decString pGuiThemeName;
	
	
	
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	igdeWidget( igdeEnvironment &environment );
	
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeWidget();
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
	void SetVisible( bool visible );
	
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
	inline igdeGuiTheme *GetWidgetGuiTheme() const{ return pGuiTheme; }
	
	/** \brief Set widget specific GuiTheme or NULL to use parent GuiTheme. */
	void SetWidgetGuiTheme( igdeGuiTheme *guitheme );
	
	/** \brief Widget specific GuiTheme name or empty string to use parent GuiTheme. */
	inline const decString &GetWidgetGuiThemeName() const{ return pGuiThemeName; }
	
	/** \brief Set widget specific GuiTheme name or empty string to use parent GuiTheme. */
	void SetWidgetGuiThemeName( const char *guitheme );
	
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
	virtual decPoint WidgetToScreen( const decPoint &position ) const;
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
	void SetParent( igdeContainer *parent );
	
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
	void SetNativeWidget( void *nativeWidget );
	/*@}*/
};

#endif
