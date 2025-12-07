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

#ifndef _DELPATCH_H_
#define _DELPATCH_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/utils/decUuid.h>
#include <dragengine/common/utils/decUuidSet.h>


/**
 * \brief Game patch.
 */
class DE_DLL_EXPORT delPatch : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delPatch> Ref;
	
	
private:
	decUuid pIdentifier;
	decUnicodeString pName;
	decUnicodeString pDescription;
	decUnicodeString pCreator;
	decString pHomepage;
	decString pPatchDirectory;
	decString pDataDirectory;
	
	decUuid pGameID;
	decUuidSet pRequiresPatches;
	
	decString pDelgaFile;
	decStringSet pHiddenPath;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game patch. */
	delPatch();
	
protected:
	/** \brief Clean up game aptch. */
	virtual ~delPatch();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Identifier. */
	inline const decUuid &GetIdentifier() const{ return pIdentifier; }
	
	/** \brief Set identifier. */
	void SetIdentifier(const decUuid &identifier);
	
	/** \brief Display name. */
	inline const decUnicodeString &GetName() const{ return pName; }
	
	/** \brief Set display name. */
	void SetName(const decUnicodeString &name);
	
	/** \brief Description. */
	inline const decUnicodeString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription(const decUnicodeString &description);
	
	/** \brief Creator. */
	inline const decUnicodeString &GetCreator() const{ return pCreator; }
	
	/** \brief Set creator. */
	void SetCreator(const decUnicodeString &creator);
	
	/** \brief Homepage. */
	inline const decString &GetHomepage() const{ return pHomepage; }
	
	/** \brief Set homepage. */
	void SetHomepage(const char *homepage);
	
	/** \brief Patch directory. */
	inline const decString &GetPatchDirectory() const{ return pPatchDirectory; }
	
	/** \brief Set patch dir. */
	void SetPatchDirectory(const char *directory);
	
	/** \brief Data directory. */
	inline const decString &GetDataDirectory() const{ return pDataDirectory; }
	
	/** \brief Set data dir. */
	void SetDataDirectory(const char *directory);
	
	/** \brief Game identifier. */
	inline const decUuid &GetGameID() const{ return pGameID; }
	
	/** \brief Set game identifier. */
	void SetGameID(const decUuid &id);
	
	/** \brief Required patch identifiers. */
	inline decUuidSet &GetRequiredPatches(){ return pRequiresPatches; }
	inline const decUuidSet &GetRequiredPatches() const{ return pRequiresPatches; }
	
	/** \brief Delga file or empty string. */
	inline const decString &GetDelgaFile() const{ return pDelgaFile; }
	
	/** \brief Set delga file or empty string. */
	void SetDelgaFile(const char *file);
	
	/**
	 * \brief Set of path to hide.
	 * 
	 * Hidden path allows to define files deleted by a patch. Hidden path have to be
	 * absolute path to a single file or directory. Wildcards are not supported.
	 */
	inline decStringSet &GetHiddenPath(){ return pHiddenPath; }
	inline const decStringSet &GetHiddenPath() const{ return pHiddenPath; }
	/*@}*/
};

#endif
