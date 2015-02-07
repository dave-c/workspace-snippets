#ifndef BUZZ_H_
#define BUZZ_H_

#include <iostream>
#include <type_traits>
#include <typeinfo>
#include "Traits.h"

template <typename T, bool E = Traits::CanUseWithBuzz<T>::value>
void buzz(T const &t)
{
  if (E)
    std::cout << "Supported with buzz" << std::endl;
  else
    std::cout << "Not supported with buzz" << std::endl;
}


template <typename T>
std::enable_if<Traits::CanUseWithBuzz<T>::value, void> bizzBuzz(T const &t)
{
  std::cout << "Compiles because (" << typeid(T).name() << ") is compatible" << std::endl;
}

#endif /* BUZZ_H_ */
