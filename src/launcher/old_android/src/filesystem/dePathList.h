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

#ifndef _DEPATHLIST_H_
#define _DEPATHLIST_H_

class decPath;


/**
 * \brief List of path.
 */
class dePathList{
private:
	decPath **pPath;
	int pPathCount;
	int pPathSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create path list. */
	dePathList();
	
	/** \brief Create path list. */
	dePathList(const dePathList &list);
	
	/** \brief Clean up path list. */
	~dePathList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of path. */
	inline int GetCount() const{return pPathCount;}
	
	/** \brief Path at index. */
	const decPath &GetAt(int index) const;
	
	/** \brief Index of the path or -1 if absent. */
	int IndexOf(const decPath &path) const;
	
	/** \brief Path is present. */
	bool Has(const decPath &path) const;
	
	/** \brief Add path. */
	void Add(const decPath &path);
	
	/** \brief Add path if absent. */
	void AddIfAbsent(const decPath &path);
	
	/** \brief Remove path. */
	void Remove(const decPath &path);
	
	/** \brief Remove path if present. */
	void RemoveIfPresent(const decPath &path);
	
	/** \brief Remove all path. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list. */
	dePathList &operator=(const dePathList &list);
	/*@}*/
};

#endif
