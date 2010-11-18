
/*
** AUTOGENERATED CODE, DO NOT EDIT
**
** Author(s):
**  - Cedric GESTES <gestes@aldebaran-robotics.com>
**
** Copyright (C) 2010 Aldebaran Robotics
*/

#ifndef   __QI_FUNCTORS_CALLFUNCTOR_HPP_IN__
#define   __QI_FUNCTORS_CALLFUNCTOR_HPP_IN__

#include <qi/functors/functor.hpp>

namespace qi
{

  template <typename R>
  R callFunctor(Functor *f) {
    qi::serialization::SerializedData args;
    qi::serialization::SerializedData ret;

    f->call(args, ret);
    R r;
    ret.read<R>(r);
    return r;
  }

  void callVoidFunctor(Functor *f) {
    qi::serialization::SerializedData args;
    qi::serialization::SerializedData ret;

    f->call(args, ret);
  }


  template <typename R, typename P0>
  R callFunctor(Functor *f, const P0 &p0) {
    qi::serialization::SerializedData args;
    qi::serialization::SerializedData ret;

    args.write<P0>(p0);
    f->call(args, ret);
    R r;
    ret.read<R>(r);
    return r;
  }

  template <typename P0>
  void callVoidFunctor(Functor *f, const P0 &p0) {
    qi::serialization::SerializedData args;
    qi::serialization::SerializedData ret;

    args.write<P0>(p0);
    f->call(args, ret);
  }


  template <typename R, typename P0, typename P1>
  R callFunctor(Functor *f, const P0 &p0, const P1 &p1) {
    qi::serialization::SerializedData args;
    qi::serialization::SerializedData ret;

    args.write<P0>(p0);
    args.write<P1>(p1);
    f->call(args, ret);
    R r;
    ret.read<R>(r);
    return r;
  }

  template <typename P0, typename P1>
  void callVoidFunctor(Functor *f, const P0 &p0, const P1 &p1) {
    qi::serialization::SerializedData args;
    qi::serialization::SerializedData ret;

    args.write<P0>(p0);
    args.write<P1>(p1);
    f->call(args, ret);
  }


  template <typename R, typename P0, typename P1, typename P2>
  R callFunctor(Functor *f, const P0 &p0, const P1 &p1, const P2 &p2) {
    qi::serialization::SerializedData args;
    qi::serialization::SerializedData ret;

    args.write<P0>(p0);
    args.write<P1>(p1);
    args.write<P2>(p2);
    f->call(args, ret);
    R r;
    ret.read<R>(r);
    return r;
  }

  template <typename P0, typename P1, typename P2>
  void callVoidFunctor(Functor *f, const P0 &p0, const P1 &p1, const P2 &p2) {
    qi::serialization::SerializedData args;
    qi::serialization::SerializedData ret;

    args.write<P0>(p0);
    args.write<P1>(p1);
    args.write<P2>(p2);
    f->call(args, ret);
  }


  template <typename R, typename P0, typename P1, typename P2, typename P3>
  R callFunctor(Functor *f, const P0 &p0, const P1 &p1, const P2 &p2, const P3 &p3) {
    qi::serialization::SerializedData args;
    qi::serialization::SerializedData ret;

    args.write<P0>(p0);
    args.write<P1>(p1);
    args.write<P2>(p2);
    args.write<P3>(p3);
    f->call(args, ret);
    R r;
    ret.read<R>(r);
    return r;
  }

  template <typename P0, typename P1, typename P2, typename P3>
  void callVoidFunctor(Functor *f, const P0 &p0, const P1 &p1, const P2 &p2, const P3 &p3) {
    qi::serialization::SerializedData args;
    qi::serialization::SerializedData ret;

    args.write<P0>(p0);
    args.write<P1>(p1);
    args.write<P2>(p2);
    args.write<P3>(p3);
    f->call(args, ret);
  }


  template <typename R, typename P0, typename P1, typename P2, typename P3, typename P4>
  R callFunctor(Functor *f, const P0 &p0, const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4) {
    qi::serialization::SerializedData args;
    qi::serialization::SerializedData ret;

    args.write<P0>(p0);
    args.write<P1>(p1);
    args.write<P2>(p2);
    args.write<P3>(p3);
    args.write<P4>(p4);
    f->call(args, ret);
    R r;
    ret.read<R>(r);
    return r;
  }

  template <typename P0, typename P1, typename P2, typename P3, typename P4>
  void callVoidFunctor(Functor *f, const P0 &p0, const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4) {
    qi::serialization::SerializedData args;
    qi::serialization::SerializedData ret;

    args.write<P0>(p0);
    args.write<P1>(p1);
    args.write<P2>(p2);
    args.write<P3>(p3);
    args.write<P4>(p4);
    f->call(args, ret);
  }


  template <typename R, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5>
  R callFunctor(Functor *f, const P0 &p0, const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5) {
    qi::serialization::SerializedData args;
    qi::serialization::SerializedData ret;

    args.write<P0>(p0);
    args.write<P1>(p1);
    args.write<P2>(p2);
    args.write<P3>(p3);
    args.write<P4>(p4);
    args.write<P5>(p5);
    f->call(args, ret);
    R r;
    ret.read<R>(r);
    return r;
  }

  template <typename P0, typename P1, typename P2, typename P3, typename P4, typename P5>
  void callVoidFunctor(Functor *f, const P0 &p0, const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5) {
    qi::serialization::SerializedData args;
    qi::serialization::SerializedData ret;

    args.write<P0>(p0);
    args.write<P1>(p1);
    args.write<P2>(p2);
    args.write<P3>(p3);
    args.write<P4>(p4);
    args.write<P5>(p5);
    f->call(args, ret);
  }

}
#endif // __QI_FUNCTORS_CALLFUNCTOR_HPP_IN__
