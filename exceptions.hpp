#ifndef SJTU_EXCEPTIONS_HPP
#define SJTU_EXCEPTIONS_HPP

#include <cstddef>
#include <cstring>
#include <string>

namespace sjtu {

class exception {
protected:
	const std::string variant = "";
	std::string detail = "";
public:
	exception() {}
	exception(const exception &ec) : variant(ec.variant), detail(ec.detail) {}
	virtual std::string what() {
		return variant + " " + detail;
	}
};

class index_out_of_bound : public exception {
public:
    index_out_of_bound() { detail = "index_out_of_bound"; }
};

class runtime_error : public exception {
public:
    runtime_error() { detail = "runtime_error"; }
};

class invalid_iterator : public exception {
public:
    invalid_iterator() { detail = "invalid_iterator"; }
};

class container_is_empty : public exception {
public:
    container_is_empty() { detail = "container_is_empty"; }
};
}

#endif