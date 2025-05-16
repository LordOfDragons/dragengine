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

#ifndef _DEOGLBASICS_H_
#define _DEOGLBASICS_H_

#include "deoglGL.h"

class deoglRenderThread;


#ifdef WITH_DEBUG
#define OGL_THREAD_CHECK 1
#endif

#ifdef OGL_THREAD_CHECK
	void ogldbgInitThreadCheck();
	#define OGL_INIT_THREAD_CHECK ogldbgInitThreadCheck()
	
	void dbgInitLoaderThreadCheck();
	#define OGL_INIT_LOADER_THREAD_CHECK dbgInitLoaderThreadCheck();
	
	void dbgExitLoaderThreadCheck();
	#define OGL_EXIT_LOADER_THREAD_CHECK dbgExitLoaderThreadCheck();
	
	void ogldbgInitMainThreadCheck();
	#define OGL_INIT_MAIN_THREAD_CHECK ogldbgInitMainThreadCheck()
	
	void ogldbgOnMainThreadCheck();
	#define OGL_ON_MAIN_THREAD ogldbgOnMainThreadCheck();
	
	void dbgOnRenderThreadCheck();
	#define OGL_ON_RENDER_THREAD dbgOnRenderThreadCheck();
	
	void dbgOnMainOrRenderThreadCheck();
	#define OGL_ON_MAIN_OR_RENDER_THREAD dbgOnMainOrRenderThreadCheck();
	
#else
	#define OGL_INIT_THREAD_CHECK
	#define OGL_INIT_LOADER_THREAD_CHECK
	#define OGL_EXIT_LOADER_THREAD_CHECK
	#define OGL_INIT_MAIN_THREAD_CHECK
	#define OGL_ON_MAIN_THREAD
	#define OGL_ON_RENDER_THREAD
	#define OGL_ON_MAIN_OR_RENDER_THREAD
#endif



void oglClearError();

void dbgCheckOglError(deoglRenderThread &renderThread, const char *file, int line, bool withRenderThreadCheck = true);

#define OGL_CHECK(renderThread,cmd)\
	oglClearError();\
	cmd;\
	dbgCheckOglError(renderThread, __FILE__, __LINE__)

#define OGLX_CHECK(renderThread,cmd)\
	if((cmd) == False)\
		(renderThread).GetLogger().LogErrorFormat("failed at %s:%i\n", __FILE__, __LINE__)

#define OGL_IF_CHECK(cmd) cmd

#define OGL_CHECK_WRTC(renderThread,wrtc,cmd) oglClearError(); cmd; dbgCheckOglError(renderThread, __FILE__, __LINE__, wrtc)


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
	ertfXRay = 0x400,
	ertfOcclusion = 0x800,
	ertfShadow = 0x1000,
	ertfCompactShadow = 0x2000
};



/** Printf problems. */
#ifdef OS_W32
#define OGLPFLLU "I64u"
#else
#define OGLPFLLU "llu"
#endif


#endif
