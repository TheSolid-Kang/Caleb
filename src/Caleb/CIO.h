#pragma once
#include <iostream>
#include <string>
#include <tchar.h>
#include <exception>
#if UNICODE 
using MyString = std::wstring;
#define tcout  wcout
#define tcin  wcin
#else
using MyString = std::string;
#define tcout  cout
#define tcin  cin
#endif;


class CIO
{
public:
	static int ask_and_return_integer(MyString&& _str_ask =_T("Choose: "))
	{
			std::tcout << _str_ask;
			int input_num;
			std::tcin >> input_num;
			try {
				if (std::tcin.fail())
				{
					std::tcin.clear();//오류 상태를 취소합니다.
					std::tcin.ignore(256, '\n'); //입력 버퍼에 남아있는 잘못된 값들을 지운다.
					std::tcout << _T("숫자만 입력해주세요.") << std::endl;

					return ask_and_return_integer(std::move(_str_ask));
					//std::move()함수는 인자로 들어오는 값을 우측값 참조로 타입을 변환해주는 기능
					//즉, &&로 바꿔준 것임.
				}
			} catch (std::exception _e) {
				std::tcout << _e.what() << std::endl;
			}
		return input_num;
	}

	static MyString ask_and_return_string(MyString&& _str_ask = _T("Choose: "))
	{
		std::tcout << _str_ask;
		MyString str_answer = _T("");
		try {
			if (std::tcin.fail())
			{
				std::tcin.clear();
				std::tcin.ignore(256, _T('n'));
			}
			std::getline(std::tcin, str_answer);
			std::tcout << "";
		}
		catch (std::exception _e) {
			std::tcout << _e.what() << std::endl;
		}
		return str_answer;
	}
};