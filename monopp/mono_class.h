#pragma once

#include "mono_config.h"
#include "mono_type_traits.h"

#include "mono_method.h"
#include "mono_noncopyable.h"
#include <mono/metadata/metadata.h>
#include <string>

namespace mono
{

class mono_assembly;
class mono_class_field;
class mono_class_property;

class mono_class : public common::noncopyable
{
public:
	explicit mono_class(mono_assembly* assembly, MonoClass* cls);
	explicit mono_class(mono_assembly* assembly, MonoImage* image, const std::string& name);
	explicit mono_class(mono_assembly* assembly, MonoImage* image, const std::string& name_space,
						const std::string& name);

	mono_class(mono_class&& o);
	auto operator=(mono_class&& o) -> mono_class&;

	auto get_static_method(const std::string& name, int argc = 0) const -> mono_method;

	template <typename function_signature_t>
	auto get_static_method(const std::string& name);

	auto get_internal_ptr() const -> MonoClass*;

	auto get_field(const std::string& name) const -> mono_class_field;
	auto get_property(const std::string& name) const -> mono_class_property;

	auto get_name() const -> std::string;

	bool is_valuetype() const;

private:
	mono_assembly* assembly_ = nullptr;
	MonoClass* class_ = nullptr;
};

template <typename function_signature_t>
auto mono_class::get_static_method(const std::string& name)
{
	constexpr auto arg_count = function_traits<function_signature_t>::arity;
	auto func = get_static_method(name, arg_count);
	return func.template get_thunk<function_signature_t>();
}

} // namespace mono
