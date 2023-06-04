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

#ifndef _IGDEDIALOGBROWSERPARTICLEEMITTER_H_
#define _IGDEDIALOGBROWSERPARTICLEEMITTER_H_

#include "igdeDialogBrowser.h"


class igdeGDParticleEmitter;


/**
 * \brief Dialog for user to browse particle emitter.
 */
class DE_DLL_EXPORT igdeDialogBrowserParticleEmitter : public igdeDialogBrowser{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	igdeDialogBrowserParticleEmitter( igdeEnvironment &environment, const char *title = "Select ParticleEmitter" );
	
protected:
	/** \brief Clean up selection dialog. */
	virtual ~igdeDialogBrowserParticleEmitter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Selected particle emitter. */
	igdeGDParticleEmitter *GetSelectedParticleEmitter() const;
	
	/** \brief Set selected particle emitter. */
	void SetSelectedParticleEmitter( igdeGDParticleEmitter *gdParticleEmitter );
	
	
	
	/** \brief Convenience method to select particle emitter. */
	static bool SelectParticleEmitter( igdeWidget *owner, igdeGDParticleEmitter* &particleEmitter,
		const char *title = "Select ParticleEmitter" );
	
	/** \brief Convenience method to select particle emitter. */
	static bool SelectParticleEmitter( igdeWidget *owner, decString &particleEmitter,
		const char *title = "Select ParticleEmitter" );
	/*@}*/
	
	
	
protected:
	virtual igdeGDCategory *GetRootCategory() const;
	virtual void AddItemsToList( igdeGDAddToListVisitor &visitor );
	virtual void RebuildItemPreview( igdeGDPreviewManager &pvmgr, igdeGDPreviewListener *listener );
	virtual void GetSelectedItemInfo( decString &info );
};

#endif
