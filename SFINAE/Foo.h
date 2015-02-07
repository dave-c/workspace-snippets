#ifndef FOO_H_
#define FOO_H_

#include "Traits.h"

class Foo
{
public:
  Foo();
  virtual ~Foo();
};


namespace Traits
{
  template <>
  struct CanUseWithBuzz<Foo>
  {
    static bool const value = false;
  };
}

#endif /* FOO_H_ */
