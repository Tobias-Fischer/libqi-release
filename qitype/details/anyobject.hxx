#pragma once
/*
**  Copyright (C) 2013 Aldebaran Robotics
**  See COPYING for the license
*/

#ifndef _QITYPE_DETAILS_GENERICOBJECT_HXX_
#define _QITYPE_DETAILS_GENERICOBJECT_HXX_

#include <boost/mpl/if.hpp>

#include <qi/future.hpp>
#include <qitype/typeinterface.hpp>
#include <qitype/typeobject.hpp>
#include <qitype/details/typeimpl.hxx>

namespace qi {

  namespace detail {
    typedef std::map<TypeInfo, boost::function<AnyReference(AnyObject)> > ProxyGeneratorMap;
    QITYPE_API ProxyGeneratorMap& proxyGeneratorMap();

  // bounce to a genericobject obtained by (O*)this->asAnyObject()
    /* Everything need to be const:
    *  anyobj.call bounces to anyobj.asObject().call, and the
    * second would work on const object
    */
    template<typename O> class GenericObjectBounce
    {
    public:
      const MetaObject &metaObject() const { return go()->metaObject();}
      inline qi::Future<AnyReference> metaCall(unsigned int method, const GenericFunctionParameters& params, MetaCallType callType = MetaCallType_Auto) const
      {
        return go()->metaCall(method, params, callType);
      }
      inline unsigned int findMethod(const std::string& name, const GenericFunctionParameters& parameters) const
      {
        return go()->findMethod(name, parameters);
      }
      inline qi::Future<AnyReference> metaCall(const std::string &nameWithOptionalSignature, const GenericFunctionParameters& params, MetaCallType callType = MetaCallType_Auto) const
      {
        return go()->metaCall(nameWithOptionalSignature, params, callType);
      }
      inline void metaPost(unsigned int event, const GenericFunctionParameters& params) const
      {
        return go()->metaPost(event, params);
      }
      inline void metaPost(const std::string &nameWithOptionalSignature, const GenericFunctionParameters &in) const
      {
        return go()->metaPost(nameWithOptionalSignature, in);
      }
      inline void post(const std::string& eventName,
        qi::AutoAnyReference p1 = qi::AutoAnyReference(),
        qi::AutoAnyReference p2 = qi::AutoAnyReference(),
        qi::AutoAnyReference p3 = qi::AutoAnyReference(),
        qi::AutoAnyReference p4 = qi::AutoAnyReference(),
        qi::AutoAnyReference p5 = qi::AutoAnyReference(),
        qi::AutoAnyReference p6 = qi::AutoAnyReference(),
        qi::AutoAnyReference p7 = qi::AutoAnyReference(),
        qi::AutoAnyReference p8 = qi::AutoAnyReference()) const
      {
        return go()->post(eventName, p1, p2, p3, p4, p5, p6, p7, p8);
      }
      template <typename FUNCTOR_TYPE>
      inline qi::FutureSync<SignalLink> connect(const std::string& eventName, FUNCTOR_TYPE callback,
        MetaCallType threadingModel = MetaCallType_Direct) const
      {
        return go()->connect(eventName, callback, threadingModel);
      }
      inline qi::FutureSync<SignalLink> connect(const std::string &name, const SignalSubscriber& functor) const
      {
        return go()->connect(name, functor);
      }
      inline qi::FutureSync<SignalLink> connect(unsigned int signal, const SignalSubscriber& subscriber) const
      {
        return go()->connect(signal, subscriber);
      }
      // Cannot inline here, AnyObject not fully declared
      qi::FutureSync<SignalLink> connect(unsigned int signal, AnyObject target, unsigned int slot) const;
      inline qi::FutureSync<void> disconnect(SignalLink linkId) const
      {
        return go()->disconnect(linkId);
      }
      template<typename T>
      inline qi::FutureSync<T> property(const std::string& name) const
      {
        return go()->template property<T>(name);
      }
      template<typename T>
      inline qi::FutureSync<void> setProperty(const std::string& name, const T& val) const
      {
        return go()->setProperty(name, val);
      }
      inline qi::FutureSync<AnyValue> property(unsigned int id) const
      {
        return go()->property(id);
      }
      inline qi::FutureSync<void> setProperty(unsigned int id, const AnyValue &val) const
      {
        return go()->setProperty(id, val);
      }
      inline EventLoop* eventLoop() const
      {
        return go()->eventLoop();
      }
      inline bool isStatsEnabled() const
      {
        return go()->isStatsEnabled();
      }
      inline void enableStats(bool enable) const
      {
        return go()->enableStats(enable);
      }
      inline ObjectStatistics stats() const
      {
        return go()->stats();
      }
      inline void clearStats() const
      {
        return go()->clearStats();
      }
      inline bool isTraceEnabled() const
      {
        return go()->isTraceEnabled();
      }
      inline void enableTrace(bool enable)
      {
        return go()->enableTrace(enable);
      }
      inline void forceEventLoop(qi::EventLoop* el)
      {
        return go()->forceEventLoop(el);
      }
      #define genCall(n, ATYPEDECL, ATYPES, ADECL, AUSE, comma)         \
      template<typename R> qi::FutureSync<R> async(                     \
        const std::string& methodName comma                              \
        QI_GEN_ARGSDECLSAMETYPE(n, qi::AutoAnyReference)) const {        \
          return go()->template async<R>(methodName comma AUSE);         \
        }                                                                \
        template<typename R> qi::FutureSync<R> call(                     \
        const std::string& methodName comma                              \
        QI_GEN_ARGSDECLSAMETYPE(n, qi::AutoAnyReference)) const {        \
          return go()->template call<R>(methodName comma AUSE);          \
        }                                                                \
        template<typename R> qi::FutureSync<R> call(                     \
          qi::MetaCallType callType,                                     \
        const std::string& methodName comma                              \
        QI_GEN_ARGSDECLSAMETYPE(n, qi::AutoAnyReference)) const {        \
          return go()->template call<R>(callType, methodName comma AUSE);\
        }
        QI_GEN(genCall)
        #undef genCall
    private:
      inline GenericObject* go() const
      {
        GenericObject* g = static_cast<const O*>(this)->asGenericObject();
        if (!g)
          throw std::runtime_error("This object is null");
        return g;
      }
    };
  }

  template<typename T> class Object
   : public detail::GenericObjectBounce<Object<T> >
  {
  public:
    Object();

    template<typename U> Object(const Object<U>& o);

    // Disable the ctor taking future if T is Empty, as it would conflict with
    // Future cast operator
    typedef typename boost::mpl::if_<typename boost::is_same<T, Empty>::type, Empty, Object<Empty> >::type MaybeAnyObject;
    Object(const qi::Future<MaybeAnyObject>& fobj);
    Object(const qi::FutureSync<MaybeAnyObject>& fobj);

    /// @{

    /** This constructors take ownership of the underlying pointers.
    * If a callback is given, it will be called instead of the default
    * behavior of deleting the stored GenericObject and the underlying T object.
    */

    Object(GenericObject* go);
    Object(T* ptr);
    Object(GenericObject* go, boost::function<void(GenericObject*)> deleter);
    Object(T* ptr, boost::function<void(T*)> deleter);
    /// @}

    /// Shares ref counter with \p other, which much handle the destrutiong of \p go.
    template<typename U> Object(GenericObject* go, boost::shared_ptr<U> other);

    bool operator <(const Object& b) const;
    template<typename U> bool operator !=(const Object<U>& b) const;
    template<typename U> bool operator ==(const Object<U>& b) const;
    operator bool() const;
    operator Object<Empty>() const;

    T& asT();
    const T& asT() const;
    T* operator ->();
    const T* operator->() const;

    T& operator *();
    const T& operator *() const;
    bool unique() const;
    GenericObject* asGenericObject() const;
    void reset();
    unsigned use_count() const { return _obj.use_count();}
    // no-op deletor callback
    static void noDeleteT(T*) {qiLogDebug("qi.object") << "AnyObject noop T deleter";}
    static void noDelete(GenericObject*) {qiLogDebug("qi.object") << "AnyObject noop deleter";}
    // deletor callback that deletes only the GenericObject and not the content
    static void deleteGenericObjectOnly(GenericObject* obj) { qiLogDebug("qi.object") << "AnyObject GO deleter"; delete obj;}
    template<typename U>
    static void deleteGenericObjectOnlyAndKeep(GenericObject* obj, U) { qiLogDebug("qi.object") << "AnyObject GO-keep deleter";delete obj;}
    static void deleteCustomDeleter(GenericObject* obj, boost::function<void(T*)> deleter)
    {
      qiLogDebug("qi.object") << "custom deleter";
      deleter((T*)obj->value);
      delete obj;
    }
  private:
    friend class GenericObject;
    friend class AnyWeakObject;
    template <typename> friend class Object;
    Object(detail::ManagedObjectPtr obj)
    {
      init(obj);
    }
    void init(detail::ManagedObjectPtr obj);
    static void deleteObject(GenericObject* obj)
    {
      qiLogCategory("qi.object");
      qiLogDebug() << "deleteObject " << obj;
      obj->type->destroy(obj->value);
      delete obj;
    }
    /* Do not change this, Object<T> must be binary-equivalent to ManagedObjectPtr.
    */
    detail::ManagedObjectPtr _obj;
  };

  class AnyWeakObject: public boost::weak_ptr<GenericObject>
  {
  public:
    AnyWeakObject() {}
    template<typename T>
    AnyWeakObject(const Object<T>& o)
    : boost::weak_ptr<GenericObject>(o._obj) {}
    AnyObject lock() { return AnyObject(boost::weak_ptr<GenericObject>::lock());}
  };

  template<typename T> inline Object<T>::Object() {}
  template<typename T> template<typename U>inline Object<T>::Object(const Object<U>& o)
  {
    init(o._obj);
  }
  template<typename T> inline Object<T>::Object(GenericObject* go)
  {
    init(detail::ManagedObjectPtr(go, &deleteObject));
  }
  template<typename T> inline Object<T>::Object(GenericObject* go, boost::function<void(GenericObject*)> deleter)
  {
    init(detail::ManagedObjectPtr(go, deleter));
  }
  template<typename T> template<typename U> Object<T>::Object(GenericObject* go, boost::shared_ptr<U> other)
  {
    init(detail::ManagedObjectPtr(other, go));
    // Notify the shared_from_this of GenericObject
    _obj->_internal_accept_owner(&other, go);
  }

  template<typename T> inline Object<T>::Object(T* ptr)
  {
    TypeInterface* type = typeOf<T>();
    if (type->kind() != TypeKind_Object)
    {
      // Try template
      TemplateTypeInterface* t = dynamic_cast<TemplateTypeInterface*>(type);
      if (t)
        type = t->next();
      if (type->kind() != TypeKind_Object)
      {
                std::stringstream err;
        err << "Object<T> can only be used on registered object types. ("
            << type->infoString() << ")(" << type->kind() << ')';

        throw std::runtime_error(err.str());
      }
    }
    ObjectTypeInterface* otype = static_cast<ObjectTypeInterface*>(type);
    _obj = detail::ManagedObjectPtr(new GenericObject(otype, ptr), &deleteObject);
  }
  template<typename T> inline Object<T>::Object(T* ptr, boost::function<void(T*)> deleter)
  {
    TypeInterface* type = typeOf<T>();
    if (type->kind() != TypeKind_Object)
    {
      // Try template
      TemplateTypeInterface* t = dynamic_cast<TemplateTypeInterface*>(type);
      if (t)
        type = t->next();
      if (type->kind() != TypeKind_Object)
      {
        std::stringstream err;
        err << "Object<T> can only be used on registered object types. ("
            << type->infoString() << ")(" << type->kind() << ')';

        throw std::runtime_error(err.str());
      }
    }
    ObjectTypeInterface* otype = static_cast<ObjectTypeInterface*>(type);
    if (deleter)
      _obj = detail::ManagedObjectPtr(new GenericObject(otype, ptr),
        boost::bind(&Object::deleteCustomDeleter, _1, deleter));
    else
      _obj = detail::ManagedObjectPtr(new GenericObject(otype, ptr), &deleteObject);
  }
  template<typename T> inline Object<T>::Object(const qi::Future<MaybeAnyObject>& fobj)
  {
    init(fobj.value()._obj);
  }
  template<typename T> inline Object<T>::Object(const qi::FutureSync<MaybeAnyObject>& fobj)
  {
    init(fobj.value()._obj);
  }

  template<typename T> inline void Object<T>::init(detail::ManagedObjectPtr obj)
  {
    if (!boost::is_same<T, Empty>::value && obj
      && obj->type->info() != typeOf<T>()->info()
      && obj->type->inherits(qi::typeOf<T>())==-1)
    {
      // What is given to us does not implement T.
      // So try if we can summon an intermediate bouncer object:
      // that is a registered Proxy object for type T
      detail::ProxyGeneratorMap& map = detail::proxyGeneratorMap();
      detail::ProxyGeneratorMap::iterator it = map.find(typeOf<T>()->info());
      if (it != map.end())
      {
        AnyReference ref = it->second(AnyObject(obj));
        /* We need to access two interfaces from ref, of type
         * shared_ptr<SomeProxyOfTImpl>:
         * - qi::Proxy : accessed by TypeProxy methods
         * - T : accessed by asT
         *
         */
        _obj = ref.to<detail::ManagedObjectPtr>();
        return;
      }
      throw std::runtime_error(
        std::string("Object<T> constructed from a different AnyObject type ")
        + obj->type->infoString()
        + " "
        + typeOf<T>()->infoString()
        );
    }
    _obj = obj;
  }
  template<typename T> inline bool Object<T>::operator <(const Object& b) const { return _obj < b._obj;}
  template<typename T> template<typename U> bool Object<T>::operator !=(const Object<U>& b) const
  {
    return !(*this ==b);
  }
  template<typename T> template<typename U> bool Object<T>::operator ==(const Object<U>& b) const
  {
    return asGenericObject() == b.asGenericObject();
  }
  template<typename T> Object<T>::operator bool() const   { return _obj && _obj->type;}

  template<typename T> Object<T>::operator Object<Empty>() const { return Object<Empty>(_obj);}
  template<typename T> T& Object<T>::asT()
  {
    return *reinterpret_cast<T*>(_obj->value);
  }
  template<typename T> const T& Object<T>::asT() const
  {
    return *reinterpret_cast<const T*>(_obj->value);
  }
  template<typename T> T* Object<T>::operator ->()
  {
      return &asT();
  }
  template<typename T> const T* Object<T>::operator->() const
  {
    return &asT();
  }
  template<typename T> T& Object<T>::operator *()
  {
    return asT();
  }
  template<typename T> const T& Object<T>::operator *() const
  {
    return asT();
  }
  template<typename T> bool Object<T>::unique() const
  {
    return _obj.unique();
  }
  template<typename T> GenericObject* Object<T>::asGenericObject() const
  {
    return _obj.get();
  }
  template<typename T> void Object<T>::reset()
  {
    _obj.reset();
  }

  class QITYPE_API Proxy
  {
  public:
    Proxy(AnyObject obj) : _obj(obj) {qiLogDebug("qitype.proxy") << "Initializing " << this << " on " << &obj.asT();}
    Proxy() {}
    ~Proxy() { qiLogDebug("qitype.proxy") << "Finalizing on " << &_obj.asT();}
    Object<Empty> asObject() const;
  protected:
    Object<Empty> _obj;
  };



  namespace detail
  {

    template<typename T> void hold(T data) {}

    template<typename T> void setPromise(qi::Promise<T>& promise, AnyValue& v)
    {
      try
      {
        qiLogDebug("qi.adapter") << "converting value";
        T val = v.to<T>();
        qiLogDebug("qi.adapter") << "setting promise";
        promise.setValue(val);
        qiLogDebug("qi.adapter") << "done";
      }
      catch(const std::exception& e)
      {
        qiLogError("qi.adapter") << e.what();
        promise.setError(e.what());
      }
    }
    template<> inline void setPromise(qi::Promise<void>& promise, AnyValue&)
    {
      promise.setValue(0);
    }
    template <typename T>
    void futureAdapterGeneric(AnyReference val, qi::Promise<T> promise)
    {
      qiLogDebug("qi.adapter") << "futureAdapter trigger";
      TemplateTypeInterface* ft1 = QI_TEMPLATE_TYPE_GET(val.type(), Future);
      TemplateTypeInterface* ft2 = QI_TEMPLATE_TYPE_GET(val.type(), FutureSync);
      TemplateTypeInterface* futureType = ft1 ? ft1 : ft2;
      ObjectTypeInterface* onext = dynamic_cast<ObjectTypeInterface*>(futureType->next());
      GenericObject gfut(onext, val.rawValue());
      // Need a live shared_ptr for shared_from_this() to work.
      boost::shared_ptr<GenericObject> ao(&gfut, hold<GenericObject*>);
      if (gfut.call<bool>(MetaCallType_Direct, "hasError", 0))
      {
        qiLogDebug("qi.adapter") << "futureAdapter: future in error";
        std::string s = gfut.call<std::string>("error", 0);
        qiLogDebug("qi.adapter") << "futureAdapter: got error: " << s;
        promise.setError(s);
        return;
      }
      qiLogDebug("qi.adapter") << "futureAdapter: future has value";
      AnyValue v = gfut.call<AnyValue>(MetaCallType_Direct, "value", 0);
      // For a Future<void>, value() gave us a void*
      if (futureType->templateArgument()->kind() == TypeKind_Void)
        v = AnyValue(qi::typeOf<void>());
      qiLogDebug("qi.adapter") << v.type()->infoString();
      setPromise(promise, v);
      qiLogDebug("qi.adapter") << "Promise set";
      val.destroy();
    }

    // futureAdapter helper that detects and handles value of kind future
    // return true if value was a future and was handled
    template <typename T>
    inline bool handleFuture(AnyReference val, Promise<T> promise)
    {
      TemplateTypeInterface* ft1 = QI_TEMPLATE_TYPE_GET(val.type(), Future);
      TemplateTypeInterface* ft2 = QI_TEMPLATE_TYPE_GET(val.type(), FutureSync);
      TemplateTypeInterface* futureType = ft1 ? ft1 : ft2;
      qiLogDebug("qi.object") << "isFuture " << val.type()->infoString() << ' ' << !!ft1 << ' ' << !!ft2;
      if (!futureType)
        return false;

      TypeInterface* next = futureType->next();
      ObjectTypeInterface* onext = dynamic_cast<ObjectTypeInterface*>(next);
      GenericObject gfut(onext, val.rawValue());
      // Need a live shared_ptr for shared_from_this() to work.
      boost::shared_ptr<GenericObject> ao(&gfut, &hold<GenericObject*>);
      boost::function<void()> cb = boost::bind(futureAdapterGeneric<T>, val, promise);
      // Careful, gfut will die at the end of this block, but it is
      // stored in call data. So call must finish before we exit this block,
      // and thus must be synchronous.
      qi::Future<void> waitResult = gfut.call<void>(MetaCallType_Direct, "_connect", cb);
      waitResult.wait();
      qiLogDebug("qi.adapter") << "future connected " << !waitResult.hasError();
      if (waitResult.hasError())
        qiLogWarning("qi.object") << waitResult.error();
      return true;
    }

    template <typename T>
    inline void futureAdapter(qi::Future<qi::AnyReference> metaFut, qi::Promise<T> promise)
    {
      qiLogDebug("qi.object") << "futureAdapter " << qi::typeOf<T>()->infoString()<< ' ' << metaFut.hasError();
      //error handling
      if (metaFut.hasError()) {
        promise.setError(metaFut.error());
        return;
      }

      AnyReference val =  metaFut.value();
      if (handleFuture(val, promise))
        return;

      static TypeInterface* targetType = typeOf<T>();
      try
      {
        std::pair<AnyReference, bool> conv = val.convert(targetType);
        if (!conv.first.type())
          promise.setError(std::string("Unable to convert call result to target type: from ")
            + val.signature(true).toPrettySignature() + " to " + targetType->signature().toPrettySignature() );
        else
        {
          promise.setValue(*conv.first.ptr<T>(false));
        }
        if (conv.second)
          conv.first.destroy();
      }
      catch(const std::exception& e)
      {
        promise.setError(std::string("Return argument conversion error: ") + e.what());
      }
      val.destroy();
    }

    template <>
    inline void futureAdapter<void>(qi::Future<qi::AnyReference> metaFut, qi::Promise<void> promise)
    {
      //error handling
      if (metaFut.hasError()) {
        promise.setError(metaFut.error());
        return;
      }
      AnyReference val =  metaFut.value();
      if (!handleFuture(val, promise))
        promise.setValue(0);
    }

    template <typename T>
    inline void futureAdapterVal(qi::Future<qi::AnyValue> metaFut, qi::Promise<T> promise)
    {
      //error handling
      if (metaFut.hasError()) {
        promise.setError(metaFut.error());
        return;
      }
      const AnyValue& val =  metaFut.value();
      try
      {
        promise.setValue(val.to<T>());
      }
      catch (const std::exception& e)
      {
        promise.setError(std::string("Return argument conversion error: ") + e.what());
      }
    }

    template<>
    inline void futureAdapterVal(qi::Future<qi::AnyValue> metaFut, qi::Promise<AnyValue> promise)
    {
      if (metaFut.hasError())
        promise.setError(metaFut.error());
      else
        promise.setValue(metaFut.value());
    }
  }


  /* Generate qi::FutureSync<R> GenericObject::call(methodname, args...)
   * for all argument counts
   * The function packs arguments in a vector<AnyReference>, computes the
   * signature and bounce those to metaCall.
   */
  #define pushi(z, n,_) params.push_back(p ## n);
#define genCall(n, ATYPEDECL, ATYPES, ADECL, AUSE, comma)                  \
  template<typename R> qi::FutureSync<R> GenericObject::call(              \
      const std::string& methodName       comma                            \
      QI_GEN_ARGSDECLSAMETYPE(n, qi::AutoAnyReference))                    \
  {                                                                        \
     if (!value || !type) {                                                \
      return makeFutureError<R>("Invalid GenericObject");                  \
     }                                                                     \
     std::vector<qi::AnyReference> params;                                 \
     params.reserve(n);                                                    \
     BOOST_PP_REPEAT(n, pushi, _)                                          \
     std::string sigret;                                                   \
     qi::Promise<R> res(qi::FutureCallbackType_Sync);                      \
     qi::Future<AnyReference> fmeta = metaCall(methodName, params);        \
     fmeta.connect(boost::bind<void>(&detail::futureAdapter<R>, _1, res)); \
     return res.future();                                                  \
  }

  QI_GEN(genCall)
  #undef genCall
  #define genCall(n, ATYPEDECL, ATYPES, ADECL, AUSE, comma)                                \
  template<typename R> qi::FutureSync<R> GenericObject::async(                             \
      const std::string& methodName       comma                                            \
      QI_GEN_ARGSDECLSAMETYPE(n, qi::AutoAnyReference))                                    \
  {                                                                                        \
     if (!value || !type) {                                                                \
      return makeFutureError<R>("Invalid GenericObject");                                  \
     }                                                                                     \
     std::vector<qi::AnyReference> params;                                                 \
     params.reserve(n);                                                                    \
     BOOST_PP_REPEAT(n, pushi, _)                                                          \
     std::string sigret;                                                                   \
     qi::Promise<R> res(qi::FutureCallbackType_Sync);                                      \
     qi::Future<AnyReference> fmeta = metaCall(methodName, params, MetaCallType_Queued);   \
     fmeta.connect(boost::bind<void>(&detail::futureAdapter<R>, _1, res));                 \
     return res.future();                                                                  \
  }

  QI_GEN(genCall)
  #undef genCall
  #define genCall(n, ATYPEDECL, ATYPES, ADECL, AUSE, comma)               \
  template<typename R> qi::FutureSync<R> GenericObject::call(             \
      MetaCallType callType,                                              \
      const std::string& methodName       comma                           \
      QI_GEN_ARGSDECLSAMETYPE(n, qi::AutoAnyReference))             \
  {                                                                        \
     if (!value || !type) {                                                \
      return makeFutureError<R>("Invalid GenericObject");                  \
     }                                                                     \
     std::vector<qi::AnyReference> params;                              \
     params.reserve(n);                                                    \
     BOOST_PP_REPEAT(n, pushi, _)                                          \
     std::string sigret;                                                   \
     qi::Promise<R> res(qi::FutureCallbackType_Sync);                       \
     qi::Future<AnyReference> fmeta = metaCall(methodName, params, callType);   \
     fmeta.connect(boost::bind<void>(&detail::futureAdapter<R>, _1, res));  \
     return res.future();                                                  \
  }

  QI_GEN(genCall)
  #undef genCall

  #define genCall(n, ATYPEDECL, ATYPES, ADECL, AUSE, comma)         \
  template<typename R,typename T> qi::FutureSync<R> async(   \
      T* instance,                                                 \
      const std::string& methodName comma                         \
      QI_GEN_ARGSDECLSAMETYPE(n, qi::AutoAnyReference)) \
  {                                                              \
    AnyObject obj = AnyReference::from(instance).toObject();       \
    qi::Future<R> res = obj.template call<R>(MetaCallType_Queued, methodName comma AUSE);  \
    res.connect(boost::bind(&detail::hold<AnyObject>, obj));   \
    return res;                                                 \
  }
  QI_GEN(genCall)
  #undef genCall

  #define genCall(n, ATYPEDECL, ATYPES, ADECL, AUSE, comma)         \
  template<typename R,typename T> qi::FutureSync<R> async(   \
      boost::shared_ptr<T> instance,                             \
      const std::string& methodName comma                         \
      QI_GEN_ARGSDECLSAMETYPE(n, qi::AutoAnyReference)) \
  {                                                              \
    AnyObject obj = AnyReference::from(instance).toObject();        \
    qi::Future<R> res = obj.call<R>(MetaCallType_Queued, methodName comma AUSE);  \
    res.connect(boost::bind(&detail::hold<AnyObject>, obj));   \
    return res;                                                 \
  }
  QI_GEN(genCall)
  #undef genCall
  #undef pushi

  #define genCall(n, ATYPEDECL, ATYPES, ADECL, AUSE, comma)         \
  template<typename R,typename T> qi::FutureSync<R> async(   \
      Object<T> instance,                             \
      const std::string& methodName comma                         \
      QI_GEN_ARGSDECLSAMETYPE(n, qi::AutoAnyReference)) \
  {                                                              \
    AnyObject obj = instance;                                    \
    qi::Future<R> res =  obj.call<R>(MetaCallType_Queued, methodName comma AUSE);  \
    res.connect(boost::bind(&detail::hold<AnyObject>, obj));   \
    return res;                                                 \
  }
  QI_GEN(genCall)
  #undef genCall
  #undef pushi

  template<typename T>
  qi::FutureSync<T> GenericObject::property(const std::string& name)
  {
    int pid = metaObject().propertyId(name);
    if (pid < 0)
      return makeFutureError<T>("Property not found");
    qi::Future<AnyValue> f = type->property(value, pid);
    qi::Promise<T> p(qi::FutureCallbackType_Sync);
    f.connect(boost::bind(&detail::futureAdapterVal<T>,_1, p));
    return p.future();
  }

  template<typename T>
  qi::FutureSync<void> GenericObject::setProperty(const std::string& name, const T& val)
  {
    int pid = metaObject().propertyId(name);
    if (pid < 0)
      return makeFutureError<void>("Property not found");
    return type->setProperty(value, pid, AnyValue::from(val));
  }

  /* An AnyObject is actually of a Dynamic type: The underlying TypeInterface*
   * is not allways the same.
   * Override backend shared_ptr<GenericObject>
  */
  template<> class QITYPE_API TypeImpl<boost::shared_ptr<GenericObject> >: public DynamicTypeInterface
  {
  public:
    virtual AnyReference get(void* storage)
    {
      detail::ManagedObjectPtr* val = (detail::ManagedObjectPtr*)ptrFromStorage(&storage);
      AnyReference result;
      if (!*val)
      {
        return AnyReference();
      }
      return AnyReference((*val)->type, (*val)->value);
    }

    virtual void set(void** storage, AnyReference source)
    {
      qiLogCategory("qitype.object");
      detail::ManagedObjectPtr* val = (detail::ManagedObjectPtr*)ptrFromStorage(storage);
      TemplateTypeInterface* templ = dynamic_cast<TemplateTypeInterface*>(source.type());
      if (templ)
        source = AnyReference(templ->next(), source.rawValue());
      if (source.type()->info() == info())
      { // source is objectptr
        detail::ManagedObjectPtr* src = source.ptr<detail::ManagedObjectPtr>(false);
        if (!*src)
          qiLogWarning() << "NULL Object";
        *val = *src;
      }
      else if (source.kind() == TypeKind_Dynamic)
      { // try to dereference dynamic type in case it contains an object
        set(storage, source.content());
      }
      else if (source.kind() == TypeKind_Object)
      { // wrap object in objectptr: we do not keep it alive,
        // but source type offers no tracking capability
        detail::ManagedObjectPtr op(new GenericObject(static_cast<ObjectTypeInterface*>(source.type()), source.rawValue()));
        *val = op;
      }
      else if (source.kind() == TypeKind_Pointer)
      {
        PointerTypeInterface* ptype = static_cast<PointerTypeInterface*>(source.type());
        // FIXME: find a way!
        if (ptype->pointerKind() == PointerTypeInterface::Shared)
          qiLogInfo() << "Object will *not* track original shared pointer";
        set(storage, *source);
      }
      else
        throw std::runtime_error((std::string)"Cannot assign non-object " + source.type()->infoString() + " to Object");

    }
    typedef DefaultTypeImplMethods<detail::ManagedObjectPtr> Methods;
    _QI_BOUNCE_TYPE_METHODS(Methods);
  };

  // Pretend that Object<T> is exactly shared_ptr<GenericObject>
  // Will be overriden for proxy objects below, for which this statement is false
  // (because a proxy has members beside the shared_ptr: signals and events
  template<typename T> class QITYPE_API TypeImpl<Object<T> >: public TypeImpl<boost::shared_ptr<GenericObject> >
  {
  };


  namespace detail
  {
    /* Factory helper functions
    */

    // create a T, wrap in a AnyObject
    template<typename T> Object<T> constructObject()
    {
      return Object<T>(new T());
    }
  }


  /* A proxy instance can have members: signals and properties, inherited from interface.
  * So it need a type of its own, we cannot pretend it's a AnyObject.
  */
  class TypeProxy: public ObjectTypeInterface
  {
  public:
    /* We need a per-instance offset from effective type to Proxy.
     * Avoid code explosion by putting it per-instance
    */
    typedef boost::function<Proxy*(void*)> ToProxy;
    TypeProxy(ToProxy  toProxy)
    : toProxy(toProxy)
    {
    }
    virtual const MetaObject& metaObject(void* instance)
    {
      Proxy* ptr = toProxy(instance);
      return ptr->asObject().metaObject();
    }
    virtual qi::Future<AnyReference> metaCall(void* instance, AnyObject context, unsigned int method, const GenericFunctionParameters& params, MetaCallType callType = MetaCallType_Auto)
    {
      Proxy* ptr = toProxy(instance);
      return ptr->asObject().metaCall(method, params, callType);
    }
    virtual void metaPost(void* instance, AnyObject context, unsigned int signal, const GenericFunctionParameters& params)
    {
      Proxy* ptr = toProxy(instance);
      ptr->asObject().metaPost(signal, params);
    }
    virtual qi::Future<SignalLink> connect(void* instance, AnyObject context, unsigned int event, const SignalSubscriber& subscriber)
    {
      Proxy* ptr = toProxy(instance);
      return ptr->asObject().connect(event, subscriber);
    }
    virtual qi::Future<void> disconnect(void* instance, AnyObject context, SignalLink linkId)
    {
       Proxy* ptr = toProxy(instance);
       return ptr->asObject().disconnect(linkId);
    }
    virtual const std::vector<std::pair<TypeInterface*, int> >& parentTypes()
    {
      static std::vector<std::pair<TypeInterface*, int> > empty;
      return empty;
    }
    virtual qi::Future<AnyValue> property(void* instance, unsigned int id)
    {
      Proxy* ptr = toProxy(instance);
      GenericObject* obj = ptr->asObject().asGenericObject();
      return obj->type->property(obj->value, id);
    }
    virtual qi::Future<void> setProperty(void* instance, unsigned int id, AnyValue value)
    {
      Proxy* ptr = toProxy(instance);
      GenericObject* obj = ptr->asObject().asGenericObject();
      return obj->type->setProperty(obj->value, id, value);
    }
    typedef DefaultTypeImplMethods<Proxy> Methods;
    _QI_BOUNCE_TYPE_METHODS(Methods);
    ToProxy toProxy;
  };

    namespace detail
  {
    // FIXME: inline that in QI_REGISTER_PROXY_INTERFACE maybe
    template<typename ProxyImpl> Proxy* static_proxy_cast(void* storage)
    {
      return static_cast<Proxy*>((ProxyImpl*)storage);
    }
    template<typename ProxyImpl>
    TypeProxy* makeProxyInterface()
    {
      static TypeProxy * result = 0;
      if (!result)
        result = new TypeProxy(&static_proxy_cast<ProxyImpl>);
      return result;
    }

    template<typename ProxyImpl>
    AnyReference makeProxy(AnyObject ptr)
    {
      boost::shared_ptr<ProxyImpl> sp(new ProxyImpl(ptr));
      return AnyReference::from(sp).clone();
    }
  }
  template<typename Proxy, typename Interface>
  bool registerProxyInterface()
  {
    qiLogVerbose("qitype.type") << "ProxyInterface registration " << typeOf<Interface>()->infoString();
    // Runtime-register TypeInterface for Proxy, using ProxyInterface with
    // proper static_cast (from Proxy template to qi::Proxy) helper.
    registerType(typeid(Proxy), detail::makeProxyInterface<Proxy>());
    detail::ProxyGeneratorMap& map = detail::proxyGeneratorMap();
    map[typeOf<Interface>()->info()] = &detail::makeProxy<Proxy>;
    return true;
  }
  template<typename Proxy>
  bool registerProxy()
  {
    detail::ProxyGeneratorMap& map = detail::proxyGeneratorMap();
    map[typeOf<Proxy>()->info()] = &detail::makeProxy<Proxy>;
    return true;
  }

  namespace detail
  {
    // in genericobjectbuilder.hxx
    template<typename T> AnyObject makeObject(const std::string& fname, T func);

    // Create a factory function for an object with one method functionName bouncing to func
    template<typename T> boost::function<AnyObject(const std::string&)>
    makeObjectFactory(const std::string functionName, T func)
    {
      return ( boost::function<AnyObject(const std::string&)>)
        boost::bind(&makeObject<T>, functionName, func);
    }

    template<typename O>
    inline qi::FutureSync<SignalLink> GenericObjectBounce<O>::connect(unsigned int signal, AnyObject target, unsigned int slot) const
    {
      return go()->connect(signal, target, slot);
    }
  }

  inline AnyObject Proxy::asObject() const
  {
    qiLogDebug("qitype.proxy") << "asObject " << this << ' ' << &_obj.asT();
    return AnyObject(_obj);
  }
}


QI_TYPE_STRUCT_AGREGATE_CONSTRUCTOR(qi::MinMaxSum,
  ("minValue",       minValue),
  ("maxValue",       maxValue),
  ("cumulatedValue", cumulatedValue));
QI_TYPE_STRUCT_AGREGATE_CONSTRUCTOR(qi::MethodStatistics,
  ("count",  count),
  ("wall",   wall),
  ("user",   user),
  ("system", system));
QI_TYPE_STRUCT_AGREGATE_CONSTRUCTOR(qi::EventTrace,
  ("id",           id),
  ("kind",         kind),
  ("slotId",       slotId),
  ("arguments",    arguments),
  ("timestamp",    timestamp),
  ("userUsTime",   userUsTime),
  ("systemUsTime", systemUsTime),
  ("callerContext", callerContext),
  ("calleeContext", calleeContext));
QI_TYPE_STRUCT(qi::os::timeval, tv_sec, tv_usec);

#endif  // _QITYPE_DETAILS_GENERICOBJECT_HXX_
