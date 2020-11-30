#pragma once

#include <string>

#include "interface/sink_interface.h"
#include "policy/sink_color_policy.h"
#include "policy/threading_policy.h"
#include "fwd.h"

namespace lwlog::sinks
{
	template<typename ColorPolicy, typename ThreadingPolicy>
	class sink : public interface::sink
	{
	private:
		using Mutex = typename ThreadingPolicy::mutex_t;
		using Lock = typename ThreadingPolicy::lock;

	public:
		sink();
		virtual ~sink() = default;

	public:
		void set_pattern(std::string_view pattern) override;
		void add_pattern_attribute(primitives::attribute_t attribute) override;
		void set_level_filter(primitives::level_list levels) override;

		bool should_sink(sink_level t_level) const override;
		std::string pattern() const override;

	private:
		mutable Mutex m_mtx;
		std::string m_pattern;
		std::vector<sink_level> m_levels;
	};
}

#include "sink_impl.h"