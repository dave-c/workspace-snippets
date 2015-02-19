#ifndef TESTOBSERVER_H_
#define TESTOBSERVER_H_

#include <iostream>

#include "TestSubject.h"

class TestObserver
{
public:
  TestObserver();
  virtual ~TestObserver();

  void callback(TestSubject const &, TestSubject::TestEvent const &)
  {
    std::cout << "TestObserver: getting call back" << std::endl;
  }

  virtual void virtualCallback(TestSubject const &, TestSubject::TestEvent const &)
  {
    std::cout << "TestObserver: getting virtual call back" << std::endl;
  }
};

#endif /* TESTOBSERVER_H_ */
