#ifndef NOTIFICATIONMEDIATOR_H_
#define NOTIFICATIONMEDIATOR_H_

#include "Event.h"

#include <map>
#include <list>
#include <algorithm>
#include <set>

class NotificationMediator
{
public:
  struct NotificationContract
  {
    class Subject;
    class Observer;
    typedef void(Observer::*ObserverMemberCallback)(Subject const *s, Event const &event);
    typedef void(*Callback)(Subject const *s, Observer &o, Event const &event);

    NotificationContract()
    : _observer(0)
    , _observerCallback(0)
    , _nonMemberCallback(0)
    {}

    template <typename S, typename O, typename E>
    NotificationContract(S const &s, O &o, void(O::*callback)(S const *s, E const &event))
      : _observer((Observer *)&o)
      , _observerCallback((ObserverMemberCallback)callback)
      , _nonMemberCallback(0)
      {}

    template <typename S, typename O, typename E>
    NotificationContract(S const &s, O &o,  void(*callback)(S const *s, O &o, E const &event))
      : _observer((Observer *)&o)
      , _observerCallback(0)
      , _nonMemberCallback((Callback)callback)
      {}

    template <typename S, typename O, typename E>
    void bind(S const &s, O &o, void(O::*callback)(S const *s, E const &event))
    {
      _observer = (Observer *)&o;
      _observerCallback = (ObserverMemberCallback)callback;
    }

    template <typename S, typename O, typename E>
    void bind(S const &s, O &o, void(*callback)(S const *s, O &o, E const &event))
    {
      _observer = (Observer *)&o;
      _nonMemberCallback = (Callback)callback;
    }

    void notify(Subject const &s, Event const &event)
    {
      if (!_observer)
        return;

      if (_observerCallback)
        (*_observer.*_observerCallback)(&s, event);
      else if (_nonMemberCallback)
        (*_nonMemberCallback)(&s, *_observer, event);
    }

    void reset()
    {
      _observer = 0;
      _observerCallback = 0;
      _nonMemberCallback = 0;
    }

    friend bool operator==(NotificationContract const &first, NotificationContract const &second);

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
    NotificationContract contract(s, o, callback);
    if (!has(&s, contract))
    {
      _subjectContractMap[(void const*)&s].push_back(buildContract(s, o, callback));
    }
  }

  template <typename S, typename O, typename E>
  void connect(S const &s, O &o, void (*callback)(S const *s, O &o, E const &event))
  {
    NotificationContract contract(s, o, callback);
    if (!has(&s, contract))
    {
      _subjectContractMap[(void const*)&s].push_back(buildContract(s, o, callback));
    }
  }

  template <typename S, typename O, typename E>
  void disConnect(S const &s, O &o, void (O::*callback)(S const *s, E const &event))
  {
    NotificationContractList &list = _subjectContractMap[(void const *)&s];
    NotificationContract contract(s, o, callback);
    NotificationContractList::iterator it = find(list, contract);
    if (it != list.end())
    {
      list.remove(*it);
      (*it)->reset();
      _contractPool.push_back(*it);
    }
  }

  template <typename S, typename O, typename E>
  void disConnect(S const &s, O &o, void (*callback)(S const *s, O const *o, E const &event))
  {
    NotificationContractList &list = _subjectContractMap[(void const *)&s];
    NotificationContract contract(s, o, callback);
    NotificationContractList::iterator it = find(list, contract);
    if (it != list.end())
    {
      list.remove(*it);
      (*it)->reset();
      _contractPool.push_back(*it);
    }
  }

  template <typename S, typename E>
  void notify(S const &s, E const &event)
  {
    SubjectContractMap::iterator it = _subjectContractMap.find((void*)&s);
    if (it != _subjectContractMap.end())
    {
      NotificationContractList &contracts = it->second;
      for (NotificationContractList::iterator it = contracts.begin(); it != contracts.end(); ++it)
      {
        (*it)->notify((NotificationContract::Subject const &)s, event);
      }
    }
  }

private:
  static NotificationMediator *_singleton;

  typedef std::list<NotificationContract *> NotificationContractList;
  typedef std::map<size_t, NotificationContractList> EventContractMap;
  typedef std::map<void const *, NotificationContractList> SubjectContractMap;

  SubjectContractMap _subjectContractMap;
  std::list<NotificationContract *> _contractPool;

protected:
  NotificationContract *has(void const *s, NotificationContract &contract)
  {
    SubjectContractMap::iterator cit = _subjectContractMap.find(s);
    if (cit != _subjectContractMap.end())
    {
      NotificationContractList::iterator it = find(cit->second, contract);
      if (it != cit->second.end())
        return *it;
    }
    return 0LL;
  }

  NotificationContractList::iterator find(NotificationContractList &list, NotificationContract &contract)
  {
    NotificationContractList::iterator n = list.begin();
    for (; n != list.end(); ++n)
    {
      if (**n == contract)
        break;
    }

    return n;
  }

  template <typename S, typename O, typename E>
  NotificationContract *buildContract(S const &s, O &o, void (O::*callback)(S const *s, E const &event))
  {
    NotificationContract *contract = 0LL;
    if (_contractPool.empty())
    {
      contract = new NotificationContract(s, o, callback);
    }
    else
    {
      contract = _contractPool.back();
      contract->bind(s, o, callback);
      _contractPool.pop_back();
    }

    return contract;
  }

  template <typename S, typename O, typename E>
  NotificationContract *buildContract(S const &s, O &o, void (*callback)(S const *s, O &o, E const &event))
  {
    NotificationContract *contract = 0LL;
    if (_contractPool.empty())
    {
      contract = new NotificationContract(s, o, callback);
    }
    else
    {
      contract = _contractPool.back();
      contract->bind(s, o, callback);
      _contractPool.pop_back();
    }

    return contract;
  }
};

bool operator==(NotificationMediator::NotificationContract const &first,
                NotificationMediator::NotificationContract const &second);
#endif /* NOTIFICATIONMEDIATOR_H_ */
