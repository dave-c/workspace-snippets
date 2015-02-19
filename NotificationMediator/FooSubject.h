#ifndef FOOSUBJECT_H_
#define FOOSUBJECT_H_

#include "TestSubject.h"
#include "Event.h"

class FooSubject : public TestSubject
{
public:
  FooSubject();
  virtual ~FooSubject();

  class FooEvent : public Event {};
};

#endif /* FOOSUBJECT_H_ */
