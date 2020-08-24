#ifndef LFORTRAN_EXCEPTION_H
#define LFORTRAN_EXCEPTION_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LFORTRAN_NO_EXCEPTION = 0,
    LFORTRAN_RUNTIME_ERROR = 1,
    LFORTRAN_TOKENIZER_ERROR = 2,
    LFORTRAN_PARSER_ERROR = 3,
    LFORTRAN_SEMANTIC_ERROR = 4,
    LFORTRAN_CODEGEN_ERROR = 5,
} lfortran_exceptions_t;

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

#include <exception>
#include <string>
#include <lfortran/parser/location.h>
#include <lfortran/config.h>
#include <lfortran/stacktrace.h>

namespace LFortran
{

class LFortranException : public std::exception
{
    std::string m_msg;
    lfortran_exceptions_t ec;
    std::string m_stacktrace;

public:
    LFortranException(const std::string &msg, lfortran_exceptions_t error,
        int stacktrace_dept)
        : m_msg(msg), ec(error)
    {
#if defined(HAVE_LFORTRAN_STACKTRACE)
        m_stacktrace = LFortran::get_stacktrace(stacktrace_dept);
#endif
    }
    LFortranException(const std::string &msg)
        : LFortranException(msg, LFORTRAN_RUNTIME_ERROR, 2)
    {
    }
    const char *what() const throw()
    {
        return m_msg.c_str();
    }
    std::string msg() const
    {
        return m_msg;
    }
    std::string stacktrace() const
    {
        return m_stacktrace;
    }
    lfortran_exceptions_t error_code()
    {
        return ec;
    }
};

class TokenizerError : public LFortranException
{
public:
    Location loc;
    std::string token;
public:
    TokenizerError(const std::string &msg, const Location &loc,
            const std::string &token)
        : LFortranException(msg, LFORTRAN_TOKENIZER_ERROR, 2), loc{loc},
            token{token}
    {
    }
};

class ParserError : public LFortranException
{
public:
    Location loc;
    int token;
public:
    ParserError(const std::string &msg, const Location &loc, const int token)
        : LFortranException(msg, LFORTRAN_PARSER_ERROR, 2), loc{loc}, token{token}
    {
    }
};

class SemanticError : public LFortranException
{
public:
    Location loc;
public:
    SemanticError(const std::string &msg, const Location &loc)
        : LFortranException(msg, LFORTRAN_SEMANTIC_ERROR, 2), loc{loc}
    {
    }
};

class CodeGenError : public LFortranException
{
public:
    CodeGenError(const std::string &msg)
        : LFortranException(msg, LFORTRAN_CODEGEN_ERROR, 2)
    {
    }
};

} // Namespace LFortran

#endif // __cplusplus
#endif // LFORTRAN_EXCEPTION_H
