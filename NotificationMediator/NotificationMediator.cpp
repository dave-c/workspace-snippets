#include "NotificationMediator.h"

NotificationMediator *NotificationMediator::_singleton = 0LL;

NotificationMediator::NotificationMediator()
{}

NotificationMediator::~NotificationMediator()
{
  for (SubjectEventMap::iterator it = _subjectEventMap.begin(); it != _subjectEventMap.end(); ++it)
  {
    for (EventBindingMap::iterator eit = it->second.begin(); eit != it->second.end(); ++eit)
    {
      deleteBindings(eit->second);
    }

    it->second.clear();
  }
  _subjectEventMap.clear();
}

NotificationMediator &
NotificationMediator::getSingleton()
{
  if (!_singleton)
    _singleton = new NotificationMediator();

  return *_singleton;
}

void
NotificationMediator::deleteBindings(BindingList &list)
{
  for (BindingList::iterator it = list.begin(); it != list.end(); ++it)
  {
    delete (*it);
  }
  list.clear();
}


NotificationMediator::Binding::Binding()
  : _observer(0)
{}

void
NotificationMediator::Binding::notify(Subject const &s, Event const &event)
{
  if (_observer)
    (*_observer.*(_callback._memberCallback))(s, event);
  else if (_callback._nonMemberCallback)
    (*(_callback._nonMemberCallback))(s, event);
}

void
NotificationMediator::Binding::reset()
{
  _observer = 0;
  _callback._memberCallback = 0;
  _callback._nonMemberCallback = 0;
}

void
NotificationMediator::removeBinding(void const *s, void const *o, Binding const &binding, std::type_info const *eventInfo)
{
  if (BindingList *bindings = hasBindings(s, eventInfo))
  {
    BindingList::iterator it = find(*bindings, binding);
    if (it != bindings->end())
    {
      bindings->remove(*it);
      if (o)
        _observerBindingMap[o].erase(*it);

      (*it)->reset();
      _bindingPool.push_back(*it);
    }
  }
}

NotificationMediator::Binding *
NotificationMediator::has(void const *s, Binding const &binding, std::type_info const *eventInfo)
{
  if (BindingList *bindings = hasBindings(s, eventInfo))
  {
    BindingList::iterator it = find(*bindings, binding);
    if (it != bindings->end())
      return *it;
  }
  return 0LL;
}

NotificationMediator::BindingList *
NotificationMediator::hasBindings(void const *s, std::type_info const *eventInfo)
{
  SubjectEventMap::iterator subjectEventIterator = _subjectEventMap.find(s);
  if (subjectEventIterator != _subjectEventMap.end())
  {
    EventBindingMap &eventBindings = subjectEventIterator->second;
    EventBindingMap::iterator eventBindingIterator = eventBindings.find(eventInfo);
    if (eventBindingIterator != eventBindings.end())
    {
      BindingList &bindings = eventBindingIterator->second;
      return &bindings;
    }
  }
  return 0LL;
}

NotificationMediator::BindingList::iterator
NotificationMediator::find(BindingList &list, Binding const &binding)
{
  BindingList::iterator n = list.begin();
  for (; n != list.end(); ++n)
  {
    if (**n == binding)
      break;
  }

  return n;
}


bool operator==(NotificationMediator::Binding const &first,
                NotificationMediator::Binding const &second)
{
  return (first._observer == second._observer) &&
         (first._callback._memberCallback == second._callback._memberCallback) &&
         (first._callback._nonMemberCallback == second._callback._nonMemberCallback) ;
}
