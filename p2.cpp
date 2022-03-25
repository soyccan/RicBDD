/****************************************************************************
  FileName     [ testBdd.cpp ]
  PackageName  [ ]
  Synopsis     [ Define main() ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cassert>
#include "bddNode.h"
#include "bddMgr.h"

using namespace std;

/**************************************************************************/
/*                        Define Global BDD Manager                       */
/**************************************************************************/
BddMgr bm;

/**************************************************************************/
/*                    Define Static Function Prototypes                   */
/**************************************************************************/
static void initBdd(size_t nSupports, size_t hashSize, size_t cacheSize);


/**************************************************************************/
/*                             Define main()                              */
/**************************************************************************/
int
main()
{
   initBdd(5, 127, 61);

   /*-------- THIS IS JUST A TEST CODE ---------*/
   BddNode a(bm.getSupport(5));
   BddNode b(bm.getSupport(4));
   BddNode c(bm.getSupport(3));
   BddNode d(bm.getSupport(2));
   BddNode e(bm.getSupport(1));

   BddNode f = ~bm.ite(a, ~(b^c)|b&~e|~b&~d, ~(b&(c&d&e|~d&~e)|~b&~(c^d)));
   BddNode g = 
         ~a & ~b & ~c & ~d
         | ~a & ~b & c & d
         | ~a & b & ~d & ~e
         | ~a & b & c & d & e
         | a & ~b & c & d
         | a & b & ~c & e;
   assert(f == g);
   f.toMinterms();
   cout << endl;

   for (auto& cube : f.getAllCubes()) {   
      cube.toMinterms();
      cout << endl;
   }
   string label = "p2";
   ofstream ofile(label + ".dot");
   f.drawBdd("f", ofile);
   system(("dot -o "+label+".png -Tpng "+label+".dot").c_str());

   /*----------- END OF TEST CODE ------------*/
}


/**************************************************************************/
/*                          Define Static Functions                       */
/**************************************************************************/
static void
initBdd(size_t nin, size_t h, size_t c)
{
   BddNode::_debugBddAddr = true;
   BddNode::_debugRefCount = true;

//   bm.reset();
   bm.init(nin, h, c);
}

