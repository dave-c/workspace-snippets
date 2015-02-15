#include "NotificationMediator.h"
#include "TestSubject.h"
#include "TestObserver.h"

#include <iostream>

void receiver(TestSubject const *, TestObserver &o, TestSubject::TestEvent const &event)
{
  std::cout << "receiver is called" << std::endl;
}

int main()
{
  TestSubject subject;
  TestObserver observer;

  NotificationMediator::getSingleton().connect(subject, observer, &TestObserver::callback);
  NotificationMediator::getSingleton().connect(subject, observer, &receiver);

  NotificationMediator::getSingleton().notify(subject, TestSubject::TestEvent());
  NotificationMediator::getSingleton().notify(subject, TestSubject::OtherEvent());

  NotificationMediator::getSingleton().disConnect(subject, observer, &TestObserver::callback);
  NotificationMediator::getSingleton().notify(subject, TestSubject::TestEvent());

  NotificationMediator::getSingleton().connect(subject, observer, &TestObserver::callback);
  NotificationMediator::getSingleton().notify(subject, TestSubject::TestEvent());
}

