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

#include <stdlib.h>
#include <string.h>

#include "decBaseFileReader.h"
#include "../exceptions.h"


// Class decBaseFileReader
////////////////////////////

// Constructor, Destructor
////////////////////////////

decBaseFileReader::decBaseFileReader(){
}

decBaseFileReader::~decBaseFileReader(){
}



// Helper Functions
/////////////////////

bool decBaseFileReader::IsEOF(){
	return GetPosition() == GetLength();
}



// Reading
////////////

int8_t decBaseFileReader::ReadChar(){
	int8_t value;
	Read(&value, 1);
	return value;
}

uint8_t decBaseFileReader::ReadByte(){
	uint8_t value;
	Read(&value, 1);
	return value;
}

int16_t decBaseFileReader::ReadShort(){
	const uint16_t value = ReadUShort();
	int16_t realValue;
	memcpy(&realValue, &value, 2);
	return realValue;
}

uint16_t decBaseFileReader::ReadUShort(){
	uint8_t value[2];
	Read(&value, 2);
	return (uint16_t)value[0]
		| ((uint16_t)value[1] << 8);
}

int32_t decBaseFileReader::ReadInt(){
	const uint32_t value = ReadUInt();
	int32_t realValue;
	memcpy(&realValue, &value, 4);
	return realValue;
}

uint32_t decBaseFileReader::ReadUInt(){
	uint8_t value[4];
	Read(&value, 4);
	return (uint32_t)value[0]
		| ((uint32_t)value[1] << 8)
		| ((uint32_t)value[2] << 16)
		| ((uint32_t)value[3] << 24);
}

int64_t decBaseFileReader::ReadLong(){
	const uint64_t value = ReadULong();
	int64_t realValue;
	memcpy(&realValue, &value, 8);
	return realValue;
}

uint64_t decBaseFileReader::ReadULong(){
	uint8_t value[8];
	Read(&value, 8);
	return (uint64_t)value[0]
		| ((uint64_t)value[1] << 8)
		| ((uint64_t)value[2] << 16)
		| ((uint64_t)value[3] << 24)
		| ((uint64_t)value[4] << 32)
		| ((uint64_t)value[5] << 40)
		| ((uint64_t)value[6] << 48)
		| ((uint64_t)value[7] << 56);
}

uint32_t decBaseFileReader::ReadVarUInt(){
	const uint8_t byte1 = ReadByte();
	const int length = byte1 >> 6;
	uint32_t value = byte1 & 0x3f;
	int i;
	for(i=0; i<length; i++){
		value = (value << 8) | ReadByte();
	}
	return value;
}

float decBaseFileReader::ReadFloat(){
	const uint32_t value = ReadUInt();
	float realValue;
	memcpy(&realValue, &value, 4);
	return realValue;
}

double decBaseFileReader::ReadDouble(){
	const uint64_t value = ReadULong();
	double realValue;
	memcpy(&realValue, &value, 8);
	return realValue;
}

decString decBaseFileReader::ReadString8(){
	decString string;
	ReadString8Into(string);
	return string;
}

void decBaseFileReader::ReadString8Into(decString &string){
	const int len = ReadByte();
	string.Set(' ', len);
	Read(string.GetMutableString(), len);
}

decString decBaseFileReader::ReadString16(){
	decString string;
	ReadString16Into(string);
	return string;
}

void decBaseFileReader::ReadString16Into(decString &string){
	const int len = ReadUShort();
	string.Set(' ', len);
	Read(string.GetMutableString(), len);
}

decString decBaseFileReader::ReadString32(){
	decString string;
	ReadString32Into(string);
	return string;
}

void decBaseFileReader::ReadString32Into(decString &string){
	const int len = ReadUInt();
	string.Set(' ', len);
	Read(string.GetMutableString(), len);
}

decString decBaseFileReader::ReadVarString(){
	decString string;
	ReadVarStringInto(string);
	return string;
}

void decBaseFileReader::ReadVarStringInto(decString &string){
	const int len = ReadVarUInt();
	string.Set(' ', len);
	Read(string.GetMutableString(), len);
}

decVector decBaseFileReader::ReadVector(){
	decVector vector;
	ReadVectorInto(vector);
	return vector;
}

void decBaseFileReader::ReadVectorInto(decVector &vector){
	vector.x = ReadFloat();
	vector.y = ReadFloat();
	vector.z = ReadFloat();
}

decVector2 decBaseFileReader::ReadVector2(){
	decVector2 vector;
	ReadVector2Into(vector);
	return vector;
}

void decBaseFileReader::ReadVector2Into(decVector2 &vector){
	vector.x = ReadFloat();
	vector.y = ReadFloat();
}

decQuaternion decBaseFileReader::ReadQuaternion(){
	decQuaternion quaternion;
	ReadQuaternionInto(quaternion);
	return quaternion;
}

void decBaseFileReader::ReadQuaternionInto(decQuaternion &quaternion){
	quaternion.x = ReadFloat();
	quaternion.y = ReadFloat();
	quaternion.z = ReadFloat();
	quaternion.w = ReadFloat();
}

decPoint decBaseFileReader::ReadPoint(){
	decPoint point;
	ReadPointInto(point);
	return point;
}

void decBaseFileReader::ReadPointInto(decPoint &point){
	point.x = ReadInt();
	point.y = ReadInt();
}

decPoint3 decBaseFileReader::ReadPoint3(){
	decPoint3 point;
	ReadPoint3Into(point);
	return point;
}

void decBaseFileReader::ReadPoint3Into(decPoint3 &point){
	point.x = ReadInt();
	point.y = ReadInt();
	point.z = ReadInt();
}

decDVector decBaseFileReader::ReadDVector(){
	decDVector vector;
	ReadDVectorInto(vector);
	return vector;
}

void decBaseFileReader::ReadDVectorInto(decDVector &vector){
	vector.x = ReadDouble();
	vector.y = ReadDouble();
	vector.z = ReadDouble();
}

decColor decBaseFileReader::ReadColor(){
	decColor color;
	ReadColorInto(color);
	return color;
}

void decBaseFileReader::ReadColorInto(decColor &color){
	color.r = ReadFloat();
	color.g = ReadFloat();
	color.b = ReadFloat();
	color.a = ReadFloat();
}

decColor decBaseFileReader::ReadColor3(){
	decColor color;
	ReadColor3Into(color);
	return color;
}

void decBaseFileReader::ReadColor3Into(decColor &color){
	color.r = ReadFloat();
	color.g = ReadFloat();
	color.b = ReadFloat();
	color.a = 1.0f;
}



void decBaseFileReader::SkipChar(){
	MovePosition(1);
}

void decBaseFileReader::SkipByte(){
	MovePosition(1);
}

void decBaseFileReader::SkipShort(){
	MovePosition(2);
}

void decBaseFileReader::SkipUShort(){
	MovePosition(2);
}

void decBaseFileReader::SkipInt(){
	MovePosition(4);
}

void decBaseFileReader::SkipUInt(){
	MovePosition(4);
}

void decBaseFileReader::SkipLong(){
	MovePosition(8);
}

void decBaseFileReader::SkipULong(){
	MovePosition(8);
}

void decBaseFileReader::SkipVarUInt(){
	const int length = ReadByte() >> 6;
	if(length > 0){
		MovePosition(length);
	}
}

void decBaseFileReader::SkipFloat(){
	MovePosition(4);
}

void decBaseFileReader::SkipDouble(){
	MovePosition(8);
}

void decBaseFileReader::SkipString8(){
	const int len = ReadByte();
	MovePosition(len);
}

void decBaseFileReader::SkipString16(){
	const int len = ReadUShort();
	MovePosition(len);
}

void decBaseFileReader::SkipString32(){
	const unsigned int len = ReadUInt();
	
	if(len < 2147483648){
		MovePosition((int)len);
		
	}else{
		MovePosition((int)(len & 0x7fffffff));
		MovePosition((int)(len > 31));
	}
}

void decBaseFileReader::SkipVarString(){
	const int len = (int)ReadVarUInt();
	MovePosition(len);
}

void decBaseFileReader::SkipVector(){
	MovePosition(12);
}

void decBaseFileReader::SkipVector2(){
	MovePosition(8);
}

void decBaseFileReader::SkipQuaternion(){
	MovePosition(16);
}

void decBaseFileReader::SkipPoint(){
	MovePosition(8);
}

void decBaseFileReader::SkipPoint3(){
	MovePosition(12);
}

void decBaseFileReader::SkipDVector(){
	MovePosition(16);
}

void decBaseFileReader::SkipColor(){
	MovePosition(16);
}

void decBaseFileReader::SkipColor3(){
	MovePosition(12);
}
