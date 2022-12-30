#pragma once
#include <string>
#include <tchar.h>

#if UNICODE 
using TString = std::wstring;
#else
using TString = std::string;
#endif;

#define DEF_CAP 2048


class StringBuilder {
private:
	TString main;
	TString scratch;

	const TString::size_type ScratchSize = 1024;  // 다른 임의의 숫자 넣어도 됨.

public:
	StringBuilder& append(const TString& str) {
		scratch.append(str);
		if (scratch.size() > ScratchSize) {
			main.append(scratch);
			scratch.resize(0);
		}
		return *this;
	}

	StringBuilder& append_endl(const TString& str) {
		append(str);
		append(_T("\n"));
		return *this;
	}

	const TString& str() {
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