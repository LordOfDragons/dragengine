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

#ifndef _LPELOADSAVESYSTEM_H_
#define _LPELOADSAVESYSTEM_H_

#include <deigde/gui/filedialog/igdeFilePatternList.h>

class lpeLoadSaveLangPack;
class lpeWindowMain;
class lpeLangPack;



/**
 * @brief Load/Save System.
 */
class lpeLoadSaveSystem{
private:
	lpeWindowMain *pWindowMain;
	
	lpeLoadSaveLangPack **pLSLangPacks;
	int pLSLangPackCount;
	int pLSLangPackSize;
	
	igdeFilePatternList pFPListLangPack;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new load/save system object. */
	lpeLoadSaveSystem(lpeWindowMain *windowMain);
	/** Cleans up the load/save system object. */
	~lpeLoadSaveSystem();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the main window. */
	inline lpeWindowMain *GetWindowMain() const{ return pWindowMain; }
	
	/** Retrieves the number of load save langpacks. */
	inline int GetLSLangPackCount() const{ return pLSLangPackCount; }
	/** Retrieves the load save langpack at the given index. */
	lpeLoadSaveLangPack *GetLSLangPackAt(int index) const;
	/** Retrieves the index of the load save langpack. */
	int IndexOfLSLangPack(lpeLoadSaveLangPack *lsLangPack) const;
	/** Determines if the save langpack exists. */
	bool HasLSLangPack(lpeLoadSaveLangPack *lsLangPack) const;
	/** Retrieves the index of the load save langpack matching the given filename. */
	int IndexOfLSLangPackMatching(const char *filename);
	/** Adds a load save langpack. */
	void AddLSLangPack(lpeLoadSaveLangPack *lsLangPack);
	/** Removes a load save langpack. */
	void RemoveLSLangPack(lpeLoadSaveLangPack *lsLangPack);
	/** Removes all load save langpacks. */
	void RemoveAllLSLangPacks();
	/** Updates the load save langpack list from the engine. */
	void UpdateLSLangPacks();
	
	/** Loads the langpack from file if possible. */
	lpeLangPack::Ref LoadLangPack(const char *filename);
	/** Saves the langpack to file if possible. */
	void SaveLangPack(lpeLangPack *langpack, const char *filename);
	
	/** \brief File pattern list. */
	inline const igdeFilePatternList &GetLangPackFPList() const{ return pFPListLangPack; }
	/*@}*/
	
private:
	void pRebuildFPListLangPack();
};

#endif
