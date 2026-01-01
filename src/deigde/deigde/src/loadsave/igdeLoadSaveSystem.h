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

#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <dragengine/common/string/decString.h>

class igdeLSGameProject;
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
		efplCamera,
		efplWorld
	};
	
	static const int FilePatternListCount = efplWorld + 1;
	
	/** \brief FOX File pattern lists. */
	enum eFOXFilePatternLists{
		effplGameProject
	};
	
	static const int FoxFilePatternListCount = effplGameProject + 1;
	
	
private:
	igdeWindowMain *pWindowMain;
	igdeLSGameProject *pLSGameProject;
	
	igdeFilePattern::List pFPLOpen[FilePatternListCount];
	igdeFilePattern::List pFPLSave[FilePatternListCount];
	
	decString pFoxFPLOpen[FoxFilePatternListCount];
	decString pFoxFPLSave[FoxFilePatternListCount];
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new load save system. */
	explicit igdeLoadSaveSystem(igdeWindowMain *windowMain);
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
	igdeGameProject::Ref LoadGameProject(const char *filename);
	/** \brief Saves a game project. */
	void SaveGameProject(igdeGameProject *project, const char *filename);
	
	/** \brief Update pattern lists. */
	void UpdatePatternLists();
	
	/** \brief Retrieves a open file pattern list. */
	const igdeFilePattern::List &GetOpenFilePatternList(eFilePatternLists list) const;
	/** \brief Retrieves a save file pattern list. */
	const igdeFilePattern::List &GetSaveFilePatternList(eFilePatternLists list) const;
	
	/** \brief Retrieves a FOX open file pattern list. */
	const decString &GetFOXOpenFilePatternList(eFOXFilePatternLists list) const;
	/** \brief Retrieves a FOX save file pattern list. */
	const decString &GetFOXSaveFilePatternList(eFOXFilePatternLists list) const;
	/*@}*/
	
private:
	void pCleanUp();
	void pAddPattern(igdeFilePattern::List &fpl, const deLoadableModule &module);
	void pAddPattern(igdeFilePattern::List &fpl, const char *name, const char *pattern, const char *defaultExtension);
	void pConvertToFOX(const igdeFilePattern::List &fpl, decString &foxfpl);
};

#endif
