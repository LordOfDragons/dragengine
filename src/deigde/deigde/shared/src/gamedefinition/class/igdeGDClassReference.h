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

#ifndef _IGDEGDCLASSREFERENCE_H_
#define _IGDEGDCLASSREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeGDClass;


/**
 * \brief GDClass reference.
 * 
 * Safe way to hold an game definition class reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member game definition class. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeGDClassReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty game definition class reference holder. */
	igdeGDClassReference();
	
	/**
	 * \brief Create game definition class reference holder with game definition class.
	 * 
	 * Reference is added if game definition class is not NULL.
	 */
	igdeGDClassReference( igdeGDClass *gdclass );
	
	/**
	 * \brief Create game definition class reference holder with game definition class from another holder.
	 * 
	 * Reference is added if game definition class in holder is not NULL.
	 */
	igdeGDClassReference( const igdeGDClassReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set game definition class without adding reference.
	 * 
	 * Use this method if the game definition class to hold has been added a reference already. This is
	 * the case with created game definition classs as well as certain methods returning newly created
	 * game definition classs. In all these cases the game definition class has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em game definition class to be a NULL game definition class.
	 */
	void TakeOver( igdeGDClass *gdclass );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if game definition class is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to game definition class. */
	operator igdeGDClass*() const;
	
	/**
	 * \brief Reference to game definition class.
	 * 
	 * \throws deeNullPointer if game definition class is NULL.
	 */
	operator igdeGDClass&() const;
	
	/**
	 * \brief Pointer to game definition class.
	 * 
	 * \throws deeNullPointer if game definition class is NULL.
	 */
	igdeGDClass* operator->() const;
	
	/**
	 * \brief Store game definition class.
	 * 
	 * If an game definition class is already held its reference is release and the new game definition class
	 * stored. If the new game definition class is not NULL a reference is added.
	 */
	igdeGDClassReference &operator=( igdeGDClass *gdclass );
	
	/**
	 * \brief Store game definition class.
	 * 
	 * If an game definition class is already held its reference is release and the new game definition class
	 * stored. If the new game definition class is not NULL a reference is added.
	 */
	igdeGDClassReference &operator=( const igdeGDClassReference &reference );
	
	/** \brief Test if game definition class is held by this holder. */
	bool operator==( igdeGDClass *gdclass ) const;
	bool operator!=( igdeGDClass *gdclass ) const;
	
	/** \brief Test if game definition class is held by this holder. */
	bool operator==( const igdeGDClassReference &reference ) const;
	bool operator!=( const igdeGDClassReference &reference ) const;
	/*@}*/
};

#endif

