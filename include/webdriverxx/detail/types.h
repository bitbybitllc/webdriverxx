#ifndef WEBDRIVERXX_DETAIL_TYPES_H
#define WEBDRIVERXX_DETAIL_TYPES_H

#include "../conversions.h"
#include "../capabilities.h"
#include <picojson.h>
#include <string>

namespace webdriverxx {
namespace detail {

struct CapabilitiesAccess {
	static
	Capabilities Construct(const picojson::object& object) {
		return Capabilities(object);
	}

	static
	const picojson::object& GetJsonObject(const Capabilities& capabilities) {
		return capabilities.GetJsonObject();
	}
};

struct SessionRef {
	std::string id;
	Capabilities capabilities;
};

struct ElementRef {
	std::string ref;
};

} // namespace detail

template<>
struct FromJsonImpl<detail::SessionRef> {
	static detail::SessionRef Convert(const picojson::value& value) {
		WEBDRIVERXX_CHECK(value.is<picojson::object>(), "Session information is not an object");
		detail::SessionRef result;
		result.id = value.get("sessionId").to_str();
		if (value.get("capabilities").is<picojson::object>())
			result.capabilities = detail::CapabilitiesAccess::Construct(value.get("capabilities").get<picojson::object>());
		return result;
	}
};

template<>
struct FromJsonImpl<detail::ElementRef> {
	static detail::ElementRef Convert(const picojson::value& value) {
		WEBDRIVERXX_CHECK(value.is<picojson::object>(), "ElementRef is not an object");
		detail::ElementRef result;
		result.ref = FromJson<std::string>(value.get("ELEMENT"));
		return result;
	}
};

template<>
struct ToJsonImpl<detail::ElementRef> {
	static picojson::value Convert(const detail::ElementRef& ref) {
		return JsonObject()
			.With("ELEMENT", ref.ref)
			.Build();
	}
};

} // namespace webdriverxx

#endif
