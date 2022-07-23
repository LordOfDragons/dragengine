/* 
 * Drag[en]gine Game Engine
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

#ifndef _DECBASEFILEWRITER_H_
#define _DECBASEFILEWRITER_H_

#include <stdint.h>

#include "../math/decMath.h"
#include "../../deObject.h"


/**
 * \brief File writer interface.
 * 
 * File writers can support seeking but are not required to.
 * If seeking is not supported an exception is thrown.
 */
class DE_DLL_EXPORT decBaseFileWriter : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<decBaseFileWriter> Ref;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file writer. */
	decBaseFileWriter();
	
protected:
	/**
	 * \brief Clean up file writer.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~decBaseFileWriter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name of the file. */
	virtual const char *GetFilename() = 0;
	
	/** \brief Current writing position in the file. */
	virtual int GetPosition() = 0;
	
	/** \brief Set file position for the next write action. */
	virtual void SetPosition( int position ) = 0;
	
	/** \brief Move file position by the given offset. */
	virtual void MovePosition( int offset ) = 0;
	
	/** \brief Set file position to the given position measured from the end of the file. */
	virtual void SetPositionEnd( int position ) = 0;
	
	/**
	 * \brief Write \em size bytes from \em buffer and advances the file pointer.
	 * \throws deeInvalidParam \em buffer is NULL.
	 * \throws deeInvalidParam \em size is less than 0.
	 */
	virtual void Write( const void *buffer, int size ) = 0;
	/*@}*/
	
	
	
	/** \name Helper Functions */
	/*@{*/
	/** \brief Write one byte to file and advances write pointer. */
	void WriteChar( int8_t value );
	
	/** \brief Write one unsigned byte to file and advances write pointer. */
	void WriteByte( uint8_t value );
	
	/** \brief Write one short integer (2 bytes) to file and advances write pointer. */
	void WriteShort( int16_t value );
	
	/** \brief Write one unsigned short integer (2 bytes) to file and advances write pointer. */
	void WriteUShort( uint16_t value );
	
	/** \brief Write one integer (4 bytes) to file and advances write pointer. */
	void WriteInt( int32_t value );
	
	/** \brief Write one unsigned integer (4 bytes) to file and advances write pointer. */
	void WriteUInt( uint32_t value );
	
	/** \brief Write one integer (8 bytes) to file and advances write pointer. */
	void WriteLong( int64_t value );
	
	/** \brief Write one unsigned integer (8 bytes) to file and advances write pointer. */
	void WriteULong( uint64_t value );
	
	/** \brief Write one float (4 bytes) to file and advances write pointer. */
	void WriteFloat( float value );
	
	/** \brief Write one double (8 bytes) to file and advances write pointer. */
	void WriteDouble( double value );
	
	/** \brief Write string-length bytes of string to file without length field and advances write pointer. */
	void WriteString( const char *string );
	
	/** \brief Write string-length bytes of string to file with a 1-byte length field in front and advances write pointer. */
	void WriteString8( const char *string );
	
	/** \brief Write string-length bytes of string to file with a 2-byte length field in front and advances write pointer. */
	void WriteString16( const char *string );
	
	/** \brief Write a 3-float vector to the file ( order x, y, z ) and advances write pointer. */
	void WriteVector( const decVector &vector );
	
	/** \brief Write a 2-float vector to the file ( order x, y, z ) and advances write pointer. */
	void WriteVector2( const decVector2 &vector );
	
	/** \brief Write a 4-float quaternion to the file ( order x, y, z, w ) and advances write pointer. */
	void WriteQuaternion( const decQuaternion &quaternion );
	
	/** \brief Write a 2-int point to the file ( order x, y ) and advances write pointer. */
	void WritePoint( const decPoint &point );
	
	/** \brief Write a 3-int point to the file ( order x, y, z ) and advances write pointer. */
	void WritePoint3( const decPoint3 &point );
	
	/** \brief Write a 3-double vector to the file ( order x, y, z ) and advances write pointer. */
	void WriteDVector( const decDVector &vector );
	
	/** \brief Write a 4-component color to the file ( order r, g, b, a ) and advances write pointer. */
	void WriteColor( const decColor &color );
	
	/** \brief Write a 3-component color to the file ( order r, g, b, a ) and advances write pointer. */
	void WriteColor3( const decColor &color );
	/*@}*/
};

#endif
