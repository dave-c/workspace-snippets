#include "NotificationMediator.h"

NotificationMediator *NotificationMediator::_singleton = 0LL;

NotificationMediator::NotificationMediator()
{}

NotificationMediator::~NotificationMediator()
{}


NotificationMediator &
NotificationMediator::getSingleton()
{
  if (!_singleton)
    _singleton = new NotificationMediator();

  return *_singleton;
}


bool operator==(NotificationMediator::Binding const &first,
                NotificationMediator::Binding const &second)
{
  return (first._observer == second._observer) &&
         (first._observerCallback == second._observerCallback) &&
         (first._nonMemberCallback == second._nonMemberCallback);
}
