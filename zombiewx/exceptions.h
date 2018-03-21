#pragma once
#include <exception>

// Macro because noexcept is NYI in VS 2013
#ifdef _WIN32
#define NOEXCEPT
#else
#define NOEXCEPT noexcept
#endif

class InvalidOp : public std::exception
{
	const char* what() const NOEXCEPT override { return "Invalid OP code or parameters."; }
};

class AccessViolation : public std::exception
{
	const char* what() const NOEXCEPT override { return "Machine tried to access outside available memory."; }
};

class InvalidParam : public std::exception
{
    const char* what() const NOEXCEPT override { return "Invalid Parameters."; }
};

class ZombieViolation : public std::exception
{
    const char* what() const NOEXCEPT override { return "Zombie tried perform an op which it is not allowed to do."; }
};

class InvalidMemory : public std::exception
{
    const char* what() const NOEXCEPT override { return "Invalid Memory Setting."; }
};