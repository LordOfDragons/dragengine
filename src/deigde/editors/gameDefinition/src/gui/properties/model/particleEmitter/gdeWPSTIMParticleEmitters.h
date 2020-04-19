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

#ifndef _GDEWPSTIMPARTICLEEMITTERS_H_
#define _GDEWPSTIMPARTICLEEMITTERS_H_

#include "../gdeWPSTreeItemModel.h"

class gdeWPSTIMParticleEmitter;
class gdeParticleEmitter;


/**
 * \brief Particle emitters.
 */
class gdeWPSTIMParticleEmitters : public gdeWPSTreeItemModel{
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	gdeWPSTIMParticleEmitters( gdeWPSTreeModel &tree );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~gdeWPSTIMParticleEmitters();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Model with particle emitter or \em NULL. */
	gdeWPSTIMParticleEmitter *GetChildWith( gdeParticleEmitter *particleEmitter ) const;
	
	/** \brief Structure changed. */
	void StructureChanged();
	
	
	
	/** \brief Added to tree. */
	virtual void OnAddedToTree();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu( igdeMenuCascade &contextMenu );
	
	/** \brief Select object mest matching name. */
	virtual void SelectBestMatching( const char *string );
	/*@}*/
};

#endif
