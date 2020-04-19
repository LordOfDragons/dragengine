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

#ifndef _IGDELOADSAVESYSTEM_H_
#define _IGDELOADSAVESYSTEM_H_

#include <deigde/gui/filedialog/igdeFilePatternList.h>
#include <dragengine/common/string/decString.h>

class igdeLSGameProject;
class igdeGameProject;
class igdeWindowMain;

class deEngine;
class deLoadableModule;



/**
 * \brief Load Save System.
 */
class igdeLoadSaveSystem{
public:
	/** \brief File pattern lists. */
	enum eFilePatternLists{
		efplGameProject,
		
		efplAll,
		efplModel,
		efplSkin,
		efplRig,
		efplAnimation,
		efplImage,
		efplSound,
		efplParticleEmitter,
		efplOcclusionMesh,
		efplNavigationSpace,
		efplVideo,
		efplLanguagePack,
		efplFont,
		efplSky,
		efplAnimator,
		efplSynthesizer,
		efplSpeechAnimation
	};
	
	/** \brief FOX File pattern lists. */
	enum eFOXFilePatternLists{
		effplGameProject
	};
	
private:
	igdeWindowMain *pWindowMain;
	igdeLSGameProject *pLSGameProject;
	
	igdeFilePatternList pFPLOpen[ efplSpeechAnimation + 1 ];
	igdeFilePatternList pFPLSave[ efplSpeechAnimation + 1 ];
	
	decString pFoxFPLOpen[ effplGameProject + 1 ];
	decString pFoxFPLSave[ effplGameProject + 1 ];
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new load save system. */
	igdeLoadSaveSystem( igdeWindowMain *windowMain );
	/** \brief Cleans up the load save system. */
	~igdeLoadSaveSystem();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the main window. */
	inline igdeWindowMain *GetWindowMain() const{ return pWindowMain; }
	/** \brief Retrieves the load save game project. */
	inline igdeLSGameProject *GetLSGameProject() const{ return pLSGameProject; }
	
	/** \brief Loads a game project. */
	igdeGameProject *LoadGameProject( const char *filename );
	/** \brief Saves a game project. */
	void SaveGameProject( igdeGameProject *project, const char *filename );
	
	/** \brief Update pattern lists. */
	void UpdatePatternLists();
	
	/** \brief Retrieves a open file pattern list. */
	const igdeFilePatternList &GetOpenFilePatternList( eFilePatternLists list ) const;
	/** \brief Retrieves a save file pattern list. */
	const igdeFilePatternList &GetSaveFilePatternList( eFilePatternLists list ) const;
	
	/** \brief Retrieves a FOX open file pattern list. */
	const decString &GetFOXOpenFilePatternList( eFOXFilePatternLists list ) const;
	/** \brief Retrieves a FOX save file pattern list. */
	const decString &GetFOXSaveFilePatternList( eFOXFilePatternLists list ) const;
	/*@}*/
	
private:
	void pCleanUp();
	void pAddPattern( igdeFilePatternList &fpl, const deLoadableModule &module );
	void pAddPattern( igdeFilePatternList &fpl, const char *name, const char *pattern, const char *defaultExtension );
	void pConvertToFOX( const igdeFilePatternList &fpl, decString &foxfpl );
};

#endif
