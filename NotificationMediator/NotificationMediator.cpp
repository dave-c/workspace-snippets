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


bool operator==(NotificationMediator::NotificationContract const &first,
                NotificationMediator::NotificationContract const &second)
{
  return (first._observer == second._observer) &&
         (first._observerCallback == second._observerCallback) &&
         (first._nonMemberCallback == second._nonMemberCallback);
}
