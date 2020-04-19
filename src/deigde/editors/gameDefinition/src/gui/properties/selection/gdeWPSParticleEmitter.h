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

#ifndef _GDEWPSPARTICLEEMITTER_H_
#define _GDEWPSPARTICLEEMITTER_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeTextAreaReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class gdeGameDefinition;
class gdeParticleEmitter;
class gdeCategoryList;
class gdeWindowProperties;
class gdeWPSParticleEmitterListener;



/**
 * \brief Particle emitter property panel.
 */
class gdeWPSParticleEmitter : public igdeContainerScroll{
private:
	gdeWindowProperties &pWindowProperties;
	gdeWPSParticleEmitterListener *pListener;
	
	gdeGameDefinition *pGameDefinition;
	
	igdeEditPathReference pEditPath;
	igdeTextFieldReference pEditName;
	igdeTextAreaReference pEditDescription;
	
	igdeComboBoxFilterReference pCBCategory;
	igdeButtonReference pBtnJumpToCategory;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSParticleEmitter( gdeWindowProperties &windowMain );
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPSParticleEmitter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Game definition or \em NULL if not set. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or \em NULL if not set. */
	void SetGameDefinition( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Active particle emitter or \em NULL if absent. */
	gdeParticleEmitter *GetParticleEmitter() const;
	
	
	
	/** \brief Update category list. */
	void UpdateCategoryList();
	
	/** \brief Update category list. */
	void UpdateCategoryList( const gdeCategoryList &list, const char *prefix );
	
	
	
	/** \brief Update particle emitter. */
	void UpdateParticleEmitter();
	/*@}*/
};

#endif
