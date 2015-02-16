#ifndef _NotificationMediator_h_
#define _NotificationMediator_h_

#include "Event.h"

#include <map>
#include <list>
#include <typeinfo>

class NotificationMediator
{
public:
  struct Binding
  {
    class Subject;
    class Observer;
    typedef void(Observer::*ObserverMemberCallback)(Subject const *s, Event const &event);
    typedef void(*Callback)(Subject const *s, Observer &o, Event const &event);

    Binding();

    template <typename S, typename O, typename E>
    Binding(S const &s, O &o, void(O::*callback)(S const *s, E const &event))
      : _observer((Observer *)&o)
      , _observerCallback((ObserverMemberCallback)callback)
      , _nonMemberCallback(0)
      {}

    template <typename S, typename O, typename E>
    Binding(S const &s, O &o,  void(*callback)(S const *s, O &o, E const &event))
      : _observer((Observer *)&o)
      , _observerCallback(0)
      , _nonMemberCallback((Callback)callback)
      {}

    template <typename S, typename O, typename E>
    void bind(S const &s, O &o, void(O::*callback)(S const *s, E const &event))
    {
      _observer = (Observer *)&o;
      _observerCallback = (ObserverMemberCallback)callback;
      _nonMemberCallback = 0LL;
    }

    template <typename S, typename O, typename E>
    void bind(S const &s, O &o, void(*callback)(S const *s, O &o, E const &event))
    {
      _observer = (Observer *)&o;
      _observerCallback = 0LL;
      _nonMemberCallback = (Callback)callback;
    }

    void notify(Subject const &s, Event const &event);
    void reset();

    friend bool operator==(Binding const &first, Binding const &second);

    Observer *_observer;
    ObserverMemberCallback _observerCallback;
    Callback _nonMemberCallback;
  };

public:
  NotificationMediator();
  virtual ~NotificationMediator();

  static NotificationMediator &getSingleton();

  template <typename S, typename O, typename E>
  void connect(S const &s, O &o, void (O::*callback)(S const *s, E const &event))
  {
    Binding binding(s, o, callback);
    if (!has(&s, binding, &typeid(E)))
    {
      _subjectEventMap[(void const*)&s][&typeid(E)].push_back(buildbinding(s, o, callback));
    }
  }

  template <typename S, typename O, typename E>
  void connect(S const &s, O &o, void (*callback)(S const *s, O &o, E const &event))
  {
    Binding binding(s, o, callback);
    if (!has(&s, binding, &typeid(E)))
    {
      _subjectEventMap[(void const*)&s][&typeid(E)].push_back(buildbinding(s, o, callback));
    }
  }

  template <typename S, typename O, typename E>
  void disConnect(S const &s, O &o, void (O::*callback)(S const *s, E const &event))
  {
    Binding binding(s, o, callback);
    removeBinding(&s, binding, &typeid(E));
  }

  template <typename S, typename O, typename E>
  void disConnect(S const &s, O &o, void (*callback)(S const *s, O &o, E const &event))
  {
    Binding binding(s, o, callback);
    removeBinding(&s, binding, &typeid(E));
  }

  template <typename S, typename E>
  void notify(S const &s, E const &event)
  {
    if (BindingList *bindings = hasBindings(&s, &typeid(E)))
    {
      for (BindingList::iterator it = bindings->begin(); it != bindings->end(); ++it)
      {
        (*it)->notify((Binding::Subject const &)s, event);
      }
    }
  }

private:
  struct EventComparator
  {
    bool operator()(std::type_info const* first, std::type_info const *second)
    {
      return first->before(*second);
    }
  };

  static NotificationMediator *_singleton;

  typedef std::list<Binding *> BindingList;
  typedef std::map<std::type_info const *, BindingList, EventComparator> EventBindingMap;
  typedef std::map<void const *, EventBindingMap> SubjectEventMap;

  SubjectEventMap _subjectEventMap;
  BindingList _bindingPool;

  void removeBinding(void const *s, Binding const &binding, std::type_info const *eventInfo);

  Binding *has(void const *s, Binding const &binding, std::type_info const *eventInfo);

  BindingList *hasBindings(void const *s, std::type_info const *eventInfo);

  BindingList::iterator find(BindingList &list, Binding const &binding);

  void deleteBindings(BindingList &list);

  template <typename S, typename O, typename E>
  Binding *buildbinding(S const &s, O &o, void (O::*callback)(S const *s, E const &event))
  {
    Binding *binding = 0LL;
    if (_bindingPool.empty())
    {
      binding = new Binding(s, o, callback);
    }
    else
    {
      binding = _bindingPool.back();
      binding->bind(s, o, callback);
      _bindingPool.pop_back();
    }

    return binding;
  }

  template <typename S, typename O, typename E>
  Binding *buildbinding(S const &s, O &o, void (*callback)(S const *s, O &o, E const &event))
  {
    Binding *binding = 0LL;
    if (_bindingPool.empty())
    {
      binding = new Binding(s, o, callback);
    }
    else
    {
      binding = _bindingPool.back();
      binding->bind(s, o, callback);
      _bindingPool.pop_back();
    }

    return binding;
  }
};

bool operator==(NotificationMediator::Binding const &first,
                NotificationMediator::Binding const &second);
#endif
