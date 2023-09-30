#pragma once
#include <string>
#include <tchar.h>



class StringBuilder {
#if UNICODE 
	using TString = std::wstring;
#else
	using TString = std::string;
#endif;

#define DEFAULT_CAP 2048

public:
	StringBuilder() {}
	StringBuilder(TString::size_type size) : _ScratchSize(size) {}
private:
	TString _Main;
	TString _Scratch;

	const TString::size_type _ScratchSize = 2048;   // 다른 임의의 숫자 넣어도 됨.

public:
	StringBuilder& Append(const TString& str) {
		_Scratch.append(str);
		if (_Scratch.size() > _ScratchSize) {
			_Main.append(_Scratch);
			_Scratch.resize(0);
		}
		return *this;
	}

	StringBuilder& Append_endl(const TString& str) {
		Append(str);
		Append(_T("\n"));
		return *this;
	}

	const TString& str() {
		if (_Scratch.size() > 0) {
			_Main.append(_Scratch);
			_Scratch.resize(0);
		}
		return _Main;
	}

	void clear() {
		_Main.resize(0);
		_Scratch.resize(0);
	}
	TString::size_type size() { return _Scratch.length(); }
};