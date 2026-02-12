/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDETRANSLATIONMANAGER_H_
#define _IGDETRANSLATIONMANAGER_H_

#include "igdeLanguagePack.h"
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

class igdeEnvironment;
class decPath;


/**
 * \brief Translation manager.
 */
class DE_DLL_EXPORT igdeTranslationManager{
public:
	/** \brief Fallback language. */
	static const char *FallbackLanguage;
	
	
private:
	igdeEnvironment &pEnvironment;
	igdeLanguagePack::List pIgdeLanguagePacks;
	decString pActiveLanguage;
	igdeLanguagePack::List pActiveLanguagePacks;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create translation manager. */
	igdeTranslationManager(igdeEnvironment &environment);
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief IGDE language packs. */
	inline const igdeLanguagePack::List &GetIgdeLanguagePacks() const{ return pIgdeLanguagePacks; }
	
	/** \brief Active language. */
	inline const decString &GetActiveLanguage() const{ return pActiveLanguage; }
	
	/** \brief Active language packs. */
	inline const igdeLanguagePack::List &GetActiveLanguagePacks() const{ return pActiveLanguagePacks; }
	
	/** \brief Load IGDE language packs. */
	void LoadIgdeLanguagePacks(const decPath &path);
	
	/** \brief Load language packs. */
	void LoadLanguagePacks(const decPath &path, igdeLanguagePack::List &languagePacks) const;
	
	/** \brief Set active language. */
	void SetActiveLanguage(const decString &language);
	
	/** \brief Add active language pack. */
	void AddActiveLanguagePack(const igdeLanguagePack::Ref &languagePack);
	
	/** \brief Translation for entry name entry name itself if absent. */
	decUnicodeString Translate(const decString &entryName) const;
	decUnicodeString Translate(const char *entryName) const;
	
	/** \brief Translation for entry name or default value if absent. */
	decUnicodeString Translate(const decString &entryName, const decUnicodeString &defaultValue) const;
	decUnicodeString Translate(const char *entryName, const decUnicodeString &defaultValue) const;
	
	/**
	 * \brief Translate text if it starts with a translation character.
	 *
	 * This call can be used for plain text and text prefixed with a marker
	 * character. If text is empty or the first character in text does not
	 * match the translation character the text is returned unmodified.
	 * Otherwise the entire text after the translation character converted to
	 * UTF-8 encoding is used as name in a call to translate(String).
	 * 
	 * Choose the marker character to not be found at the start of any text
	 * you can possibly encounter. A typical choice is '@'.
	 */
	decUnicodeString TranslateIf(const decUnicodeString &text, int translationCharacter = '@') const;
	decUnicodeString TranslateIf(const char *text, int translationCharacter = '@') const;
	
	/**
	 * \brief Translate text if it starts with a translation character.
	 *
	 * This call can be used for plain text and text prefixed with a marker
	 * character. If text is empty or the first character in text does not
	 * match the translation character the text is returned unmodified.
	 * Otherwise the entire text after the translation character converted to
	 * UTF-8 encoding is used as name in a call to translate(String, UnicodeString).
	 * 
	 * Choose the marker character to not be found at the start of any text
	 * you can possibly encounter. A typical choice is '@'.
	 */
	decUnicodeString TranslateIf(const decUnicodeString &text, const decUnicodeString &defaultValue,
		int translationCharacter = '@') const;
	
	decUnicodeString TranslateIf(const char *text, const decUnicodeString &defaultValue,
		int translationCharacter = '@') const;
	/*@}*/
};

#endif
