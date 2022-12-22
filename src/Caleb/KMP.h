#pragma once
#include <iostream>
#include <vector>
#include <string>
class KMP
{
private:
	static std::unique_ptr<std::vector<int> > get_pi(const std::string& _strSearchKey) {
		int j = 0;
		int iSearchKeySize = (int)_strSearchKey.size();
		std::unique_ptr<std::vector<int> > uniq_vec_pi = std::make_unique<std::vector<int>>(iSearchKeySize, 0);

		for (int i = 1; i < iSearchKeySize; ++i)
		{
			while (j > 0 && _strSearchKey[i] != _strSearchKey[j])
				j = (*uniq_vec_pi).at(j - 1);
			if (_strSearchKey[i] == _strSearchKey[j])
				(*uniq_vec_pi).at(i) = ++j;
		}
		return std::move(uniq_vec_pi);
	}

public:
	static std::unique_ptr<std::vector<int> > GetSearchedAddress(const std::string& _strText, const std::string& _strSearchKey) {
		int j = 0;
		int iTextSize = (int)_strText.size();
		int iSearchKeySize = (int)_strSearchKey.size();

		decltype(auto) uniq_vec_pi = get_pi(_strSearchKey);
		std::unique_ptr<std::vector<int>> uniq_vec_addr = std::make_unique<std::vector<int>>();

		for (int i = 0; i < iTextSize; ++i)
		{
			while (j > 0 && _strText[i] != _strSearchKey[j])
				j = (*uniq_vec_pi).at(j - 1);
			if (_strText[i] == _strSearchKey[j])
			{
				if (j == iSearchKeySize - 1)
				{
					(*uniq_vec_addr).push_back(i - iSearchKeySize + 1);
					j = (*uniq_vec_pi).at(j);
				}
				else
					++j;
			}
		}

		return std::move(uniq_vec_addr);
	}

};