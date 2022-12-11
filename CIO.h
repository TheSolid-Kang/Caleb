#pragma once
#include <iostream>
#include <string>

class CIO
{
public:
	static int ask_and_return_integer(std::string&& _str_ask = "Choose: ")
	{
		std::cout << _str_ask;
		int input_num;
		std::cin >> input_num;
		if (std::cin.fail())
		{
			std::cin.clear();//오류 상태를 취소합니다.
			std::cin.ignore(256, '\n'); //입력 버퍼에 남아있는 잘못된 값들을 지운다.
			std::cout << "숫자만 입력해주세요." << std::endl;

			return ask_and_return_integer(std::move(_str_ask));
			//std::move()함수는 인자로 들어오는 값을 우측값 참조로 타입을 변환해주는 기능
			//즉, &&로 바꿔준 것임.
		}
		return input_num;
	}

	static std::string ask_and_return_string(const std::string&& _str_ask = "Choose: ")
	{
		std::cout << _str_ask;
		std::string str_answer = "";
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(256, 'n');
		}
		std::getline(std::cin, str_answer);
		std::cout << "";
		return str_answer;
	}
};