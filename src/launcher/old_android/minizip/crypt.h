/* 
 * Drag[en]gine Android Launcher
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

#define CRC32(c, b) ((*(pcrc_32_tab+(((int)(c) ^ (b)) & 0xff))) ^ ((c) >> 8))

/***********************************************************************
 * Return the next byte in the pseudo-random sequence
 */
static int decrypt_byte(unsigned long* pkeys, const unsigned long* pcrc_32_tab)
{
    unsigned temp;  /* POTENTIAL BUG:  temp*(temp^1) may overflow in an
                     * unpredictable manner on 16-bit systems; not a problem
                     * with any known compiler so far, though */

    temp = ((unsigned)(*(pkeys+2)) & 0xffff) | 2;
    return (int)(((temp * (temp ^ 1)) >> 8) & 0xff);
}

/***********************************************************************
 * Update the encryption keys with the next byte of plain text
 */
static int update_keys(unsigned long* pkeys,const unsigned long* pcrc_32_tab,int c)
{
    (*(pkeys+0)) = CRC32((*(pkeys+0)), c);
    (*(pkeys+1)) += (*(pkeys+0)) & 0xff;
    (*(pkeys+1)) = (*(pkeys+1)) * 134775813L + 1;
    {
      register int keyshift = (int)((*(pkeys+1)) >> 24);
      (*(pkeys+2)) = CRC32((*(pkeys+2)), keyshift);
    }
    return c;
}


/***********************************************************************
 * Initialize the encryption keys and the random header according to
 * the given password.
 */
static void init_keys(const char* passwd,unsigned long* pkeys,const unsigned long* pcrc_32_tab)
{
    *(pkeys+0) = 305419896L;
    *(pkeys+1) = 591751049L;
    *(pkeys+2) = 878082192L;
    while (*passwd != '\0') {
        update_keys(pkeys,pcrc_32_tab,(int)*passwd);
        passwd++;
    }
}

#define zdecode(pkeys,pcrc_32_tab,c) \
    (update_keys(pkeys,pcrc_32_tab,c ^= decrypt_byte(pkeys,pcrc_32_tab)))

#define zencode(pkeys,pcrc_32_tab,c,t) \
    (t=decrypt_byte(pkeys,pcrc_32_tab), update_keys(pkeys,pcrc_32_tab,c), t^(c))

#ifdef INCLUDECRYPTINGCODE_IFCRYPTALLOWED

#define RAND_HEAD_LEN  12
   /* "last resort" source for second part of crypt seed pattern */
#  ifndef ZCR_SEED2
#    define ZCR_SEED2 3141592654UL     /* use PI as default pattern */
#  endif

static int crypthead(const char* passwd,      /* password string */
                     unsigned char* buf,      /* where to write header */
                     int bufSize,
                     unsigned long* pkeys,
                     const unsigned long* pcrc_32_tab,
                     unsigned long crcForCrypting)
{
    int n;                       /* index in random header */
    int t;                       /* temporary */
    int c;                       /* random byte */
    unsigned char header[RAND_HEAD_LEN-2]; /* random header */
    static unsigned calls = 0;   /* ensure different random header each time */

    if (bufSize<RAND_HEAD_LEN)
      return 0;

    /* First generate RAND_HEAD_LEN-2 random bytes. We encrypt the
     * output of rand() to get less predictability, since rand() is
     * often poorly implemented.
     */
    if (++calls == 1)
    {
        srand((unsigned)(time(NULL) ^ ZCR_SEED2));
    }
    init_keys(passwd, pkeys, pcrc_32_tab);
    for (n = 0; n < RAND_HEAD_LEN-2; n++)
    {
        c = (rand() >> 7) & 0xff;
        header[n] = (unsigned char)zencode(pkeys, pcrc_32_tab, c, t);
    }
    /* Encrypt random header (last two bytes is high word of crc) */
    init_keys(passwd, pkeys, pcrc_32_tab);
    for (n = 0; n < RAND_HEAD_LEN-2; n++)
    {
        buf[n] = (unsigned char)zencode(pkeys, pcrc_32_tab, header[n], t);
    }
    buf[n++] = (unsigned char)zencode(pkeys, pcrc_32_tab, (int)(crcForCrypting >> 16) & 0xff, t);
    buf[n++] = (unsigned char)zencode(pkeys, pcrc_32_tab, (int)(crcForCrypting >> 24) & 0xff, t);
    return n;
}

#endif
