#pragma once

#include <memory>

#include "core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace BlackThorn {

	class BLACKTHORN_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define BT_CORE_TRACE(...) ::BlackThorn::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define BT_CORE_INFO(...)  ::BlackThorn::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BT_CORE_WARN(...)  ::BlackThorn::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BT_CORE_ERROR(...) ::BlackThorn::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BT_CORE_CRITICAL(...) ::BlackThorn::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define BT_TRACE(...)      ::BlackThorn::Log::GetClientLogger()->trace(__VA_ARGS__)
#define BT_INFO(...)       ::BlackThorn::Log::GetClientLogger()->info(__VA_ARGS__)
#define BT_WARN(...)       ::BlackThorn::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BT_ERROR(...)      ::BlackThorn::Log::GetClientLogger()->error(__VA_ARGS__)
#define BT_CRITICAL(...)      ::BlackThorn::Log::GetClientLogger()->critical(__VA_ARGS__)