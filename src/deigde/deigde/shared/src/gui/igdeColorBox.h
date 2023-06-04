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

#ifndef _IGDECOLORBOX_H_
#define _IGDECOLORBOX_H_

#include "igdeWidget.h"
#include "event/igdeAction.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class igdeColorBoxListener;


/**
 * \brief IGDE UI Box with color showing a color picker dialog if clicked.
 */
class DE_DLL_EXPORT igdeColorBox : public igdeWidget{
public:
	class DE_DLL_EXPORT cActionCopy : public igdeAction{
		igdeColorBox &pColorBox;
	public:
		cActionCopy( igdeColorBox &colorBox );
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionCopyHex : public igdeAction{
		igdeColorBox &pColorBox;
	public:
		cActionCopyHex( igdeColorBox &colorBox );
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionPaste : public igdeAction{
		igdeColorBox &pColorBox;
	public:
		cActionPaste( igdeColorBox &colorBox );
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionPasteHex : public igdeAction{
		igdeColorBox &pColorBox;
	public:
		cActionPasteHex( igdeColorBox &colorBox );
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionEditValues : public igdeAction{
		igdeColorBox &pColorBox;
	public:
		cActionEditValues( igdeColorBox &colorBox );
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionEditHex : public igdeAction{
		igdeColorBox &pColorBox;
	public:
		cActionEditHex( igdeColorBox &colorBox );
		virtual void OnAction();
	};
	
	
	
private:
	decString pDescription;
	bool pEnabled;
	decColor pColor;
	
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create color box. */
	igdeColorBox( igdeEnvironment &environment );
	
	/** \brief Create color box. */
	igdeColorBox( igdeEnvironment &environment, const char *description );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeColorBox();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription( const char *description );
	
	/** \brief ColorBox is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if color box is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set color. */
	void SetColor( const decColor &color );
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeColorBoxListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( igdeColorBoxListener *listener );
	
	/** \brief Notify listeners color changed. */
	virtual void NotifyColorChanged();
	
	/** \brief Show context menu at position. */
	virtual void ShowContextMenu( const decPoint &position );
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
	/** \brief Color changed. */
	virtual void OnColorChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	/*@}*/
};

#endif
