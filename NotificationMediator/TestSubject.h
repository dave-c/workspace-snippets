#ifndef TESTSUBJECT_H_
#define TESTSUBJECT_H_

#include "Event.h"

class TestSubject
{
public:
  TestSubject();
  virtual ~TestSubject();

  class TestEvent : public Event {};
  class OtherEvent : public Event {};
};

#endif /* TESTSUBJECT_H_ */
