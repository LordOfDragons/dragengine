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

#ifndef _IGDEUNDO_H_
#define _IGDEUNDO_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Undo action.
 * 
 * Supports Undo/Redo operations.
 */
class DE_DLL_EXPORT igdeUndo : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeUndo> Ref;
	
	
private:
	decString pShortInfo;
	decString pLongInfo;
	
	int pMemoryConsumption;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	igdeUndo();
	
	
	
protected:
	/**
	 * \brief Clean up unfo.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	~igdeUndo() override = default;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Short info. */
	inline const decString &GetShortInfo() const{ return pShortInfo; }
	
	/** \brief Set short info. */
	void SetShortInfo(const char *info);
	
	/** \brief Long info. */
	inline const decString &GetLongInfo() const{ return pLongInfo; }
	
	/** \brief Set long info. */
	void SetLongInfo(const char *info);
	
	/** \brief Memory consumption. */
	inline int GetMemoryConsumption() const{ return pMemoryConsumption; }
	
	/** \brief Set memory consumption. */
	void SetMemoryConsumption(int bytes);
	
	
	
	/** \brief Undo. */
	virtual void Undo() = 0;
	
	/** \brief Redo. */
	virtual void Redo() = 0;
	/*@}*/
};

#endif
