#include "NotificationMediator.h"
#include "TestSubject.h"
#include "TestObserver.h"
#include "FooSubject.h"
#include "FooObserver.h"

#include <iostream>

void receiver(TestSubject const &, TestObserver &o, TestSubject::TestEvent const &event)
{
  std::cout << "TestEvent receiver is called" << std::endl;
}

void otherReceiver(TestSubject const &, TestObserver &o, TestSubject::OtherEvent const &event)
{
  std::cout << "OtherEvent receiver is called" << std::endl;
}

int main()
{
  TestSubject subject;
  TestObserver observer;
  FooSubject fooSubject;
  FooObserver fooObserver;
  TestObserver *fooObserver_p = &fooObserver;

  std::cout << std::endl;
  std::cout << "Testing..." << std::endl;
  std::cout << std::endl;
  NotificationMediator::getSingleton().connect(subject, observer, &TestObserver::callback);
  NotificationMediator::getSingleton().connect(subject, observer, &receiver);
  NotificationMediator::getSingleton().connect(subject, observer, &otherReceiver);

  NotificationMediator::getSingleton().notify(subject, TestSubject::TestEvent());
  NotificationMediator::getSingleton().notify(subject, TestSubject::OtherEvent());

  std::cout << std::endl;
  std::cout << "Testing..." << std::endl;
  std::cout << std::endl;
  NotificationMediator::getSingleton().disConnect(subject, observer, &TestObserver::callback);
  NotificationMediator::getSingleton().notify(subject, TestSubject::TestEvent());

  std::cout << std::endl;
  std::cout << "Testing..." << std::endl;
  std::cout << std::endl;
  NotificationMediator::getSingleton().connect(subject, observer, &TestObserver::callback);
  NotificationMediator::getSingleton().notify(subject, TestSubject::TestEvent());
  NotificationMediator::getSingleton().disConnect(subject, observer, &TestObserver::callback);

  std::cout << std::endl;
  std::cout << "Testing..." << std::endl;
  std::cout << std::endl;
  NotificationMediator::getSingleton().disConnect(subject, observer, &otherReceiver);
  NotificationMediator::getSingleton().notify(subject, TestSubject::OtherEvent());
  NotificationMediator::getSingleton().disConnect(subject, observer, &receiver);

  std::cout << std::endl;
  std::cout << "Testing..." << std::endl;
  std::cout << std::endl;
  NotificationMediator::getSingleton().connect(fooSubject, fooObserver, &FooObserver::callback);

  NotificationMediator::getSingleton().notify(fooSubject, TestSubject::TestEvent());

  NotificationMediator::getSingleton().disConnect(fooSubject, fooObserver, &FooObserver::callback);

  std::cout << std::endl;
  std::cout << "Testing..." << std::endl;
  std::cout << "NotificationMediator::getSingleton().connect(subject, observer, &TestObserver::virtualCallback);" << std::endl;
  std::cout << "NotificationMediator::getSingleton().connect(subject, *fooObserver_p, &TestObserver::virtualCallback);" << std::endl;
  std::cout << "NotificationMediator::getSingleton().notify(subject, TestSubject::TestEvent());" << std::endl;
  std::cout << "NotificationMediator::getSingleton().disConnect(subject, observer, &TestObserver::virtualCallback);" << std::endl;
  std::cout << "NotificationMediator::getSingleton().disConnect(subject, *fooObserver_p, &TestObserver::virtualCallback);" << std::endl;
  std::cout << std::endl;

  NotificationMediator::getSingleton().connect(subject, observer, &TestObserver::virtualCallback);
  NotificationMediator::getSingleton().connect(subject, *fooObserver_p, &TestObserver::virtualCallback);

  NotificationMediator::getSingleton().notify(subject, TestSubject::TestEvent());

  NotificationMediator::getSingleton().disConnect(subject, observer, &TestObserver::virtualCallback);
  NotificationMediator::getSingleton().disConnect(subject, *fooObserver_p, &TestObserver::virtualCallback);

  std::cout << std::endl;
  std::cout << "Testing..." << std::endl;
  std::cout << "NotificationMediator::getSingleton().connect((TestSubject)fooSubject, fooObserver, &FooObserver::anotherCallback);" << std::endl;
  std::cout << "NotificationMediator::getSingleton().connect(fooSubject, fooObserver, &FooObserver::yetAnotherCallback);" << std::endl;
  std::cout << "NotificationMediator::getSingleton().notify((TestSubject)fooSubject, TestSubject::TestEvent());" << std::endl;
  std::cout << "NotificationMediator::getSingleton().notify(fooSubject, TestSubject::TestEvent());" << std::endl;
  std::cout << "NotificationMediator::getSingleton().notify(subject, TestSubject::TestEvent());" << std::endl;
  std::cout << std::endl;

  NotificationMediator::getSingleton().connect((TestSubject const &)fooSubject, fooObserver, &FooObserver::anotherCallback);
  NotificationMediator::getSingleton().connect(fooSubject, fooObserver, &FooObserver::yetAnotherCallback);

  NotificationMediator::getSingleton().notify((TestSubject const &)fooSubject, TestSubject::TestEvent());
  NotificationMediator::getSingleton().notify(fooSubject, TestSubject::TestEvent());
  NotificationMediator::getSingleton().notify(subject, TestSubject::TestEvent());
}

