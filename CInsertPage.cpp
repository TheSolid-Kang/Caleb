#include "CInsertPage.h"

CInsertPage::CInsertPage()
{
}

CInsertPage::~CInsertPage()
{
	release();
}

void CInsertPage::initialize(void)
{
	//CFIOMgr���� std::locale::global(std::locale(".UTF-8"));
	//ó�� �����Ƿ� 
	//�ַܼ� �ѱ� ������ korean���� ���� �ؾ���.
	//std::locale::global(std::locale("Korean"));
	auto arr_note = build_array(typeid(*this).name()
		, "1. ������ �ϱ� ����"
		, "2. ����� ���� �ϱ� ����(���� ��� ����: 221127.TXT)"
		, "3. "
		, ""
		, "99. EXIT");
	m_list_note.insert(m_list_note.end(), arr_note.begin(), arr_note.end());
}

void CInsertPage::render(void)
{
	std::for_each(m_list_note.cbegin(), m_list_note.cend(), [](auto _note) {std::cout << _note << std::endl; });
}

int CInsertPage::update(int _event)
{
	int selected_event = CIO::ask_and_return_integer();
	if(selected_event > 0 && selected_event < m_list_note.size() - 1)
		CreateDiary(selected_event);

	return m_r_page_mgr.SetPage(static_cast<int>(PAGES::P_SELECT));
}

void CInsertPage::release(void)
{
	m_list_note.clear();
}

/// <summary>
/// 
/// 1. INI���� Diary ������ Path Ȯ��
/// 2. ���ϸ� ����
/// 3. 
/// </summary>
/// <param name="iSelectedEvent"></param>
void CInsertPage::CreateDiary(int _iSelectedEvent)
{
	//1. ���� ��ΰ� ���� ��� �����ϱ�
	std::string path = CINIMgr::GetPrivateProfileStringA_INI("PATH", "DIARY_PATH");
	if (false == std::filesystem::exists(path))
		CFIOMgr::CreateDirectorysA(path);

	//2. ���� �����
	auto cur_time = std::chrono::system_clock::now();// �ڷ��� == std::chrono::system_clock::time_point
	std::time_t t_cur_time = std::chrono::system_clock::to_time_t(cur_time);
	struct tm t_tm = *localtime(&t_cur_time);

	std::string str_file_name = CreateName(_iSelectedEvent);

	if (true == std::filesystem::exists(path + "\\" + str_file_name))//������ �̹� �ִٸ� ���� �� ����.
		return;

	//3. ��� �������� 
	//  1) ��� path 
	std::string form_path = CINIMgr::GetPrivateProfileStringA_INI("PATH", "DIARY_FORM_PATH");
	auto vec_line = CFIOMgr::GetVecFileLines(form_path);
	StringBuilder str_buil_line;
	std::for_each(vec_line.cbegin(), vec_line.cend(), [&](auto _line) {str_buil_line.append_endl(_line); });

	//4. ������� ���Ͽ� ��Ŀ� ��� ������ �ֱ�
	std::string full_path = path + "\\" + str_file_name;
	CFIOMgr::WriteText(full_path, str_buil_line.str());
}

/// <summary>
/// ���� �� ����
/// </summary>
/// <param name="_iSelectedEvent"></param>
/// <returns></returns>
std::string CInsertPage::CreateName(int _iSelectedEvent)
{
	StringBuilder str_buil;
		auto cur_time = std::chrono::system_clock::now();// �ڷ��� == std::chrono::system_clock::time_point
		std::time_t t_cur_time = std::chrono::system_clock::to_time_t(cur_time);
		struct tm t_tm = *localtime(&t_cur_time);

	switch (_iSelectedEvent)
	{
	case 1: //������ �ϱ� ����
		//221127
		//������ �𸣰ڴ� error
		//error: case ���̺� ���� t_cur_time, cur_time �� �ʱ�ȭ�� �����ƽ��ϴ�.
		//auto cur_time = std::chrono::system_clock::now();// �ڷ��� == std::chrono::system_clock::time_point
		//std::time_t t_cur_time = std::chrono::system_clock::to_time_t(cur_time);
		//struct tm t_tm = *localtime(&t_cur_time);
		
		//��
		str_buil.append(std::to_string(t_tm.tm_year - 100)); //-100�� ����: 1900�⵵�����̹Ƿ� 100���� �������� �����⿡  
		
		//��
		if(t_tm.tm_mon + 1 > 10)
			str_buil.append(std::to_string(t_tm.tm_mon + 1));// + 1�� ����: 0 ~ 11 ���� ��޵ǹǷ� 
		else
			str_buil.append("0" + std::to_string(t_tm.tm_mon + 1));// + 1�� ����: 0 ~ 11 ���� ��޵ǹǷ� 
		
		//��
		if(t_tm.tm_mday >= 10)
			str_buil.append(std::to_string(t_tm.tm_mday));
		else
			str_buil.append("0" + std::to_string(t_tm.tm_mday));

		break;
	case 2: //����� ���� �ϱ� ����(���� ��� ����: 221127.TXT)
		str_buil.append(std::to_string(CIO::ask_and_return_integer("�Է�(���� 221127): ")));
		
		break;
	}
	str_buil.append(".TXT");

	return str_buil.str();
}

