/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
		efplSpeechAnimation,
		efplCamera
	};
	
	/** \brief FOX File pattern lists. */
	enum eFOXFilePatternLists{
		effplGameProject
	};
	
private:
	igdeWindowMain *pWindowMain;
	igdeLSGameProject *pLSGameProject;
	
	igdeFilePatternList pFPLOpen[ efplCamera + 1 ];
	igdeFilePatternList pFPLSave[ efplCamera + 1 ];
	
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
