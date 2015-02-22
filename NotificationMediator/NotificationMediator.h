#ifndef _NotificationMediator_h_
#define _NotificationMediator_h_

#include "Event.h"

#include <map>
#include <list>
#include <typeinfo>
#include <iostream>

class NotificationMediator
{
protected:
  struct Binding
  {
    class Subject;
    class Observer;
    typedef void(Observer::*MemberCallback)(Subject const &s, Event const &event);
    typedef void(*NonMemberCallback)(Subject const &s, Event const &event);

    Binding();

    template <typename S, typename O, typename E>
    Binding(S const &s, O &o, void(O::*callback)(S const &s, E const &event))
      : _observer((Observer *)&o)
      , _callback((MemberCallback)callback)
      {}

    template <typename S, typename E>
    Binding(S const &s, void(*callback)(S const &s, E const &event))
      : _observer(0)
      , _callback((NonMemberCallback)callback)
      {}

    template <typename S, typename O, typename E>
    void bind(S const &s, O &o, void(O::*callback)(S const &s, E const &event))
    {
      _observer = (Observer *)&o;
      _callback._memberCallback = (MemberCallback)callback;
    }

    template <typename S, typename E>
    void bind(S const &s, void(*callback)(S const &s, E const &event))
    {
      _observer = 0LL;
      _callback._nonMemberCallback = (NonMemberCallback)callback;
    }

    void notify(Subject const &s, Event const &event);
    void reset();

    Observer *_observer;
    union Callback
    {
      Callback() {}

      template <typename S, typename O, typename E>
      Callback(void(O::*callback)(S const &s, E const &event))
        : _memberCallback(callback)
      {}

      template <typename S, typename E>
      Callback(void(*callback)(S const &s, E const &event))
        : _nonMemberCallback(callback)
      {}

      MemberCallback _memberCallback;
      NonMemberCallback _nonMemberCallback;
    } _callback;
  };

  class BindingList : public std::list<Binding *>
  {
  public:
    BindingList()
      : std::list<Binding *>()
      , _inNotify(false)
    {}

    void setInNotify(bool inNotify) { _inNotify = inNotify; }
    bool inNotify() const { return _inNotify; }

    BindingList(BindingList const &other)
      : std::list<Binding *>(other)
      , _inNotify(other._inNotify)
    {}

  private:
    bool _inNotify;
  };

  friend bool operator==(Binding const &first, Binding const &second);

public:
  NotificationMediator();
  virtual ~NotificationMediator();

  static NotificationMediator &getSingleton();

  template <typename S, typename O, typename E>
  void connect(S const &s, O &o, void (O::*callback)(S const &s, E const &event))
  {
    Binding binding(s, o, callback);
    if (!has(&s, binding, &typeid(E)))
    {
      std::cout << "Adding binding for (" << &s << ")" << std::endl;
      _subjectEventMap[(void const*)&s][&typeid(E)].push_back(createBinding(s, o, callback));
    }
  }

  template <typename S, typename E>
  void connect(S const &s, void (*callback)(S const &s, E const &event))
  {
    Binding binding(s, callback);
    if (!has(&s, binding, &typeid(E)))
    {
      _subjectEventMap[(void const*)&s][&typeid(E)].push_back(createBinding(s, callback));
    }
  }

  template <typename S, typename O, typename E>
  void disConnect(S const &s, O &o, void (O::*callback)(S const &s, E const &event))
  {
    Binding binding(s, o, callback);
    removeBinding(&s, binding, &typeid(E));
  }

  template <typename S, typename E>
  void disConnect(S const &s, void (*callback)(S const &s, E const &event))
  {
    Binding binding(s, callback);
    removeBinding(&s, binding, &typeid(E));
  }

  template <typename S, typename E>
  void notify(S const &s, E const &event)
  {
    if (BindingList *bindings = hasBindings(&s, &typeid(E)))
    {
      std::cout << "Found bindings for (" << &s << ")" << std::endl;
      // Copy the binding list for connect/disconnect during notification
      if (bindings->inNotify())
        return;

      struct ScopedNotify
      {
        ScopedNotify(BindingList *bindings)
          : _bindings(bindings)
        {
          _bindings->setInNotify(true);
        }

        ~ScopedNotify()
        {
          _bindings->setInNotify(false);
        }

        BindingList *_bindings;
      } _scopedNotify(bindings);

      BindingList copy(*bindings);
      std::cout << "Have (" << bindings->size() << ") bindings" << std::endl;
      for (BindingList::iterator it = copy.begin(); it != copy.end(); ++it)
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
  Binding *createBinding(S const &s, O &o, void (O::*callback)(S const &s, E const &event))
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

  template <typename S, typename E>
  Binding *createBinding(S const &s, void (*callback)(S const &s, E const &event))
  {
    Binding *binding = 0LL;
    if (_bindingPool.empty())
    {
      binding = new Binding(s, callback);
    }
    else
    {
      binding = _bindingPool.back();
      binding->bind(s, callback);
      _bindingPool.pop_back();
    }

    return binding;
  }
};

bool operator==(NotificationMediator::Binding const &first,
                NotificationMediator::Binding const &second);
#endif
