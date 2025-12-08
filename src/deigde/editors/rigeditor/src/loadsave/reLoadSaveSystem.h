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

// include only once
#ifndef _RELOADSAVESYSTEM_H_
#define _RELOADSAVESYSTEM_H_

// includes
#include <deigde/gui/filedialog/igdeFilePatternList.h>

// predefinitions
class reLSRig;
class reWindowMain;
class reRig;
class igdeGameDefinition;



/**
 * Load/Save System for the editor.
 */
class reLoadSaveSystem{
private:
	reWindowMain &pWindowMain;
	
	reLSRig **pLSRigs;
	int pLSRigCount;
	int pLSRigSize;
	
public:
	// constructor, destructor
	reLoadSaveSystem(reWindowMain &windowMain);
	~reLoadSaveSystem();
	
	// rigs management
	/** Retrieves the main window. */
	inline reWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** Retrieves the number of load save rigs. */
	inline int GetLSRigCount() const{ return pLSRigCount; }
	/** Retrieves the load save rig at the given index. */
	reLSRig *GetLSRigAt(int index) const;
	/** Retrieves the index of the load save rig. */
	int IndexOfLSRig(reLSRig *lsRig) const;
	/** Determines if the save rig exists. */
	bool HasLSRig(reLSRig *lsRig) const;
	/** Retrieves the index of the load save rig matching the given filename. */
	int IndexOfLSRigMatching(const char *filename);
	/** Adds a load save rig. */
	void AddLSRig(reLSRig *lsRig);
	/** Removes a load save rig. */
	void RemoveLSRig(reLSRig *lsRig);
	/** Removes all load save rigs. */
	void RemoveAllLSRigs();
	/** Updates the load save rig list from the engine. */
	void UpdateLSRigs();
	
	/** Loads the rig from file if possible. */
	reRig *LoadRig(const char *filename);
	/** Saves the rig to file if possible. */
	void SaveRig(reRig::Ref rig, const char *filename);
	
private:
	void pCleanUp();
};

// end of include only once
#endif
