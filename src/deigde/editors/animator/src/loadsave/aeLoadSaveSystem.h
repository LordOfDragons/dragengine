/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AELOADSAVESYSTEM_H_
#define _AELOADSAVESYSTEM_H_

#include <deigde/gui/filedialog/igdeFilePatternList.h>

class aeLoadSaveAttachmentConfig;
class aeLSAnimator;
class aeWindowMain;
class aeAnimator;
class igdeGameDefinition;
class igdeFilePatternList;



/**
 * Load/Save System for the editor.
 */
class aeLoadSaveSystem{
private:
	aeWindowMain *pWndMain;
	aeLSAnimator *pLSAnimator;
	
	aeLoadSaveAttachmentConfig *pLSAttConfig;
	igdeFilePatternList pFPAttConfig;
	
public:
	// constructor, destructor
	aeLoadSaveSystem( aeWindowMain *wndMain );
	~aeLoadSaveSystem();
	
	// actor animators management
	inline aeWindowMain *GetWindowMain() const{ return pWndMain; }
	inline aeLSAnimator *GetLSAnimator() const{ return pLSAnimator; }
	aeAnimator *LoadAnimator( const char *filename );
	void SaveAnimator( aeAnimator *animator, const char *filename );
	
	
	
	/** \brief Load save attachment configuration. */
	inline aeLoadSaveAttachmentConfig *GetLSAttConfig(){ return pLSAttConfig; }
	
	/** \brief Load attachment configuration from file. */
	void LoadAttConfig( const char *filename, aeAnimator &animator );
	
	/** \brief Save attachment configuration to file. */
	void SaveAttConfig( const char *filename, const aeAnimator &animator );
	
	/** \brief Attachment configuration file pattern list. */
	inline const igdeFilePatternList &GetAttConfigFilePatterns() const{ return pFPAttConfig; }
	
private:
	void pCleanUp();
	void pBuildFilePattern();
};

#endif
