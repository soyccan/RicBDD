/****************************************************************************
  FileName     [ p6a.cpp ]
  PackageName  [ ]
  Synopsis     [ Define main() ]
  Author       [ 陳正康 R10922187 ]
  Description  [ Witness-BDD generation algorithm ]
****************************************************************************/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include "bddNode.h"
#include "bddMgr.h"
#include "p7a.h"

inline bool BddNode::isOne() const
{
   return isTerminal() && isPosEdge();
}

inline bool BddNode::isZero() const
{
   return isTerminal() && isNegEdge();
}

BddNode BddNode::restrict(const BddNode& c) const {
   assert(!c.isZero());
   if (c.isOne()) return *this;
   if (isOne() || isZero()) return *this;
   
   int a = c.getLevel();
   // left is positive cofactor
   if (c.getRightCofactor(a).isZero())
      return getLeftCofactor(a).restrict(c.getLeftCofactor(a));
   if (c.getLeftCofactor(a).isZero())
      return getRightCofactor(a).restrict(c.getRightCofactor(a));
   
   if (getLeftCofactor(a) == getRightCofactor(a))
      return restrict(c.getLeftCofactor(a) | c.getRightCofactor(a));

   return c & getLeftCofactor(a).restrict(c.getLeftCofactor(a)) 
      | ~c & getRightCofactor(a).restrict(c.getRightCofactor(a));
}

int main() {
   BddNode::_debugBddAddr = true;
   BddNode::_debugRefCount = true;

   BddMgr bm;
   bm.init(6, 127, 61);

   BddNode a(bm.getSupport(6));
   BddNode b(bm.getSupport(5));
   BddNode c(bm.getSupport(4));
   BddNode d(bm.getSupport(3));
   BddNode e(bm.getSupport(2));
   BddNode f(bm.getSupport(1));

   BddNode g1 = b & d;
   BddNode g2 = e & ~c;
   BddNode g3 = a & b;
   BddNode g4 = c & g1;
   BddNode g5 = g1 | g2;
   BddNode g6 = g2 | d;
   BddNode g7 = g6 & g3;
   BddNode g8 = g4 | g7;
   BddNode g9 = g8 & f;
   
   g4.drawbddPNG("g4", "p7a.g4");
   (~g7).drawbddPNG("~g7", "p7a.~g7");
   g4.restrict(~g7).drawbddPNG("~g7", "p7a.res_g4_~g7");
   g7.restrict(~g4).drawbddPNG("tmp", "p7a.tmp");
   
   assert((~g7).getLevel() == 6);
   assert(g4.getLevel() == 5);
   assert(g4.getLeftCofactor(6) == g4.getRightCofactor(6));
   assert(g4.restrict(~g7) == g4.restrict((~g7).getLeftCofactor(6) | (~g7).getRightCofactor(6)));
   assert(((~g7).getLeftCofactor(6) | (~g7).getRightCofactor(6)).isOne());
   assert(g4.restrict(~g7) == g4.restrict(BddNode::_one));
   assert(g4.restrict(~g7) == g4);
}