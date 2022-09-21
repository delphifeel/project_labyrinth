/**
 * @defgroup   CORE-disjoint-set
 *
 * @brief      
    Disjoint set.
    Created from numbers according to setup size.

    Example: 
        for size 10 will be created 10 sets
        {0} {1} {2} {3} {4} {5} {6} {7} {8} {9}
 *
 * @author     Delphifeel
 * @date       2022
 */

#ifndef _CORE_DISJOINT_SET_H_
#define _CORE_DISJOINT_SET_H_

#include <cstring>
#include <cstdlib>
#include "core-types.h"

namespace core {

class DisjointSet
{
public:
   uint GetSubsetsCount() const 
   {
      return m_subsets_count;
   }

   bool Union(int value1, int value2)
   {
      int value1_subset;
      int value2_subset;


      _FindSubset(value1, &value1_subset);
      _FindSubset(value2, &value2_subset);

      if (value1_subset == value2_subset) {
         return false;
      }

      m_child_to_parent[value1_subset] = value2_subset;
      m_subsets_count--;

      return true;
   }

   explicit DisjointSet(uint size)
   {
      m_child_to_parent_size  = size;
      m_subsets_count         = size;
      m_child_to_parent       = new int[size];
      memset(m_child_to_parent, -1, sizeof(int) * size);
   }

   ~DisjointSet()
   {
      delete m_child_to_parent;
   }

private:
   void _FindSubset(int value, int *subset) const
   {
      int parent;

      if ( ( parent = m_child_to_parent[value] ) != -1 ) {
         _FindSubset(parent, subset);
      } else {
         *subset = value;
      }
   }

   uint         m_child_to_parent_size;
   uint         m_subsets_count;
   int         *m_child_to_parent;
};

}


#endif