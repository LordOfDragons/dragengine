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

#ifndef _DEOGLBASICS_H_
#define _DEOGLBASICS_H_

#include "deoglGL.h"

class deoglRenderThread;


#ifdef WITH_DEBUG
#define OGL_THREAD_CHECK 1
#endif

#ifdef OGL_THREAD_CHECK
	void dbgInitThreadCheck();
	#define OGL_INIT_THREAD_CHECK dbgInitThreadCheck()
	void dbgInitMainThreadCheck();
	#define OGL_INIT_MAIN_THREAD_CHECK dbgInitMainThreadCheck()
	
	void dbgOnMainThreadCheck();
	#define OGL_ON_MAIN_THREAD dbgOnMainThreadCheck();
	
	void dbgOnRenderThreadCheck();
	#define OGL_ON_RENDER_THREAD dbgOnRenderThreadCheck();
	
	void dbgOnMainOrRenderThreadCheck();
	#define OGL_ON_MAIN_OR_RENDER_THREAD dbgOnMainOrRenderThreadCheck();
	
#else
	#define OGL_INIT_THREAD_CHECK
	#define OGL_INIT_MAIN_THREAD_CHECK
	#define OGL_ON_MAIN_THREAD
	#define OGL_ON_RENDER_THREAD
	#define OGL_ON_MAIN_OR_RENDER_THREAD
#endif



void oglClearError();

void dbgCheckOglError( deoglRenderThread &renderThread, const char *file, int line );
void oglWaitFence( deoglRenderThread &renderThread, GLsync &fence, const char *file, int line );

#define OGL_CHECK(renderThread,cmd) oglClearError(); cmd; dbgCheckOglError(renderThread, __FILE__, __LINE__)
#define OGLX_CHECK(renderThread,cmd) if((cmd) == False) (renderThread).GetLogger().LogErrorFormat("failed at %s:%i\n", __FILE__, __LINE__)
#define OGL_IF_CHECK(cmd) cmd
#define OGL_FENCE_WAIT(renderThread,fence) oglWaitFence(renderThread, fence, __FILE__, __LINE__)


struct oglRGBA{
	GLubyte r, g, b, a;
};

struct oglRGB{
	GLubyte r, g, b;
};



struct oglVector2{
	GLfloat x, y;
};

struct oglVector3{
	GLfloat x, y, z;
};

struct oglVector4{
	GLfloat x, y, z, w;
};



struct oglMatrix2x3{
	GLfloat a11, a12, a13;
	GLfloat a21, a22, a23;
};

struct oglMatrix3x4{
	GLfloat a11, a12, a13, a14;
	GLfloat a21, a22, a23, a24;
	GLfloat a31, a32, a33, a34;
};

struct oglMatrix4x4{
	GLfloat a11, a12, a13, a14;
	GLfloat a21, a22, a23, a24;
	GLfloat a31, a32, a33, a34;
	GLfloat a41, a42, a43, a44;
};



// color formats
struct oglHTFacePoint{
	unsigned char x;
	unsigned char z;
};



// VBO stuff
#define VBO_PNTC_POSITION		0
#define VBO_PNTC_REAL_NORMAL	12
#define VBO_PNTC_NORMAL			24
#define VBO_PNTC_TANGENT		36
#define VBO_PNTC_TEXCOORD		48
#define VBO_PNTC_STRIDE			56
struct deoglVBOpntc{
	GLfloat x, y, z; // position
	GLfloat rnx, rny, rnz; // realNormal
	GLfloat nx, ny, nz; // normal
	GLfloat tx, ty, tz; // tangent
	GLfloat u, v; // tex-pos
};

struct deoglVBOpnt{
	GLfloat x, y, z; // position
	GLfloat rnx, rny, rnz; // realNormal
	GLfloat nx, ny, nz; // normal
	GLfloat tx, ty, tz, tw; // tangent
};

#define VBO_PN_POSITION		0
#define VBO_PN_NORMAL		12
#define VBO_PN_STRIDE		24
struct deoglVBOpn{
	GLfloat x, y, z; // position
	GLfloat nx, ny, nz; // normal
};

#define VBO_P_POSITION		0
#define VBO_P_STRIDE		12
struct deoglVBOp{
	GLfloat x, y, z; // position
};



#define VBO_R_PNT_POSITION	0
#define VBO_R_PNT_NORMAL		12
#define VBO_R_PNT_TANGENT		18
#define VBO_R_PNT_STRIDE		24
struct deoglVBORpnt{
	GLfloat x, y, z; // position
	GLshort nx, ny, nz; // normal
	GLshort tx, ty, tz; // tangent
};



#define VBO_RP1_POINT_STRIDE				26
#define VBO_RP1_POINT_OFFSET_POS			0
#define VBO_RP1_POINT_OFFSET_NORMAL			12
#define VBO_RP1_POINT_OFFSET_TANGENT		15
#define VBO_RP1_POINT_OFFSET_TEXPOS			18
struct deoglVBOPointRP1{
	GLfloat x, y, z; // position
	GLbyte nx, ny, nz; // normal
	GLbyte tx, ty, tz; // tangent
	GLfloat u, v; // tex-pos
};

#define VBO_RP2_POINT_STRIDE				20
#define VBO_RP2_POINT_OFFSET_POS			0
#define VBO_RP2_POINT_OFFSET_NORMAL			12
#define VBO_RP2_POINT_OFFSET_TANGENT		15
#define VBO_RP2_POINT_OFFSET_TEXPOS			18
struct deoglVBOPointRP2{
	GLfloat x, y, z; // position
	GLbyte nx, ny, nz; // normal
	GLbyte tx, ty, tz; // tangent
	GLbyte u, v; // tex-pos
};

#define VBO_PHT_POSITION	0
#define VBO_PHT_NORMAL		12
#define VBO_PHT_TANGENT		24
#define VBO_PHT_TEXCOORD	36
#define VBO_PHT_STRIDE		44
struct deoglVBOHeightTerrain{
	GLfloat x, y, z; // position
	GLfloat nx, ny, nz; // normal
	GLfloat tx, ty, tz; // tangent
	GLfloat u, v; // tex-pos
};

#define VBO_PHT1_POSITION	0
#define VBO_PHT1_STRIDE		8
struct deoglVBOHeightTerrain1{
	GLfloat x, z; // position
};

#define VBO_PHT2_HEIGHT		0
#define VBO_PHT2_NORMAL		4
//#define VBO_PHT2_STRIDE		16
#define VBO_PHT2_STRIDE		8
struct deoglVBOHeightTerrain2{
	GLfloat height;
	//GLfloat nx, ny, nz;
	GLfloat normal;
};



/** Render task filters. */
enum eRenderTaskFilters{
	ertfRender = 0x1,
	ertfSolid = 0x2,
	ertfShadowNone = 0x4,
	ertfReflected = 0x8,
	ertfRendered = 0x10,
	ertfOutline = 0x20,
	ertfOutlineSolid = 0x40,
	ertfHoles = 0x80,
	ertfDecal = 0x100,
	ertfDoubleSided = 0x200,
	ertfXRay = 0x400
};



/** Printf problems. */
#ifdef OS_W32
#define OGLPFLLU "I64u"
#else
#define OGLPFLLU "llu"
#endif


#endif
