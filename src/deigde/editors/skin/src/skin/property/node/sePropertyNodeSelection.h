/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SEPROPERTYNODESELECTION_H_
#define _SEPROPERTYNODESELECTION_H_

#include "sePropertyNodeList.h"

class seProperty;



/**
 * \brief Texture property selected nodes.
 */
class sePropertyNodeSelection{
private:
	seProperty &pProperty;
	sePropertyNodeList pSelection;
	sePropertyNode *pActive;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create selection. */
	sePropertyNodeSelection( seProperty &property );
	
	/** \brief Clean up selection. */
	~sePropertyNodeSelection();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief List of selected nodes. */
	inline const sePropertyNodeList &GetSelected() const{ return pSelection; }
	
	/** \brief Add node to selection if absent. */
	void Add( sePropertyNode *node );
	
	/** \brief Remove node from selection if present. */
	void Remove( sePropertyNode *node );
	
	/** \brief Remove all nodes from selection. */
	void RemoveAll();
	
	/** \brief Set list of selected nodes. */
	void SetSelected( const sePropertyNodeList &list );
	
	
	
	/** \brief Active node or \em NULL if absent. */
	inline sePropertyNode *GetActive() const{ return pActive; }
	
	/** \brief Active node is present. */
	bool HasActive() const;
	
	/** \brief Set active node or \em NULL if absent. */
	void SetActive( sePropertyNode *navspaces );
	
	
	
	/** \brief Notify active node changed. */
	void NotifyActiveNodeChanged() const;
	
	/** \brief Notify node selection changed. */
	void NotifyNodeSelectionChanged() const;
};

#endif
