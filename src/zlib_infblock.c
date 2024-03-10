/* infblock.c -- interpret and process block types to last block
 * Copyright (C) 1995-1996 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h 
 */

#include "zutil.h"
#include "infblock.h"
#include "inftrees.h"
#include "infcodes.h"
#include "infutil.h"

struct inflate_codes_state {int dummy;}; /* for buggy compilers */

#ifdef DATA_IN_C
/* Table for deflate from PKZIP's appnote.txt. */
local uInt border[] = { /* Order of the bit length code lengths */
        16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
#else
extern uInt z_border[19];
#define border z_border
#endif

/*
   Notes beyond the 1.93a appnote.txt:

   1. Distance pointers never point before the beginning of the output
      stream.
   2. Distance pointers can point back across blocks, up to 32k away.
   3. There is an implied maximum of 7 bits for the bit length table and
      15 bits for the actual data.
   4. If only one code exists, then it is encoded using one bit.  (Zero
      would be more efficient, but perhaps a little confusing.)  If two
      codes exist, they are coded using one bit each (0 and 1).
   5. There is no way of sending zero distance codes--a dummy must be
      sent if there are none.  (History: a pre 2.0 version of PKZIP would
      store blocks with no distance codes, but this was discovered to be
      too harsh a criterion.)  Valid only for 1.93a.  2.04c does allow
      zero distance codes, which is sent as one code of zero bits in
      length.
   6. There are up to 286 literal/length codes.  Code 256 represents the
      end-of-block.  Note however that the static length tree defines
      288 codes just to fill out the Huffman codes.  Codes 286 and 287
      cannot be used though, since there is no length base or extra bits
      defined for them.  Similarily, there are up to 30 distance codes.
      However, static trees define 32 codes (all 5 bits) to fill out the
      Huffman codes, but the last two had better not show up in the data.
   7. Unzip can check dynamic Huffman blocks for complete code sets.
      The exception is that a single code would not be complete (see #4).
   8. The five bits following the block type is really the number of
      literal codes sent minus 257.
   9. Length codes 8,16,16 are interpreted as 13 length codes of 8 bits
      (1+6+6).  Therefore, to output three times the length, you output
      three codes (1+1+1), whereas to output four times the same length,
      you only need two codes (1+3).  Hmm.
  10. In the tree reconstruction algorithm, Code = Code + Increment
      only if BitLength(i) is not zero.  (Pretty obvious.)
  11. Correction: 4 Bits: # of Bit Length codes - 4     (4 - 19)
  12. Note: length code 284 can represent 227-258, but length code 285
      really is 258.  The last length deserves its own, short code
      since it gets used a lot in very redundant files.  The length
      258 is special since 258 - 3 (the min match length) is 255.
  13. The literal/length and distance code bit lengths are read as a
      single stream of lengths.  It is possible (and advantageous) for
      a repeat code (16, 17, or 18) to go across the boundary between
      the two sets of lengths.
 */


void inflate_blocks_reset(s, z, c)
inflate_blocks_statef *s;
z_streamp z;
uLongf *c;
{
  if (s->checkfn != Z_NULL)
    *c = s->check;
  if (s->mode == BTREE || s->mode == DTREE)
    ZFREE(z, s->sub.trees.blens);
  if (s->mode == CODES)
  {
    inflate_codes_free(s->sub.decode.codes, z);
    inflate_trees_free(s->sub.decode.td, z);
    inflate_trees_free(s->sub.decode.tl, z);
  }
  s->mode = TYPE;
  s->bitk = 0;
  s->bitb = 0;
  s->read = s->write = s->window;
  if (s->checkfn != Z_NULL)
    z->adler = s->check = (*s->checkfn)(0L, Z_NULL, 0);
  Trace((stderr, "inflate:   blocks reset\n"));
}


inflate_blocks_statef *inflate_blocks_new(z, c, w)
z_streamp z;
check_func c;
uInt w;
{
  inflate_blocks_statef *s;

  if ((s = (inflate_blocks_statef *)ZALLOC
       (z,1,sizeof(struct inflate_blocks_state))) == Z_NULL)
    return s;
  if ((s->window = (Bytef *)ZALLOC(z, 1, w)) == Z_NULL)
  {
    ZFREE(z, s);
    return Z_NULL;
  }
  s->end = s->window + w;
  s->checkfn = c;
  s->mode = TYPE;
  Trace((stderr, "inflate:   blocks allocated\n"));
  inflate_blocks_reset(s, z, &s->check);
  return s;
}

