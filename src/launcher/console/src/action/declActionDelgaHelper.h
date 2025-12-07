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

#ifndef _DECLACTIONDELGAHELPER_H_
#define _DECLACTIONDELGAHELPER_H_

#include <delauncher/game/delGameList.h>
#include <delauncher/game/patch/delPatchList.h>

#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/common/string/decString.h>

class declLauncher;

/**
 * \brief Action helper for handling delga files.
 */
class declActionDelgaHelper{
private:
	declLauncher &pLauncher;
	decString pFilename;
	delGameList pGames;
	delPatchList pPatches;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create action. */
	declActionDelgaHelper(declLauncher &launcher, const decString &filename);
	
	/** \brief Clean up action. */
	~declActionDelgaHelper();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Launcher. */
	inline declLauncher &GetLauncher() const{ return pLauncher; }
	
	/** \brief Filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** \brief Games. */
	inline const delGameList &GetGames() const{ return pGames; }
	
	/** \brief Patches. */
	inline const delPatchList &GetPatches() const{ return pPatches; }
	
	/** \brief Load delga file. */
	void Load();
	
	/** \brief Unload delga file. */
	void Unload();
	
	/** \brief Has any content. */
	bool HasContent() const;
	
	/** \brief Install delga file. */
	void Install();
	
	/** \brief Uninstall delga file. */
	void Uninstall();
	/*@}*/
};

#endif
