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

#ifndef _DEVKFORMAT_H_
#define _DEVKFORMAT_H_

#include <dragengine/common/string/decString.h>

#include "devkBasics.h"

class decUnicodeString;


/**
 * Vulkan format information.
 */
class devkFormat{
private:
	VkFormat pFormat{};
	int pBitsPerPixel{};
	bool pIsDepth{};
	bool pIsDepthFloat{};
	bool pIsStencil{};
	bool pIsCompressed{};
	bool pCanSample{};
	bool pCanSampleFilter{};
	bool pCanImageStore{};
	bool pCanImageStoreAtomic{};
	bool pCanAttach{};
	bool pCanAttachBlend{};
	decString pName;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create format information. */
	devkFormat();
	
	/** Create format information. */
	devkFormat(VkFormat format, int bitsPerPixel, bool isDepth, bool isDepthFloat,
		bool isStencil, bool isCompressed, const char *name);
	
	/** Create copy of format information. */
	devkFormat(const devkFormat &format);
	
	/** Clean up format information. */
	~devkFormat();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Format. */
	inline VkFormat GetFormat() const{return pFormat;}
	
	/** Set format. */
	void SetFormat(VkFormat format);
	
	/** Bits per pixel. */
	inline int GetBitsPerPixel() const{return pBitsPerPixel;}
	
	/** Set bits per pixel. */
	void SetBitsPerPixel(int bitsPerPixel);
	
	/** Format is a depth/stencil format. */
	inline bool GetIsDepth() const{return pIsDepth;}
	
	/** \brief Set if format is a depth/stencil format */
	void SetIsDepth(bool isDepth);
	
	/** Format is a float depth/stencil format. */
	inline bool GetIsDepthFloat() const{return pIsDepthFloat;}
	
	/** Set if format is a float depth/stencil format. */
	void SetIsDepthFloat(bool isDepthFloat);
	
	/** Format is a stencil format. */
	inline bool GetIsStencil() const{return pIsStencil;}
	
	/** Set if format is a stencil format. */
	void SetIsStencil(bool isStencil);
	
	/** Format is a compressed format. */
	inline bool GetIsCompressed() const{return pIsCompressed;}
	
	/** Format is a compressed format. */
	void SetIsCompressed(bool isCompressed);
	
	/** Format can be used to sample from. */
	inline bool GetCanSample() const{return pCanSample;}
	
	/** Set if format can be used to sample from. */
	void SetCanSample(bool canSample);
	
	/** Format can be used to sample from using filtering. */
	inline bool GetCanSampleFilter() const{return pCanSampleFilter;}
	
	/** Set if format can be used to sample from using filtering. */
	void SetCanSampleFilter(bool canSampleFilter);
	
	/** Format can be used for image store. */
	inline bool GetCanImageStore() const{return pCanImageStore;}
	
	/** Set if format can be used for image store. */
	void SetCanImageStore(bool canImageStore);
	
	/** Format can be used for atomic image store. */
	inline bool GetCanImageStoreAtomic() const{return pCanImageStoreAtomic;}
	
	/** Set if format can be used for atomic image store. */
	void SetCanImageStoreAtomic(bool canImageStoreAtomic);
	
	/** Format can be used as framebuffer attachment. */
	inline bool GetCanAttach() const{return pCanAttach;}
	
	/** Set if format can be used as framebuffer attachment. */
	void SetCanAttach(bool canAttach);
	
	/** Format can be used as blending framebuffer attachment. */
	inline bool GetCanAttachBlend() const{return pCanAttachBlend;}
	
	/** Set if format can be used as blending framebuffer attachment. */
	void SetCanAttachBlend(bool canAttachBlend);
	
	/** Name. */
	inline const decString &GetName() const{return pName;}
	
	/** Set name. */
	void SetName(const char *name);
	
	/** Print format to buffer. */
	void WriteToString(decUnicodeString &buffer) const;
	
	/** Assign. */
	devkFormat &operator=(const devkFormat &format);
	/*@}*/
};

#endif
