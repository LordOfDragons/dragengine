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

#ifndef _DECPATH_H_
#define _DECPATH_H_

#include "../string/decStringList.h"


/**
 * \brief File path.
 * 
 * This class provides an abstraction over file systems. A path is represented as an
 * optional prefix followed by 0 or more path components. A string path can be parsed
 * from a unix file system used by the engine or native file system used by the
 * operating system below.
 */
class decPath{
private:
	decStringList pComponents;
	decString pPrefix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty path. */
	decPath();
	
	/** \brief Create copy of path. */
	decPath(const decPath &path);
	
	/** \brief Clean up path. */
	~decPath();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create path from unix path. */
	static decPath CreatePathUnix(const char *path);
	
	/** \brief Create path from native path. */
	static decPath CreatePathNative(const char *path);
	
	/** \brief Working directory absolute path. */
	static decPath CreateWorkingDirectory();
	
	/**
	 * \brief Create absolute path of base directory and a potential relative path.
	 * 
	 * Used for loading files relative to a base path where the file path can be absolute
	 * or relative. If path is absolute a File instance of path is returned.  If path is
	 * relative a File instance of baseDirectory with path appended is returned.
	 * 
	 * This method is safe to be used with path containing '..' at the beginning.
	 */
	static decPath AbsolutePathUnix(const char *path, const char *baseDirectory);
	
	/**
	 * \brief Create absolute path of base directory and a potential relative path.
	 * 
	 * Used for loading files relative to a base path where the file path can be absolute
	 * or relative. If path is absolute a File instance of path is returned.  If path is
	 * relative a File instance of baseDirectory with path appended is returned.
	 * 
	 * This method is safe to be used with path containing '..' at the beginning.
	 */
	static decPath AbsolutePathNative(const char *path, const char *baseDirectory);
	
	/**
	 * \brief Create relative or absolute path of source path relative to base directory.
	 * \param[in] path Source path.
	 * \param[in] baseDirectory Base directory.
	 * \param[in] onlyBelow Allow only relative path below \em baseDirectory.
	 */
	static decPath RelativePathUnix(const char *path, const char *baseDirectory, bool onlyBelow = false);
	
	/**
	 * \brief Create relative or absolute path of source path relative to base directory.
	 * \param[in] path Source path.
	 * \param[in] baseDirectory Base directory.
	 * \param[in] onlyBelow Allow only relative path below \em baseDirectory.
	 */
	static decPath RelativePathNative(const char *path, const char *baseDirectory, bool onlyBelow = false);
	
	/** \brief Path prefix. */
	inline const decString &GetPrefix() const{ return pPrefix; }
	
	/**
	 * \brief Set path prefix.
	 * \throws deeInvalidParam \em prefix is NULL.
	 */
	void SetPrefix(const char *prefix);
	
	/**
	 * \brief Set path from the given string path in unix form.
	 * \throws deeInvalidParam \em path is NULL.
	 */
	void SetFromUnix(const char *path);
	
	/**
	 * \brief Set from the given string path in native form.
	 * \throws deeInvalidParam \em path is NULL.
	 */
	void SetFromNative(const char *path);
	
	/** \brief Set to working directory. */
	void SetWorkingDirectory();
	
	/** \brief Set empty path. */
	void SetEmpty();
	
	/** \brief Path in unix form. */
	decString GetPathUnix() const;
	
	/** \brief Path in native form. */
	decString GetPathNative() const;
	
	/** \brief Path is empty. */
	bool IsEmpty() const;
	
	/**
	 * \brief Path is absolute.
	 * 
	 * A path is absolute if it does have a prefix.
	 */
	bool IsAbsolute() const;
	
	/**
	 * \brief Path is relative.
	 * 
	 * A path is absolute if it does have a prefix.
	 */
	bool IsRelative() const;
	
	/** \brief Copy another path to this path. */
	void SetFrom(const decPath &path);
	
	/** \brief Add path to this path. */
	void Add(const decPath &path);
	
	/**
	 * \brief Add unix path to this path.
	 * \throws deeInvalidParam \em path is NULL.
	 */
	void AddUnixPath(const char *path);
	
	/**
	 * \brief Add native path to this path.
	 * \throws deeInvalidParam \em path is NULL.
	 */
	void AddNativePath(const char *path);
	
	/** \brief Path matches file pattern. */
	bool MatchesPattern(const decPath &filePattern) const;
	
	/**
	 * \brief Create absolute path of base directory and a potential relative path.
	 * 
	 * Used for loading files relative to a base path where the file path can be absolute
	 * or relative. If path is absolute a File instance of path is returned.  If path is
	 * relative a File instance of baseDirectory with path appended is returned.
	 * 
	 * This method is safe to be used with path containing '..' at the beginning.
	 */
	decPath AbsolutePath(const decPath &baseDirectory) const;
	
	/**
	 * \brief Relative path of source path relative to base directory if possible.
	 * \note Relative path is returned it possible otherwise copy of this path.
	 * \param[in] baseDirectory Base directory.
	 * \param[in] onlyBelow Allow only relative path below \em baseDirectory.
	 */
	decPath RelativePath(const decPath &baseDirectory, bool onlyBelow = false) const;
	/*@}*/
	
	
	
	/** \name Component Management */
	/*@{*/
	/** \brief Number of path components. */
	int GetComponentCount() const;
	
	/**
	 * \brief Component at index.
	 * \throws deeInvalidParam \em index is less than 0.
	 * \throws deeInvalidParam \em index is larger than or equal to GetComponentCount().
	 */
	const decString &GetComponentAt(int index) const;
	
	/**
	 * \brief Set component at index.
	 * \throws deeInvalidParam \em index is less than 0.
	 * \throws deeInvalidParam \em index is larger than or equal to GetComponentCount().
	 */
	void SetComponentAt(int index, const char *component);
	
	/**
	 * \brief Last path component.
	 * \throws deeInvalidParam Path is empty.
	 */
	const decString &GetLastComponent() const;
	
	/**
	 * \brief Set last path component.
	 * \throws deeInvalidParam Path is empty.
	 */
	void SetLastComponent(const char *component);
	
	/**
	 * \brief Add component.
	 * \throws deeInvalidParam \em component is NULL.
	 */
	void AddComponent(const char *component);
	
	/**
	 * \brief Remove component from index.
	 * \throws deeInvalidParam \em index is less than 0.
	 * \throws deeInvalidParam \em index is larger than or equal to GetComponentCount().
	 */
	void RemoveComponentFrom(int index);
	
	/**
	 * \brief Remove last component.
	 * \throws deeInvalidParam Path is empty.
	 */
	void RemoveLastComponent();
	
	/** \brief Remove all components. */
	void RemoveAllComponents();
	
	/** \brief Path has components. */
	bool HasComponents() const;
	/*@}*/
	
	
	
	/** \name String Path Testing */
	/*@{*/
	/**
	 * \brief Unix path is absolute.
	 * \throws deeInvalidParam \em path is NULL.
	 */
	static bool IsUnixPathAbsolute(const char *path);
	
	/**
	 * \brief Native path is absolute.
	 * \throws deeInvalidParam \em path is NULL.
	 */
	static bool IsNativePathAbsolute(const char *path);
	/*@}*/
	
	
	
	/** \name Constants */
	/*@{*/
	/** \brief Platform specific  path separator character. */
	static char PathSeparator();
	
	/** \brief Platform specific  path separator string. */
	static const char *PathSeparatorString();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Path is equal to another path. */
	bool operator==(const decPath &path) const;
	
	/** \brief Path is not equal to another path. */
	bool operator!=(const decPath &path) const;
	
	/** \brief Copy another path to this path. */
	decPath &operator=(const decPath &path);
	
	/** \brief Add another path to this path. */
	decPath &operator+=(const decPath &path);
	
	/** \brief Concatenation of path with another path. */
	decPath operator+(const decPath &path) const;
	/*@}*/
	
	
	
private:
	/** \brief Parse relative path. */
	void pParseRelativePath(const char *filename, int separator);
	
	/** \brief Get prefix length. */
	int pGetPrefixLength(const char *path) const;
	
	/**
	 * \brief File matching function borrowed from the fox toolkit ( www.fox-toolkit.org ).
	 * 
	 * Original file is fxfilematch.cpp .
	 */
	static bool domatch(const char *pattern,const char *string, int flags);
	static bool fxfilematch(const char *pattern,const char *string, int flags);
};

#endif
