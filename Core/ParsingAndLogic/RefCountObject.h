// --------------------------------------------------------------------------
//
// RefCountObject.h
//
/// @file
/// Simple reference counting implementation.
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

// --------------------------------------------------------------------------
// IRCObject    
//
/// Interface for a reference counted object.
///
/// Typical usage pattern is:
///   Usually, the subclass does not directly implement this interface.
///   Implementation is deferred until instantiation at which point these
///   methods are implemented by wrapping the class with either the TLocalObject
///   or THeapObject template.
// --------------------------------------------------------------------------
class IRCObject
{
public:
   virtual long AddRef()  const = 0;
   virtual long GetRef()  const = 0;
   virtual long Release() const = 0;
};


// --------------------------------------------------------------------------
// THeapObject    
//
/// Implementation of reference counting behavior
///
/// Typical usage pattern is:
///   This class is used to instantiate reference counted objects on the heap.
///   When the object is completely released (m_nRef == 0), it deletes itself
///   The heap object may not be instatiated directly, but must be created 
///   through the templated static Create() function.
///   The Create function and constructors are templated and overloaded to 
//    allow varying numbers of initialization parameters.
// --------------------------------------------------------------------------
template<typename _Base>
class THeapObject
: public _Base
{
private:
   mutable long m_nRef;

protected:
   THeapObject() 
   : m_nRef(0) 
   {}
   
   template<typename _T1>
   THeapObject(const _T1& arg1) 
   : _Base(arg1)
   , m_nRef(0) 
   {}

   template<typename _T1, typename _T2>
   THeapObject(
      const _T1& arg1,
      const _T2& arg2
   )
   : _Base(arg1, arg2)
   , m_nRef(0)
   {}

   template<typename _T1, typename _T2, typename _T3>
   THeapObject(
      const _T1& arg1,
      const _T2& arg2,
      const _T3& arg3
   )
   : _Base(arg1, arg2, arg3)
   , m_nRef(0)
   {}

   template<typename _T1, typename _T2, typename _T3, typename _T4>
   THeapObject(
      const _T1& arg1,
      const _T2& arg2,
      const _T3& arg3,
      const _T4& arg4
   )
   : _Base(arg1, arg2, arg3, arg4)
   , m_nRef(0)
   {}

   virtual ~THeapObject() {}

public:
   typedef THeapObject<_Base> MyType;

   static MyType* Create()
   { return new MyType; }

   template<typename _T1>

	   static MyType* Create(const _T1& arg1)
   { return new MyType(arg1); }


   template<typename _T1, typename _T2>
   static MyType* Create(
      const _T1& arg1,
      const _T2& arg2
   )
   { return new MyType(arg1, arg2); }


   template<typename _T1, typename _T2, typename _T3>
   static MyType* Create(
      const _T1& arg1,
      const _T2& arg2,
      const _T3& arg3
   )
   { return new MyType(arg1, arg2, arg3); }

   template<typename _T1, typename _T2, typename _T3, typename _T4>
   static MyType* Create(
      const _T1& arg1,
      const _T2& arg2,
      const _T3& arg3,
      const _T4& arg4
   )
   { return new MyType(arg1, arg2, arg3, arg4); }

   virtual long AddRef()  const 
   {
      return ++m_nRef;
   }

   virtual long GetRef()  const
   {
      return m_nRef;
   }

   virtual long Release() const
   {
      long nRef = --m_nRef;
      if(nRef == 0)
      {
         delete this;
      }
      return nRef;
   }
};


// --------------------------------------------------------------------------
// TLocalObject    
//
/// Implementation of reference counting behavior for local objects 
///   (function local, class members, statics, globals)
///
/// Typical usage pattern is:
///   This class is used to instantiate local objects with the reference counting
///   protocol.  This allows Heap and Local objects to be mixed without 
///   changing the memory management strategy.
// --------------------------------------------------------------------------
template<typename _Base>
class TLocalObject
: public _Base
{
public:
   TLocalObject() {}

   template<typename _T1>
   TLocalObject(const _T1& arg1) 
   : _Base(arg1)
   {}

   template<typename _T1, typename _T2>
   TLocalObject(
      const _T1& arg1,
      const _T2& arg2
   )
   : _Base(arg1, arg2)
   {}

   template<typename _T1, typename _T2, typename _T3>
   TLocalObject(
      const _T1& arg1,
      const _T2& arg2,
      const _T3& arg3
   )
   : _Base(arg1, arg2, arg3)
   {}

   template<typename _T1, typename _T2, typename _T3, typename _T4>
   TLocalObject(
      const _T1& arg1,
      const _T2& arg2,
      const _T3& arg3,
      const _T4& arg4
   )
   : _Base(arg1, arg2, arg3, arg4)
   {}

   virtual ~TLocalObject() {}

   typedef TLocalObject<_Base> MyType;

   virtual long AddRef()  const { return -1; }
   virtual long GetRef()  const { return -1; }
   virtual long Release() const { return -1; }

   void* operator new(size_t nSize) { throw 0; return (void*)0L; }
   void operator delete(void* p) { throw 0; }
};


// --------------------------------------------------------------------------
// TRCPointer    
//
/// A smart pointer for reference counted objects.
///
/// Typical usage pattern is:
///   This class encapsulates the AddRef and Release calls.  On assignment or
///   initialization, the AddRef method is called.  On re-assignment or 
///   destruction of the pointer object, the Release method is called.
// --------------------------------------------------------------------------
template<typename _PT>
class TRCPointer
{
private:
   _PT* m_ptr;

public:
   typedef TRCPointer<_PT> MyType;

   TRCPointer() 
   : m_ptr(0L) 
   {
   }
   
   TRCPointer(_PT* ptr)  
   : m_ptr(0L)    
   { 
      Set(ptr); 
   }

   TRCPointer(const TRCPointer& ptr) 
   : m_ptr(0L)  
   { 
      Set(ptr.m_ptr); 
   }

   ~TRCPointer()                       
   { 
      Set(0L); 
   }

   MyType& operator=(_PT* ptr)
   {
      return Set(ptr);
   }

   MyType& operator=(const TRCPointer& ptr) 
   { 
      return Set(ptr.m_ptr); 
   }

   MyType& Set(_PT* ptr)
   {
      if(ptr == m_ptr) return *this;

      if(m_ptr) m_ptr->Release();
         
      m_ptr = ptr;

      if(m_ptr) m_ptr->AddRef();

      return *this;
   }

   _PT* Get() { return m_ptr; }

   const _PT* Get() const { return m_ptr; }

   bool IsNull() const { return m_ptr == 0L; }

   operator _PT* () { return m_ptr; }
   operator const _PT* () const { return m_ptr; }

   _PT& operator*() { return *m_ptr; }
   const _PT& operator*() const { return *m_ptr; }

   _PT* operator->() { return m_ptr; }
   const _PT* operator->() const { return m_ptr; }


   MyType& Create()
   { Set(THeapObject<_PT>::Create()); return *this; }

   template<typename _T1>
   MyType& Create(const _T1& arg1)
   { Set(THeapObject<_PT>::Create(arg1)); return *this; }

   template<typename _T1, typename _T2>
   MyType& Create(
      const _T1& arg1,
      const _T2& arg2
   )
   { Set(THeapObject<_PT>::Create(arg1,arg2)); return *this; }

   template<typename _T1, typename _T2, typename _T3>
   MyType& Create(
      const _T1& arg1,
      const _T2& arg2,
      const _T3& arg3
   )
   { Set(THeapObject<_PT>::Create(arg1,arg2,arg3)); return *this; }

   template<typename _T1, typename _T2, typename _T3, typename _T4>
   MyType& Create(
      const _T1& arg1,
      const _T2& arg2,
      const _T3& arg3,
      const _T4& arg4
   )
   { Set(THeapObject<_PT>::Create(arg1,arg2,arg3,arg4)); return *this; }
};

template<typename _PT>
inline bool operator<(const TRCPointer<_PT>& lhs, const TRCPointer<_PT>& rhs)
{
   return static_cast<const _PT*>(lhs) < static_cast<const _PT*>(rhs);
}

template<typename _PT>
inline bool operator>(const TRCPointer<_PT>& lhs, const TRCPointer<_PT>& rhs)
{
   return static_cast<const _PT*>(lhs) > static_cast<const _PT*>(rhs);
}

template<typename _PT>
inline bool operator==(const TRCPointer<_PT>& lhs, const TRCPointer<_PT>& rhs)
{
   return static_cast<const _PT*>(lhs) == static_cast<const _PT*>(rhs);
}

template<typename _PT>
inline bool operator!=(const TRCPointer<_PT>& lhs, const TRCPointer<_PT>& rhs)
{
   return !operator==(lhs, rhs);
}

template<typename _PT>
inline bool operator<=(const TRCPointer<_PT>& lhs, const TRCPointer<_PT>& rhs)
{
   return !operator>(lhs, rhs);
}

template<typename _PT>
inline bool operator>=(const TRCPointer<_PT>& lhs, const TRCPointer<_PT>& rhs)
{
   return !operator<(lhs, rhs);
}

template<typename Target, typename Current>
Target rcp_cast(TRCPointer<Current> rcp)
{
    #ifdef _CPPRTTI
    {
        try
        {
            return dynamic_cast<Target>(rcp.Get());
        }
        catch(...)
        {
            return NULL;
        }
    }
    #else
    {
        return reinterpret_cast<Target>(rcp.Get());
    }
    #endif
}
