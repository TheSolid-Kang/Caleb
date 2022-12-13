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
	//CFIOMgr에서 std::locale::global(std::locale(".UTF-8"));
	//처리 했으므로 
	//콘솔로 한글 보려면 korean으로 변경 해야함.
	//std::locale::global(std::locale("Korean"));
	auto arr_note = build_array(typeid(*this).name()
		, "1. 오늘자 일기 생성"
		, "2. 사용자 정의 일기 생성(생성 결과 예시: 221127.TXT)"
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
/// 1. INI에서 Diary 저장할 Path 확인
/// 2. 파일명 제작
/// 3. 
/// </summary>
/// <param name="iSelectedEvent"></param>
void CInsertPage::CreateDiary(int _iSelectedEvent)
{
	//1. 폴더 경로가 없는 경우 생성하기
	std::string path = CINIMgr::GetPrivateProfileStringA_INI("PATH", "DIARY_PATH");
	if (false == std::filesystem::exists(path))
		CFIOMgr::CreateDirectorysA(path);

	//2. 파일 만들기
	auto cur_time = std::chrono::system_clock::now();// 자료형 == std::chrono::system_clock::time_point
	std::time_t t_cur_time = std::chrono::system_clock::to_time_t(cur_time);
	struct tm t_tm = *localtime(&t_cur_time);

	std::string str_file_name = CreateName(_iSelectedEvent);

	if (true == std::filesystem::exists(path + "\\" + str_file_name))//파일이 이미 있다면 로직 안 돈다.
		return;

	//3. 양식 가져오기 
	//  1) 양식 path 
	std::string form_path = CINIMgr::GetPrivateProfileStringA_INI("PATH", "DIARY_FORM_PATH");
	auto vec_line = CFIOMgr::GetVecFileLines(form_path);
	StringBuilder str_buil_line;
	std::for_each(vec_line.cbegin(), vec_line.cend(), [&](auto _line) {str_buil_line.append_endl(_line); });

	//4. 만들어진 파일에 양식에 담긴 데이터 넣기
	std::string full_path = path + "\\" + str_file_name;
	CFIOMgr::WriteText(full_path, str_buil_line.str());
}

/// <summary>
/// 파일 명 생성
/// </summary>
/// <param name="_iSelectedEvent"></param>
/// <returns></returns>
std::string CInsertPage::CreateName(int _iSelectedEvent)
{
	StringBuilder str_buil;
		auto cur_time = std::chrono::system_clock::now();// 자료형 == std::chrono::system_clock::time_point
		std::time_t t_cur_time = std::chrono::system_clock::to_time_t(cur_time);
		struct tm t_tm = *localtime(&t_cur_time);

	switch (_iSelectedEvent)
	{
	case 1: //오늘자 일기 생성
		//221127
		//이유는 모르겠는 error
		//error: case 레이블에 있해 t_cur_time, cur_time 의 초기화가 생략됐습니다.
		//auto cur_time = std::chrono::system_clock::now();// 자료형 == std::chrono::system_clock::time_point
		//std::time_t t_cur_time = std::chrono::system_clock::to_time_t(cur_time);
		//struct tm t_tm = *localtime(&t_cur_time);
		
		//연
		str_buil.append(std::to_string(t_tm.tm_year - 100)); //-100한 이유: 1900년도부터이므로 100년이 더해져서 나오기에  
		
		//월
		if(t_tm.tm_mon + 1 > 10)
			str_buil.append(std::to_string(t_tm.tm_mon + 1));// + 1한 이유: 0 ~ 11 월로 취급되므로 
		else
			str_buil.append("0" + std::to_string(t_tm.tm_mon + 1));// + 1한 이유: 0 ~ 11 월로 취급되므로 
		
		//일
		if(t_tm.tm_mday >= 10)
			str_buil.append(std::to_string(t_tm.tm_mday));
		else
			str_buil.append("0" + std::to_string(t_tm.tm_mday));

		break;
	case 2: //사용자 정의 일기 생성(생성 결과 예시: 221127.TXT)
		str_buil.append(std::to_string(CIO::ask_and_return_integer("입력(예시 221127): ")));
		
		break;
	}
	str_buil.append(".TXT");

	return str_buil.str();
}

