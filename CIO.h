#pragma once
#include <iostream>
#include <string>
#include <exception>

class CIO
{
public:
	static int ask_and_return_integer(std::string&& _str_ask = "Choose: ")
	{
			std::cout << _str_ask;
			int input_num;
			std::cin >> input_num;
			try {
				if (std::cin.fail())
				{
					std::cin.clear();//���� ���¸� ����մϴ�.
					std::cin.ignore(256, '\n'); //�Է� ���ۿ� �����ִ� �߸��� ������ �����.
					std::cout << "���ڸ� �Է����ּ���." << std::endl;

					return ask_and_return_integer(std::move(_str_ask));
					//std::move()�Լ��� ���ڷ� ������ ���� ������ ������ Ÿ���� ��ȯ���ִ� ���
					//��, &&�� �ٲ��� ����.
				}
			} catch (std::exception _e) {
				std::cout << _e.what() << std::endl;
			}
		return input_num;
	}

	static std::string ask_and_return_string(const std::string&& _str_ask = "Choose: ")
	{
		std::cout << _str_ask;
		std::string str_answer = "";
		try {
			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore(256, 'n');
			}
			std::getline(std::cin, str_answer);
			std::cout << "";
		}
		catch (std::exception _e) {
			std::cout << _e.what() << std::endl;
		}
		return str_answer;
	}
};