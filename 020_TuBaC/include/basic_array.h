#pragma once

#include <string>

class basic_array
{
private:
	std::string _name;
	std::size_t _size[2];
	bool _two_dimensional;

public:
	void init();
	const std::string& get_name() const;
	void set_name(const std::string& name);
	const bool& is_two_dimensional() const;
	void set_two_dimensional(const bool two_dimensional);
	void set_size(int which, std::size_t size);
	const std::size_t& get_size(int which) const;
};

