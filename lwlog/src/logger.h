#pragma once

#include "policy/log_policy.h"
#include "policy/sink_storage_policy.h"
#include "interface/logger_interface.h"

namespace lwlog
{
	template<typename LogPolicy, template<typename...> typename StoragePolicy,
		typename ThreadingPolicy, template<typename> typename... Sinks>
	class logger : public interface::logger
	{
	private:
		using Storage = typename StoragePolicy<Sinks<ThreadingPolicy>...>::storage_t;

	public:
		template<typename... SinkParams>
		logger(std::string_view name, SinkParams&&... params);
		template<typename Iterator, typename... SinkParams>
		logger(std::string_view name, Iterator begin, Iterator end, SinkParams&&... params);
		template<typename... SinkParams>
		logger(std::string_view name, sink_list sink_list, SinkParams&&... params);
		template<typename... SinkParams>
		logger(std::string_view name, sink_ptr sink, SinkParams&&... params);

	public:
		void add_sink(sink_ptr sink);
		void remove_sink(sink_ptr sink);
		void sink_logs() override;

		void set_pattern(std::string_view pattern) override;
		void add_attribute(std::string_view flag, details::attrib_value value) override;
		void add_attribute(std::string_view flag, details::attrib_value value, details::attrib_callback_t fn) override;
		void set_level_filter(level t_level) override;

		std::string_view name() const override;
		Storage& sinks();

	private:
		void log(const details::log_message& log_msg, level t_level, details::format_args_list args) override;
		void info_impl(const details::log_message& log_msg, details::format_args_list args) override;
		void warning_impl(const details::log_message& log_msg, details::format_args_list args) override;
		void error_impl(const details::log_message& log_msg, details::format_args_list args) override;
		void critical_impl(const details::log_message& log_msg, details::format_args_list args) override;
		void debug_impl(const details::log_message& log_msg, details::format_args_list args) override;

	private:
		std::string_view m_name;
		Storage m_sink_storage;
	};
}

#include "logger_impl.h"
