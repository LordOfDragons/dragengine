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

#ifndef _DECBASEFILEREADER_H_
#define _DECBASEFILEREADER_H_

#include <stdint.h>

#include "../../deObject.h"
#include "../math/decMath.h"
#include "../string/decString.h"
#include "../utils/decDateTime.h"

class decDataChunk;


/**
 * \brief File reader interface.
 */
class decBaseFileReader : public deObject{
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
	~decBaseFileReader() override;
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
	virtual void SetPosition(int position) = 0;
	
	/** \brief Move file position by the given offset. */
	virtual void MovePosition(int offset) = 0;
	
	/** \brief Set file position to the given position measured from the end of the file. */
	virtual void SetPositionEnd(int position) = 0;
	
	/**
	 * \brief Read \em size bytes into \em buffer and advances the file pointer.
	 * \throws deeInvalidParam \em buffer is NULL.
	 * \throws deeInvalidParam \em size is less than 1.
	 */
	virtual void Read(void *buffer, int size) = 0;
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
	void ReadString8Into(decString &string);
	
	/**
	 * \brief Read a string prefixed by a 2-byte2 length field and advances the file pointer.
	 * 
	 * The returned string pointer has to be freed by the caller itself.
	 */
	decString ReadString16();
	
	/**
	 * \brief Read string prefixed by a 2-byte2 length field and advances the file pointer.
	 */
	void ReadString16Into(decString &string);
	
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
	void ReadVectorInto(decVector &vector);
	
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
	void ReadVector2Into(decVector2 &vector);
	
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
	void ReadQuaternionInto(decQuaternion &quaternion);
	
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
	void ReadPointInto(decPoint &point);
	
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
	void ReadPoint3Into(decPoint3 &point);
	
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
	void ReadDVectorInto(decDVector &vector);
	
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
	void ReadColorInto(decColor &color);
	
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
	void ReadColor3Into(decColor &color);
	
	
	
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
	
	/** \brief Skip one float (4 bytes) and advances the file pointer. */
	void SkipFloat();
	
	/** \brief Skip one double (8 bytes) and advances the file pointer. */
	void SkipDouble();
	
	/** \brief Skip a string prefixed by a 1-byte length field and advances the file pointer. */
	void SkipString8();
	
	/** \brief Skip a string prefixed by a 2-byte2 length field and advances the file pointer. */
	void SkipString16();
	
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
