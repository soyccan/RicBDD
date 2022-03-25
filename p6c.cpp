/****************************************************************************
  FileName     [ p6c.cpp ]
  PackageName  [ ]
  Synopsis     [ Define main() ]
  Author       [ 陳正康 R10922187 ]
****************************************************************************/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "bddNode.h"
#include "bddMgr.h"

#define FILENAME "p6c"
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
   BddNode g9 = g8 & f & g5;
   
   print_node(g1);
   print_node(g2);
   print_node(g3);
   print_node(g4);
   print_node(g5);
   print_node(g6);
   print_node(g7);
   print_node(g8);
   print_node(g9);

   cout << "===================================" << endl;
   BddNode o1 = g5;
   print_node(o1);

   BddNode o2 = g9;
   print_node(o2);
   
   BddNode diff = (g9 & g5) ^ (~g9 & g5);
   print_node(diff);

//   cout << k.getLeftCofactor(1) << endl;
   // cout << k.getLeftCofactor(2) << endl;
//   cout << k.getRightCofactor(4) << endl;
   // cout << k.getRightCofactor(2) << endl;
   // BddNode l1 = k.exist(2);
   // cout << l1 << endl;
   // BddNode l2 = k.getLeftCofactor(2) | k.getRightCofactor(2);
   // cout << l2 << endl;

   ofstream ofile(FILENAME ".dot");
   diff.drawBdd(FILENAME, ofile);
   system("dot -o " FILENAME ".png -Tpng " FILENAME ".dot");

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

