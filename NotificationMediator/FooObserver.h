#ifndef FOOOBSERVER_H_
#define FOOOBSERVER_H_

#include "TestObserver.h"

class FooSubject;

class FooObserver : public TestObserver
{
public:
  FooObserver();
  virtual ~FooObserver();

  void callback(FooSubject const &, TestSubject::TestEvent const &)
  {
    std::cout << "FooObserver: getting call back" << std::endl;
  }

  virtual void virtualCallback(TestSubject const &, TestSubject::TestEvent const &)
  {
    std::cout << "FooObserver: getting virtual call back" << std::endl;
  }

  void anotherCallback(TestSubject const &, TestSubject::TestEvent const &)
  {
    std::cout << "FooObserver: getting another call back" << std::endl;
  }

  void yetAnotherCallback(FooSubject const &, TestSubject::TestEvent const &)
  {
    std::cout << "FooObserver: getting yet another call back" << std::endl;
  }
};

#endif /* FOOOBSERVER_H_ */
