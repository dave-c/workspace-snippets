#ifndef BAR_H_
#define BAR_H_

#include "Traits.h"

class Bar
{
public:
  Bar ();
  virtual ~Bar ();
};

namespace Traits
{
  template <>
  struct CanUseWithBuzz<Bar>
  {
    static bool const value = true;
  };
}
#endif /* BAR_H_ */
