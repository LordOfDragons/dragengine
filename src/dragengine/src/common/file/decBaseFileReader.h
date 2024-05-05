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

#ifndef _DECBASEFILEREADER_H_
#define _DECBASEFILEREADER_H_

#include <stdint.h>

#include "../math/decMath.h"
#include "../string/decString.h"
#include "../utils/decDateTime.h"
#include "../../deObject.h"

class decDataChunk;


/**
 * \brief File reader interface.
 */
class DE_DLL_EXPORT decBaseFileReader : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<decBaseFileReader> Ref;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file reader. */
	decBaseFileReader();
	
protected:
	/**
	 * \brief Clean up file reader.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~decBaseFileReader();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name of the file. */
	virtual const char *GetFilename() = 0;
	
	/** \brief Length of the file. */
	virtual int GetLength() = 0;
	
	/** \brief Modification time. */
	virtual TIME_SYSTEM GetModificationTime() = 0;
	
	/** \brief Current reading position in the file. */
	virtual int GetPosition() = 0;
	
	/** \brief Set file position for the next read action. */
	virtual void SetPosition( int position ) = 0;
	
	/** \brief Move file position by the given offset. */
	virtual void MovePosition( int offset ) = 0;
	
	/** \brief Set file position to the given position measured from the end of the file. */
	virtual void SetPositionEnd( int position ) = 0;
	
	/**
	 * \brief Read \em size bytes into \em buffer and advances the file pointer.
	 * \throws deeInvalidParam \em buffer is NULL.
	 * \throws deeInvalidParam \em size is less than 1.
	 */
	virtual void Read( void *buffer, int size ) = 0;
	
	/** \brief Duplicate file reader. */
	virtual Ref Duplicate() = 0;
	/*@}*/
	
	
	
	/** \name Helper Functions */
	/*@{*/
	/** \brief File pointer is at the end of the file. */
	bool IsEOF();
	
	
	
	/** \brief Read one byte and advances the file pointer. */
	int8_t ReadChar();
	
	/** \brief Read one unsigned byte and advances the file pointer. */
	uint8_t ReadByte();
	
	/** \brief Read one short integer (2 bytes) and advances the file pointer. */
	int16_t ReadShort();
	
	/** \brief Read one unsigned short integer (2 bytes) and advances the file pointer. */
	uint16_t ReadUShort();
	
	/** \brief Read one integer (4 bytes) and advances the file pointer. */
	int32_t ReadInt();
	
	/** \brief Read one unsigned integer (4 bytes) and advances the file pointer. */
	uint32_t ReadUInt();
	
	/** \brief Read one integer (8 bytes) and advances the file pointer. */
	int64_t ReadLong();
	
	/** \brief Read one unsigned integer (8 bytes) and advances the file pointer. */
	uint64_t ReadULong();
	
	/**
	 * \brief Read variable length unsigned integer (1-4 bytes) and advances file pointer.
	 * 
	 * Variable length integers are written using 1 to 4 bytes. The highest 2 bits of the
	 * first byte stores the total length (0=1 byte, 1=2 bytes, 2=3 bytes, 3=4 bytes).
	 * The lower 6 bits are used as value. With each added byte the previous bits are
	 * shifted up. The maximum storable value is thus 1073741823.
	 * 
	 * Variable length unsigned integers are typically used for values like versions
	 * or revisions which start low and potentially grow large over time.
	 */
	uint32_t ReadVarUInt();
	
	/** \brief Read one float (4 bytes) and advances the file pointer. */
	float ReadFloat();
	
	/** \brief Read one double (8 bytes) and advances the file pointer. */
	double ReadDouble();
	
	
	
	/**
	 * \brief Read a string prefixed by a 1-byte length field and advances the file pointer.
	 * 
	 * The returned string pointer has to be freed by the caller itself.
	 */
	decString ReadString8();
	
	/**
	 * \brief Read string prefixed by a 1-byte length field and advances the file pointer.
	 */
	void ReadString8Into( decString &string );
	
	/**
	 * \brief Read a string prefixed by a 2-bytes length field and advances the file pointer.
	 * 
	 * The returned string pointer has to be freed by the caller itself.
	 */
	decString ReadString16();
	
	/**
	 * \brief Read string prefixed by a 2-bytes length field and advances the file pointer.
	 */
	void ReadString16Into( decString &string );
	
	/**
	 * \brief Read a string prefixed by a 4-bytes length field and advances the file pointer.
	 * 
	 * The returned string pointer has to be freed by the caller itself.
	 */
	decString ReadString32();
	
	/**
	 * \brief Read string prefixed by a 4-bytes length field and advances the file pointer.
	 */
	void ReadString32Into( decString &string );
	
	/**
	 * \brief Read a variable string prefixed by a 1-4 bytes length field and advances the file pointer.
	 * 
	 * The length is stored as variable unsigned integer (ReadVarUInt).
	 * The returned string pointer has to be freed by the caller itself.
	 */
	decString ReadVarString();
	
	/**
	 * \brief Read variable string prefixed by a 1-4 bytes length field and advances the file pointer.
	 * 
	 * The length is stored as variable unsigned integer (ReadVarUInt).
	 */
	void ReadVarStringInto( decString &string );
	
	/**
	 * \brief Read a 3-float vector and advances the file pointer.
	 * 
	 * The vector components are read in the order x, y and z.
	 */
	decVector ReadVector();
	
	/**
	 * \brief Read a 3-float vector and advances the file pointer.
	 * 
	 * The vector components are read in the order x, y and z.
	 */
	void ReadVectorInto( decVector &vector );
	
	/**
	 * \brief Read a 2-float vector and advances the file pointer.
	 * 
	 * The vector components are read in the order x and y.
	 */
	decVector2 ReadVector2();
	
	/**
	 * \brief Read a 2-float vector and advances the file pointer.
	 * 
	 * The vector components are read in the order x and y.
	 */
	void ReadVector2Into( decVector2 &vector );
	
	/**
	 * \brief Read a 4-float quaternion and advances the file pointer.
	 * 
	 * The quaternion components are read in the order x, y, z and w.
	 */
	decQuaternion ReadQuaternion();
	
	/**
	 * \brief Read a 4-float quaternion and advances the file pointer.
	 * 
	 * The quaternion components are read in the order x, y, z and w.
	 */
	void ReadQuaternionInto( decQuaternion &quaternion );
	
	/**
	 * \brief Read a 2-int point and advances the file pointer.
	 * 
	 * The point components are read in the order x and y.
	 */
	decPoint ReadPoint();
	
	/**
	 * \brief Read a 2-int point and advances the file pointer.
	 * 
	 * The point components are read in the order x and y.
	 */
	void ReadPointInto( decPoint &point );
	
	/**
	 * \brief Read a 3-int point and advances the file pointer.
	 * 
	 * The point components are read in the order x, y and z.
	 */
	decPoint3 ReadPoint3();
	
	/**
	 * \brief Read a 3-int point and advances the file pointer.
	 * 
	 * The point components are read in the order x, y and z.
	 */
	void ReadPoint3Into( decPoint3 &point );
	
	/**
	 * \brief Read a 3-double vector and advances the file pointer.
	 * 
	 * The vector components are read in the order x, y and z.
	 */
	decDVector ReadDVector();
	
	/**
	 * \brief Read a 3-double vector and advances the file pointer.
	 * 
	 * The vector components are read in the order x, y and z.
	 */
	void ReadDVectorInto( decDVector &vector );
	
	/**
	 * \brief Read a 4-component color and advances the file pointer.
	 * 
	 * The color components are read in the order r, g, b and a.
	 */
	decColor ReadColor();
	
	/**
	 * \brief Read a 4-component color and advances the file pointer.
	 * 
	 * The color components are read in the order r, g, b and a.
	 */
	void ReadColorInto( decColor &color );
	
	/**
	 * \brief Read a 3-component color and advances the file pointer.
	 * 
	 * The color components are read in the order r, g and b.
	 */
	decColor ReadColor3();
	
	/**
	 * \brief Read a 3-component color and advances the file pointer.
	 * 
	 * The color components are read in the order r, g and b.
	 */
	void ReadColor3Into( decColor &color );
	
	
	
	/** \brief Skip one byte and advances the file pointer. */
	void SkipChar();
	
	/** \brief Skip one unsigned byte and advances the file pointer. */
	void SkipByte();
	
	/** \brief Skip one short integer (2 bytes) and advances the file pointer. */
	void SkipShort();
	
	/** \brief Skip one unsigned short integer (2 bytes) and advances the file pointer. */
	void SkipUShort();
	
	/** \brief Skip one integer (4 bytes) and advances the file pointer. */
	void SkipInt();
	
	/** \brief Skip one unsigned integer (4 bytes) and advances the file pointer. */
	void SkipUInt();
	
	/** \brief Skip one integer (8 bytes) and advances the file pointer. */
	void SkipLong();
	
	/** \brief Skip one unsigned integer (8 bytes) and advances the file pointer. */
	void SkipULong();
	
	/** \brief Skip variable length unsigned integer (1-4 bytes) and advances file pointer. */
	void SkipVarUInt();
	
	/** \brief Skip one float (4 bytes) and advances the file pointer. */
	void SkipFloat();
	
	/** \brief Skip one double (8 bytes) and advances the file pointer. */
	void SkipDouble();
	
	/** \brief Skip a string prefixed by a 1-byte length field and advances the file pointer. */
	void SkipString8();
	
	/** \brief Skip a string prefixed by a 2-byte length field and advances the file pointer. */
	void SkipString16();
	
	/** \brief Skip a string prefixed by a 4-byte length field and advances the file pointer. */
	void SkipString32();
	
	/** \brief Skip a string prefixed by a 1-4 byte length field and advances the file pointer. */
	void SkipVarString();
	
	/** \brief Skip a 3-float vector and advances the file pointer. */
	void SkipVector();
	
	/** \brief Skip a 2-float vector and advances the file pointer. */
	void SkipVector2();
	
	/** \brief Skip a 4-float quaternion and advances the file pointer. */
	void SkipQuaternion();
	
	/** \brief Skip a 2-int point and advances the file pointer. */
	void SkipPoint();
	
	/** \brief Skip a 3-int point and advances the file pointer. */
	void SkipPoint3();
	
	/** \brief Skip a 3-double vector and advances the file pointer. */
	void SkipDVector();
	
	/** \brief Skip a 4-component color and advances the file pointer. */
	void SkipColor();
	
	/** \brief Skip a 3-component color and advances the file pointer. */
	void SkipColor3();
	/*@}*/
};

#endif
