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

   BddNode F = (a & ~(b | (c & ~d)));
   BddNode G = a & b & c & d;
   BddNode I = F.itp(G);

   // F.getLeft().drawbddPNG("fa", "fa");
   // F.getRight().drawbddPNG("fb", "fb");
   (I&G).drawbddPNG("IG","IG");
   F.drawbddPNG("F", "F");
   G.drawbddPNG("G", "G");
   I.drawbddPNG("I", "I");

   assert((~F | I) == BddNode::_one);
   assert((I & G) == BddNode::_zero);

}