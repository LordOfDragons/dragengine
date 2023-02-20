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

#ifndef _IGDEGROUPBOX_H_
#define _IGDEGROUPBOX_H_

#include "igdeContainer.h"

#include <dragengine/common/string/decString.h>


/**
 * \brief IGDE UI Container decorating one widget with a titled border.
 */
class DE_DLL_EXPORT igdeGroupBox : public igdeContainer{
public:
	/** \brief Title alignment. */
	enum eTitleAlignment{
		/** \brief Left alignment. */
		etaLeft,
		
		/** \brief Right alignment. */
		etaRight,
		
		/** \brief Center alignment. */
		etaCenter
	};
	
	
	
private:
	decString pTitle;
	eTitleAlignment pTitleAlignment;
	decString pDescription;
	bool pCanCollapse;
	bool pCollapsed;
	bool pStretchLast;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create groupbox. */
	igdeGroupBox( igdeEnvironment &environment, const char *title,
		eTitleAlignment titleAlignment = etaLeft );
	
	/** \brief Create groupbox. */
	igdeGroupBox( igdeEnvironment &environment, const char *title, const char *description,
		eTitleAlignment titleAlignment = etaLeft );
	
	/** \brief Create groupbox. */
	igdeGroupBox( igdeEnvironment &environment, const char *title, bool collapsed,
		eTitleAlignment titleAlignment = etaLeft );
	
	/** \brief Create groupbox. */
	igdeGroupBox( igdeEnvironment &environment, const char *title, const char *description,
		bool collapsed, eTitleAlignment titleAlignment = etaLeft );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeGroupBox();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Title. */
	inline const decString &GetTitle() const{ return pTitle; }
	
	/** \brief Set title. */
	void SetTitle( const char *title );
	
	/** \brief Title alignment. */
	inline eTitleAlignment GetTitleAlignment() const{ return pTitleAlignment; }
	
	/** \brief Set title alignment. */
	void SetTitleAlignment( eTitleAlignment alignment );
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription( const char *description );
	
	/** \brief Group box can be collapsed by clicking on the title. */
	inline bool GetCanCollapse() const{ return pCanCollapse; }
	
	/** \brief Set if group box can be collapsed by clicking on the title. */
	void SetCanCollapse( bool canCollapse );
	
	/** \brief Group box is collapsed. */
	inline bool GetCollapsed() const{ return pCollapsed; }
	
	/** \brief Set if group box is collapsed. */
	void SetCollapsed( bool collapsed );
	
	/** \brief Stretch last widget. */
	inline bool GetStretchLast() const{ return pStretchLast; }
	
	/** \brief Set stretch last widget. */
	void SetStretchLast( bool stretchLast );
	
	
	
	/** \brief Add child. */
	virtual void AddChild( igdeWidget *child );
	
	/** \brief Remove child. */
	virtual void RemoveChild( igdeWidget *child );
	/*@}*/
	
	
	
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Create native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void CreateNativeWidget();
	
	/**
	 * \brief Destroy native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DestroyNativeWidget();
	
	
	
protected:
	/** \brief Title changed. */
	virtual void OnTitleChanged();
	
	/** \brief Title alignment changed. */
	virtual void OnTitleAlignmentChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	
	/** \brief Can collapsed changed. */
	virtual void OnCanCollapseChanged();
	
	/** \brief Collapsed changed. */
	virtual void OnCollapsedChanged();
	
	/** \brief Stretch last changed. */
	virtual void OnStretchLastChanged();
	/*@}*/
};

#endif
