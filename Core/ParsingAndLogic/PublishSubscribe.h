// --------------------------------------------------------------------------
//
// PublishSubscribe.h
//
/// Publisher / Subscriber interface.
///
/// Implements the publish/subscribe pattern, also known as the 
/// subject/observer pattern, where a Subscriber (observer) registers
/// with a Publisher (subject) to be notified when any interesting change 
/// occurs. This implementation is templatized on the notification object
/// to give implementors complete control over what information
/// is communicated between the Publisher and Subscriber.
///
/// @file
/// @ingroup pubsubmodule
//
// Copyright (c) 2006 QUALCOMM Inc.
//
// All Rights Reserved.  QUALCOMM Proprietary
//
// Export of this technology or software is regulated by the U.S. Government.
// Diversion contrary to U.S. law prohibited.
//
// --------------------------------------------------------------------------

#pragma once

#include <list>
#include <algorithm>

template<typename NotifyType> class Publisher;

// --------------------------------------------------------------------------
//
// Subscriber class declaration.
//
/// Implements the subscribe part of the publish/subscribe pattern
///    (analogous to the observer part of the subject/observer pattern).
///
/// Typical usage pattern is:
/// - You implement subclass of Subscriber and find or write a Publisher 
///     subclass that you want to subscribe to and call SubscribeTo() 
///     to subscribe to it (subscribe to subscribe to). For example,
///     a user interface class might subscribe to a data class to be 
///     notified any time the data changes.
/// - When something changes in the Publisher that it wants subscribers to 
///     know about, it calls its own NotifySubscribers() method which calls
///     OnPublisherNotify() for each subscriber. The notification object
///     is templatized so you have complete control over the information
///     you send to the Subscriber. Usually the notification object is
///     a representation of the thing that caused the Publisher
///     to want to notify you.
///     Continuing the example above, if the data class notified subscribers
///     of a name change, say, then the user interface class would update
///     its display to reflect the new name.
/// - Alternatively, if you want to really spice things up you should
///     take a look at ActionHandler.h, which implements subclasses of
///     Publisher and Subscriber that automatically generate specific
///     ActionHandler objects when notifications occur.
/// - If you ever need to cancel your subscription you call 
///     UnsubscribeTo() to unsubscribe.
///
/// @see Publisher
/// @ingroup pubsubmodule
//
// --------------------------------------------------------------------------
template<typename NotifyType> class Subscriber
{
public:
   Subscriber() {}
   virtual ~Subscriber() {}

   /// Subscribe to a publisher.
   virtual void SubscribeTo(Publisher<NotifyType>& publisher)
   {
      publisher.AddSubscriber(this);
   }

   /// Unsubscribe to a publisher.
   virtual void UnsubscribeTo(Publisher<NotifyType>& publisher)
   {
      publisher.RemoveSubscriber(this);
   }

   /// Respond to a publisher notification.
   virtual void OnPublisherNotify(const Publisher<NotifyType>&,const NotifyType&) = 0;
};

// --------------------------------------------------------------------------
//
// Publisher class declaration.
//
/// Implements the publish part of the publish/subscribe pattern
///    (analogous to the subject part of the subject/observer pattern).
///
/// Typical usage pattern is:
/// - You implement a subclass of Publisher if you know other classes will
///     be interested in finding out when things happen to your class, and
///     especially when those other classes exist in a different scope
///     from your class. For example, data classes are often publishers
///     because user interface classes often need to know when the
///     data changes, and you want the loosest possible coupling between
///     the classes because they typically exist in different scopes and
///     so that they can evolve independently.
/// - Classes that want to be notified of changes call their SubscribeTo()
///     method which calls AddSubscriber() to add them to the list. 
///     If a class no longer wants notifications it calls its UnsubscribeTo() 
///     which calls RemoveSubscriber() to remove it from the list.
/// - When something changes that you want subscribers to know about, you 
///     call NotifySubscribers(), which calls Subscriber::OnPublisherNotify() 
///     for each subscriber. The notification object
///     is templatized so you have complete control over the information
///     you send to the Subscriber. Usually the notification object is
///     a representation of the thing that caused the notification.
///
/// @see Subscriber
/// @ingroup pubsubmodule
//
// --------------------------------------------------------------------------
template<typename NotifyType> class Publisher
{
public:
   // subscriber list and iter typedefs
   typedef std::list<typename Subscriber<NotifyType>*> SubscriberList;
   typedef typename SubscriberList::iterator SubscriberListIter;

   Publisher() {}
   virtual ~Publisher() {}

   /// Add subscriber.
   void AddSubscriber(Subscriber<NotifyType>* pSubscriber)
   {
      // !!! ASSERT_AND_RETURN_IF(std::find(m_subscriberList.begin(),
      // !!!   m_subscriberList.end(),pSubscriber) != m_subscriberList.end());
      m_subscriberList.push_back(pSubscriber);
   }

   /// Remove subscriber.
   void RemoveSubscriber(Subscriber<NotifyType>* pSubscriber)
   {
      SubscriberListIter i = 
         std::find(m_subscriberList.begin(),m_subscriberList.end(),pSubscriber);
      // !!! ASSERT_AND_RETURN_IF(i == m_subscriberList.end());
      m_subscriberList.erase(i);
   }

   /// Unary function to notify one subscriber.
   struct NotifySubscriber : std::unary_function<Subscriber<NotifyType>*, void>
   {
      NotifySubscriber(
         const Publisher<NotifyType>& publisher,
         const NotifyType& action
         ) : 
         m_publisher(publisher),
         m_action(action)
      {}
      void operator() (Subscriber<NotifyType>* pSubscriber)
      {
         pSubscriber->OnPublisherNotify(m_publisher,m_action);
      }
      const Publisher<NotifyType>& m_publisher;
      const NotifyType& m_action;
   };

   /// Notify subscribers that an action has been performed.
   void NotifySubscribers(const NotifyType& action) const
   {
      std::for_each(m_subscriberList.begin(),m_subscriberList.end(),
         NotifySubscriber(*this,action));
   }

private:
   /// subscriber list
   SubscriberList m_subscriberList;
};

// doxygen pubsubmodule declaration (must be C-style comment)
/** @defgroup pubsubmodule Publish/subscribe module */
