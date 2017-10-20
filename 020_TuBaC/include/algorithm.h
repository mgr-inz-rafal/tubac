#pragma once
#include <cstdint>

template<typename Func>
void call_n(int64_t count, Func f) {
	while(count--) {
		f();
	}
}

