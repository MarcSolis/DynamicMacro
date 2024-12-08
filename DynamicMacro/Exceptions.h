#pragma once
#include <exception>
#include <string>

namespace Exceptions 
{
	class Exception : public std::exception
	{
#if !defined(_MSC_VER)
	public:
		const char* what() const noexcept override { return reason.c_str(); }
#endif

	protected:
		using RootException = std::exception;

		Exception() noexcept = default;

#if defined(_MSC_VER)
		explicit Exception(const char* message) noexcept : RootException{ message } {}
		explicit Exception(const std::string& message) noexcept : RootException{ message.c_str() } {}
#else
		explicit Exception(const char* message) : reason{ message } {}
		explicit Exception(const std::string& message) : reason{ message.c_str() } {}

		std::string reason;
#endif
	};

	template <class ExceptionT, class BaseExceptionT = Exception> class
		ExceptionClass final : public BaseExceptionT
	{
	public:
		using BaseException = BaseExceptionT;

		ExceptionClass() noexcept = default;
		explicit ExceptionClass(const char* message) noexcept : BaseException{ message } {}
		explicit ExceptionClass(const std::string& message) noexcept :BaseException{ message } {}
	};

	class ExceptionInvalidInputDesktop {};
	class ExceptionInvalidDesktopThread {};
	class ExceptionInvalidInputHook {};
	class ExceptionSaveToFileError {};
	class ExceptionInvalidExecutablePath {};

	using InvalidInputDesktop = ExceptionClass<ExceptionInvalidInputDesktop>;
	using InvalidDesktopThread = ExceptionClass<ExceptionInvalidDesktopThread>;
	using InvalidInputHook = ExceptionClass<ExceptionInvalidInputHook>;
	using ErrorSavingFile = ExceptionClass<ExceptionSaveToFileError>;
	using InvalidExecutablePath = ExceptionClass<ExceptionInvalidExecutablePath>;
}
