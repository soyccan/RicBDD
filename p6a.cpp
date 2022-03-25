/****************************************************************************
  FileName     [ p6.cpp ]
  PackageName  [ ]
  Synopsis     [ Define main() ]
  Author       [ 陳正康 R10922187 ]
  Copyright    [ Copyleft(c) 2005-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include "bddNode.h"
#include "bddMgr.h"

#define print_node(node) std::cout << #node << std::endl << node << std::endl

using namespace std;

/**************************************************************************/
/*                        Define Global BDD Manager                       */
/**************************************************************************/
BddMgr bm;

/**************************************************************************/
/*                    Define Static Function Prototypes                   */
/**************************************************************************/
static void initBdd(size_t nSupports, size_t hashSize, size_t cacheSize);

static void 
drawbddPNG(const BddNode& node, const string& label, const string& filename)
{
   ofstream ofile(filename + ".dot");
   node.drawBdd(label, ofile);
   system(("dot -o " + filename + ".png -Tpng " + filename + ".dot").c_str());
}


/**************************************************************************/
/*                             Define main()                              */
/**************************************************************************/
int
main()
{
   initBdd(6, 127, 61);

   /*-------- THIS IS JUST A TEST CODE ---------*/
   BddNode a(bm.getSupport(1));
   BddNode b(bm.getSupport(2));
   BddNode c(bm.getSupport(3));
   BddNode d(bm.getSupport(4));
   BddNode e(bm.getSupport(5));
   BddNode f(bm.getSupport(6));

   BddNode g1 = b & d;
   BddNode g2 = e & ~c;
   BddNode g3 = a & b;
   BddNode g4 = c & g1;
   BddNode g5 = g1 | g2;
   BddNode g6 = g2 | d;
   BddNode g7 = g6 & g3;
   BddNode g8 = g4 | g7;
   BddNode g9 = g8 & f;
   
   print_node(g1);
   print_node(g2);
   print_node(g3);
   print_node(g4);
   print_node(g5);
   print_node(g6);
   print_node(g7);
   print_node(g8);
   print_node(g9);

   /// problem b
   BddNode g9_ = g9 & g5;
   print_node(g9_);
   assert(g9 == g9_);

   /// problem c
   // g9'(g5=1)(a,f)
   // remove dependency on b,c,d,e
   BddNode g9_g5_1 = g9_ & g5;
   g9_g5_1 = g9_g5_1.getLeftCofactor(2) | g9_g5_1.getRightCofactor(2);
   g9_g5_1 = g9_g5_1.getLeftCofactor(3) | g9_g5_1.getRightCofactor(3);
   g9_g5_1 = g9_g5_1.getLeftCofactor(4) | g9_g5_1.getRightCofactor(4);
   g9_g5_1 = g9_g5_1.getLeftCofactor(5) | g9_g5_1.getRightCofactor(5);
   assert(g9_g5_1 == f);

   // g9'(g5=0)
   BddNode g9_g5_0 = g9_ & ~g5;
   g9_g5_0 = g9_g5_0.getLeftCofactor(2) | g9_g5_0.getRightCofactor(2);
   g9_g5_0 = g9_g5_0.getLeftCofactor(3) | g9_g5_0.getRightCofactor(3);
   g9_g5_0 = g9_g5_0.getLeftCofactor(4) | g9_g5_0.getRightCofactor(4);
   g9_g5_0 = g9_g5_0.getLeftCofactor(5) | g9_g5_0.getRightCofactor(5);
   assert(g9_g5_0 == BddNode::_zero);
   
   // Diff(g9'|g5)
   BddNode d_g9_g5 = g9_g5_0 ^ g9_g5_1;
   print_node(d_g9_g5);
   assert(d_g9_g5 == f);
   assert((d_g9_g5 & ~f) == BddNode::_zero);
   
   drawbddPNG(g9, "g9", "p6a");
   drawbddPNG(g9_, "g9_", "p6b");
   drawbddPNG(g9_g5_1, "g9_g5_1", "p6c.g9_g5_1");
   drawbddPNG(g9_g5_0, "g9_g5_0", "p6c.g9_g5_0");

   // cout << "g5" << endl;
   // g5.toMinterms();
   // cout << endl;

   // cout << "g9" << endl;
   // g9.toMinterms();
   // cout << endl;

   // cout << "g9_" << endl;
   // g9_.toMinterms();
   // cout << endl;

   // for (auto& cube : o2.getAllCubes()) {   
   //    cube.toMinterms();
   //    cout << endl;
   // }

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


/* 
g5
       bcde
onset: 0001
onset: 0011
onset: 1001
onset: 1010
onset: 1011
onset: 1110
onset: 1111
  
g9
       a bcde f
onset: 0 1110 1
onset: 0 1111 1
onset: 1 1001 1
onset: 1 1010 1
onset: 1 1011 1
onset: 1 1110 1
onset: 1 1111 1

*/