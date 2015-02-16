#include "NotificationMediator.h"
#include "TestSubject.h"
#include "TestObserver.h"

#include <iostream>

void receiver(TestSubject const *, TestObserver &o, TestSubject::TestEvent const &event)
{
  std::cout << "TestEvent receiver is called" << std::endl;
}

void otherReceiver(TestSubject const *, TestObserver &o, TestSubject::OtherEvent const &event)
{
  std::cout << "OtherEvent receiver is called" << std::endl;
}

int main()
{
  TestSubject subject;
  TestObserver observer;

  NotificationMediator::getSingleton().connect(subject, observer, &TestObserver::callback);
  NotificationMediator::getSingleton().connect(subject, observer, &receiver);
  NotificationMediator::getSingleton().connect(subject, observer, &otherReceiver);

  NotificationMediator::getSingleton().notify(subject, TestSubject::TestEvent());
  NotificationMediator::getSingleton().notify(subject, TestSubject::OtherEvent());

  NotificationMediator::getSingleton().disConnect(subject, observer, &TestObserver::callback);
  NotificationMediator::getSingleton().notify(subject, TestSubject::TestEvent());

  NotificationMediator::getSingleton().connect(subject, observer, &TestObserver::callback);
  NotificationMediator::getSingleton().notify(subject, TestSubject::TestEvent());

  NotificationMediator::getSingleton().disConnect(subject, observer, &otherReceiver);
  NotificationMediator::getSingleton().notify(subject, TestSubject::OtherEvent());
}

