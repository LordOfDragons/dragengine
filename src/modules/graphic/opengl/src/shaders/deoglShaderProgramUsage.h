/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEOGLSHADERPROGRAMREFERENCE_H_
#define _DEOGLSHADERPROGRAMREFERENCE_H_

class deoglShaderProgram;


/**
 * \brief Shader program usage holder.
 * 
 * Safe way to hold an shader program usage with correct usage handling.
 * Storing \em NULL is allowed. Initial value is \em NULL.
 * 
 * \note This class is designed to be used as stack or member shader program. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class deoglShaderProgramUsage{
private:
	deoglShaderProgram *pProgram;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty shader program usage holder. */
	deoglShaderProgramUsage();
	
	/**
	 * \brief Create shader program usage holder with shader program.
	 * 
	 * deoglShaderProgramUsage is added if shader program is not \em NULL.
	 */
	deoglShaderProgramUsage( deoglShaderProgram *program );
	
	/**
	 * \brief Create shader program usage holder with shader program from another holder.
	 * 
	 * deoglShaderProgramUsage is added if shader program in holder is not \em NULL.
	 */
	deoglShaderProgramUsage( const deoglShaderProgramUsage &usage );
	
	/**
	 * \brief Clean up shader program usage holder.
	 * 
	 * Decrease usage if shader program is not \em NULL.
	 */
	~deoglShaderProgramUsage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set shader program without adding usage.
	 * 
	 * Use this method if the shader program to hold has been added a usage already. This is
	 * the case with created shader programs as well as certain methods returning newly created
	 * shader programs. In all these cases the shader program has to be held without adding a usage.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em shader program to be a \em NULL shader program.
	 */
	void TakeOver( deoglShaderProgram *program );
	
	/** \brief Shader program is \em NULL. */
	bool operator!() const;
	
	/** \brief Test if shader program is not \em NULL. */
	operator bool() const;
	
	/** \brief Pointer to shader program. */
	inline operator deoglShaderProgram*() const{ return pProgram; }
	
	/**
	 * \brief deoglShaderProgramUsage to shader program.
	 * 
	 * \throws deeNullPointer if shader program is \em NULL.
	 */
	operator deoglShaderProgram&() const;
	
	/**
	 * \brief Pointer to shader program.
	 * 
	 * \throws deeNullPointer if shader program is \em NULL.
	 */
	deoglShaderProgram* operator->() const;
	
	/**
	 * \brief Store shader program.
	 * 
	 * If an shader program is already held its usage is decreased and the new shader
	 * program stored. If the new shader program is not \em NULL a usage is added.
	 */
	deoglShaderProgramUsage &operator=( deoglShaderProgram *program );
	
	/**
	 * \brief Store shader program.
	 * 
	 * If an shader program is already held its usage is decreased and the new shader
	 * program stored. If the new shader program is not \em NULL a usage is added.
	 */
	deoglShaderProgramUsage &operator=( const deoglShaderProgramUsage &usage );
	
	/** \brief Test if shader program is held by this holder. */
	inline bool operator==( deoglShaderProgram *program ) const{ return pProgram == program; }
	
	/** \brief Test if shader program is held by this holder. */
	inline bool operator==( const deoglShaderProgramUsage &usage ) const{
		return pProgram == usage.pProgram;
	}
	/*@}*/
};

#endif
