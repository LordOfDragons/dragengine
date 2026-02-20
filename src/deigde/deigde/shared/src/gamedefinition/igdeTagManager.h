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

#ifndef _IGDETAGMANAGER_H_
#define _IGDETAGMANAGER_H_

#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/string/decStringSet.h>


/**
 * \brief List of string tags.
 */
class DE_DLL_EXPORT igdeTagManager{
private:
	decStringList pTags;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create tag manager. */
	igdeTagManager();
	
	/** \brief Create tag manager as a copy of another tag manager. */
	igdeTagManager(const igdeTagManager &tagManager);
	
	/** \brief Clean up tag manager. */
	~igdeTagManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of tags. */
	int GetTagCount() const;
	
	/** \brief Tag at the given index. */
	const decString &GetTagAt(int index) const;
	
	/** \brief Determines if the given tag exists. */
	bool HasTag(const char *tag) const;
	
	/** \brief Index of the given tag or -1 if not found. */
	int IndexOfTag(const char *tag) const;
	
	/** \brief Adds a new tag if not already existing. */
	void AddTag(const char *tag);
	
	/** \brief Removes the given tag if existing. */
	void RemoveTag(const char *tag);
	
	/** \brief Removes all tags. */
	void RemoveAllTags();
	
	/**
	 * \brief Determines if all tags of another tag manager are present in this tag manager.
	 * 
	 * If the other tag manager contains no tags false is returned.
	 */
	bool HasAllOf(const igdeTagManager &tags) const;
	
	/** \brief Determines if at least one tag of another tag manager is present in this tag manager. */
	bool HasAnyOf(const igdeTagManager &tags) const;
	
	/** \brief Determines if no tag of another tag manager is present in this tag manager. */
	bool HasNoneOf(const igdeTagManager &tags) const;
	
	/** \brief Adds tags from another tag manager if not existing yet. */
	void AddTagsFrom(const igdeTagManager &tags);
	
	/** \brief Get tags as string set. */
	decStringSet ToSet() const;
	/*@}*/
	
	
	
	/** \name Operators. */
	/*@{*/
	/** \brief Set tags to the same as another tag manager. */
	igdeTagManager &operator=(const igdeTagManager &tags);
	
	/** \brief Set tags. */
	igdeTagManager &operator=(const decStringSet &tags);
	
	/** \brief Get tags as string set. */
	operator decStringSet() const;
	/*@}*/
};

#endif
