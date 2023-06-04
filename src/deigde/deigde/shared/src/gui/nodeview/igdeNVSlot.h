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

#ifndef _IGDENVSLOT_H_
#define _IGDENVSLOT_H_

#include "../igdeContainer.h"

#include <dragengine/common/collection/decPointerSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class igdeNVNode;
class igdeNVLink;


/**
 * \brief IGDE UI NodeView Slot with text and icon.
 * 
 * Add widget to edit the default value using AddChild(). Depending on input type behaves
 * like igdeContainerFlow with X-Axis layout from left to right or reversed.
 */
class DE_DLL_EXPORT igdeNVSlot : public igdeContainer{
private:
	decString pText;
	decString pDescription;
	bool pEnabled;
	
	bool pIsInput;
	decColor pColor;
	
	igdeNVNode *pOwnerNode;
	decPointerSet pLinks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create checkbox. */
	igdeNVSlot( igdeEnvironment &environment, const char *text, bool isInput );
	
	/** \brief Create checkbox. */
	igdeNVSlot( igdeEnvironment &environment, const char *text, const char *description, bool isInput );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeNVSlot();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Text. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText( const char *text );
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription( const char *description );
	
	/** \brief Button is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if button is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Slot is an input slot (left side) or output slot (right side). */
	inline bool GetIsInput() const{ return pIsInput; }
	
	/** \brief Color indicating slot type. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set color indicating slot type. */
	void SetColor( const decColor &color );
	
	/** \brief Owner node or NULL. */
	inline igdeNVNode *GetOwnerNode() const{ return pOwnerNode; }
	
	/**
	 * \brief Set owner node or NULL.
	 * \warning For use by \em igdeNVNode only!
	 */
	void SetOwnerNode( igdeNVNode *node );
	
	
	
	/** \brief Number of links. */
	int GetLinkCount() const;
	
	/** \brief Link at index. */
	igdeNVLink *GetLinkAt( int index ) const;
	
	/** \brief Has link. */
	bool HasLink( igdeNVLink *link ) const;
	
	/**
	 * \brief Add link.
	 * \warning For use by \em igdeNVBoard only!
	 */
	void AddLink( igdeNVLink *link );
	
	/**
	 * \brief Remove link.
	 * \warning For use by \em igdeNVBoard only!
	 */
	void RemoveLink( igdeNVLink *link );
	
	/**
	 * \brief Remove all links.
	 * \warning For use by \em igdeNVBoard only!
	 */
	void RemoveAllLinks();
	
	
	
	/** \brief Center of slot in local window coordinates. */
	decPoint GetCenter() const;
	
	/** \brief Center of slot in owner node coordinates.. */
	decPoint GetCenterNode() const;
	
	/** \brief Center of slot in owner board coordinates.. */
	decPoint GetCenterBoard() const;
	
	/** \brief Connector position of slot in local window coordinates. */
	decPoint GetConnector() const;
	
	/** \brief Connector position of slot in owner node coordinates.. */
	decPoint GetConnectorNode() const;
	
	/** \brief Connector position of slot in owner board coordinates.. */
	decPoint GetConnectorBoard() const;
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
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Color changed. */
	virtual void OnColorChanged();
	
	/** \brief Linked slots changed. */
	virtual void OnLinksChanged();
	/*@}*/
};

#endif
