/* Part 2 of infblock.c (skipped one function using jump tables) */

#include "zutil.h"
#include "infblock.h"
#include "inftrees.h"
#include "infcodes.h"
#include "infutil.h"

/* HACK: duplicated here for inlining */
static inline void inflate_blocks_reset(s, z, c)
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

int inflate_blocks_free(s, z, c)
inflate_blocks_statef *s;
z_streamp z;
uLongf *c;
{
  inflate_blocks_reset(s, z, c);
  ZFREE(z, s->window);
  ZFREE(z, s);
  Trace((stderr, "inflate:   blocks freed\n"));
  return Z_OK;
}


void inflate_set_dictionary(s, d, n)
inflate_blocks_statef *s;
const Bytef *d;
uInt  n;
{
  zmemcpy((charf *)s->window, d, n);
  s->read = s->write = s->window + n;
}
