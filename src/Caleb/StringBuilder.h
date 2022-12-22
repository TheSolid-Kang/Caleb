#pragma once
#include <string>

class StringBuilder {
private:
	std::string main;
	std::string scratch;

	const std::string::size_type ScratchSize = 1024;  // 다른 임의의 숫자 넣어도 됨.

public:
	StringBuilder& append(const std::string& str) {
		scratch.append(str);
		if (scratch.size() > ScratchSize) {
			main.append(scratch);
			scratch.resize(0);
		}
		return *this;
	}

	StringBuilder& append_endl(const std::string& str) {
		append(str);
		append("\n");
		return *this;
	}

	const std::string& str() {
		if (scratch.size() > 0) {
			main.append(scratch);
			scratch.resize(0);
		}
		return main;
	}

	void clear() {
		main.resize(0);
		scratch.resize(0);
	}
};