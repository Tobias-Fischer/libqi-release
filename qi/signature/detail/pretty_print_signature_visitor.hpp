#pragma once
/*
*  Author(s):
*  - Cedric Gestes <gestes@aldebaran-robotics.com>
*  - Chris  Kilner <ckilner@aldebaran-robotics.com>
*
*  Copyright (C) 2010 Aldebaran Robotics
*/


#ifndef _QI_SIGNATURE_DETAIL_PRETTY_PRINT_SIGNATURE_VISITOR_HPP_
#define _QI_SIGNATURE_DETAIL_PRETTY_PRINT_SIGNATURE_VISITOR_HPP_

# include<string>

namespace qi {

  namespace detail {

    class PrettyPrintSignatureVisitor {
    public:
      PrettyPrintSignatureVisitor(const char *signature, std::string &result);

      void visit(const char *sep = " ");

    protected:
      void visitSingle();
      void visitSimple();
      void visitList();
      void visitMap();
      void visitProtobuf();
      void visitFunctionArguments();
      void visitFunction();

      std::string &_result;
      const char  *_current;
      const char  *_signature;
      std::string _method;
    };

  };

};


#endif  // _QI_SIGNATURE_DETAIL_PRETTY_PRINT_SIGNATURE_VISITOR_HPP_
