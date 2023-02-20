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

#ifndef _IGDELABEL_H_
#define _IGDELABEL_H_

#include "igdeWidget.h"
#include "resources/igdeIconReference.h"

#include <dragengine/common/string/decString.h>


/**
 * \brief IGDE UI Label with text and icon.
 */
class DE_DLL_EXPORT igdeLabel : public igdeWidget{
public:
	/** \brief Alignment. */
	enum eAlignment{
		/** \brief Center alignment. */
		eaCenter = 0,
		
		/** \brief Left alignment. */
		eaLeft = 0x1,
		
		/** \brief Right alignment. */
		eaRight = 0x2,
		
		/** \brief Middle alignment. */
		eaMiddle = 0,
		
		/** \brief Top alignment. */
		eaTop = 0x4,
		
		/** \brief Bottom alignment. */
		eaBottom = 0x8,
	};
	
	
	
private:
	decString pText;
	int pAlignment;
	decString pDescription;
	igdeIconReference pIcon;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create label. */
	igdeLabel( igdeEnvironment &environment, const char *text, int alignment = eaCenter | eaMiddle );
	
	/** \brief Create label. */
	igdeLabel( igdeEnvironment &environment, const char *text, const char *description,
		int alignment = eaCenter | eaMiddle );
	
	/** \brief Create label. */
	igdeLabel( igdeEnvironment &environment, const char *text, igdeIcon *icon,
		int alignment = eaCenter | eaMiddle );
	
	/** \brief Create label. */
	igdeLabel( igdeEnvironment &environment, const char *text, const char *description,
		igdeIcon *icon, int alignment = eaCenter | eaMiddle );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeLabel();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Text. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText( const char *text );
	
	/** \brief Alignment. */
	inline int GetAlignment() const{ return pAlignment; }
	
	/** \brief Set alignment. */
	void SetAlignment( int alignment );
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription( const char *description );
	
	/** \brief Icon or NULL. */
	inline igdeIcon *GetIcon() const{ return pIcon; }
	
	/** \brief Set icon or NULL. */
	void SetIcon( igdeIcon *icon );
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
	/** \brief Text changed. */
	virtual void OnTextChanged();
	
	/** \brief Alignment changed. */
	virtual void OnAlignmentChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	
	/** \brief Icon changed. */
	virtual void OnIconChanged();
	/*@}*/
};

#endif
