#pragma once
#include <exception>
#include <string>
#include <string_view>
#include <source_location>

class UndoRedoException : public std::exception
{
public:
	enum class ExceptionType
	{
#define UNDO_REDO_EXCEPT_DEF(enumerate, message) enumerate,
#include "UndoRedoExceptionDefines.h"
#undef UNDO_REDO_EXCEPT_DEF
	};

private:
	std::source_location m_location;
	ExceptionType m_type;
	std::string m_message;

public:
	UndoRedoException(const std::source_location& a_loc, const ExceptionType a_type, const std::string_view& a_extra = "");
	virtual ~UndoRedoException() = default;
	const ExceptionType type()const noexcept { return m_type; }
	const char* message()const { return m_message.c_str(); }
};

#define UNDO_REDO_TROW(enumerate) \
	throw UndoRedoException(std::source_location::current(), enumerate);


#define UNDO_REDO_TROW_EXTRA(enumerate, extra) \
	throw UndoRedoException(std::source_location::current(), enumerate, #extra);