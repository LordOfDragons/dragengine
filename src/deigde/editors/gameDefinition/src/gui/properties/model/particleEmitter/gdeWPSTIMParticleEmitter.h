/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEWPSTIMPARTICLEEMITTER_H_
#define _GDEWPSTIMPARTICLEEMITTER_H_

#include "../gdeWPSTreeItemModel.h"

class gdeParticleEmitter;


/**
 * \brief Particle emitter.
 */
class gdeWPSTIMParticleEmitter : public gdeWPSTreeItemModel{
private:
	gdeParticleEmitter *pParticleEmitter;
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	gdeWPSTIMParticleEmitter( gdeWPSTreeModel &tree, gdeParticleEmitter *particleEmitter );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~gdeWPSTIMParticleEmitter();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Particle emitter. */
	inline gdeParticleEmitter *GetParticleEmitter() const{ return pParticleEmitter; }
	
	
	
	/** \brief Name changed. */
	void NameChanged();
	
	/**
	 * \brief Compare this item with another for sorting.
	 * 
	 * Default implementation returns 0 to keep same order.
	 */
	virtual int Compare( const gdeWPSTreeItemModel &item ) const;
	
	/** \brief User selected item. */
	virtual void OnSelected();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu( igdeMenuCascade &contextMenu );
	
	/** \brief Select object mest matching name. */
	virtual void SelectBestMatching( const char *string );
	/*@}*/
};

#endif
