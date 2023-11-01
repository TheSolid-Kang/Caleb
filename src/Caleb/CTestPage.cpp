#include "CTestPage.h"
#include "CFileMgr.h"
#include "KMP.h"
#include "CMyEtc.hpp"

CTestPage::CTestPage() 
{
}

CTestPage::~CTestPage()
{
}

void CTestPage::initialize(void)
{
	auto arr_note = build_array(_T("1. 선택한 폴더 내 모든 파일명 출력")
		, _T("2. 원하는 섹션 보기")
		, _T("3. 전체 Caleb 읽고 언급횟수 확인")
		, _T("4. Record 섹션 MSSQL DB에 넣기")
		, _T("5. 가계부 입력")
		, _T("99. EXIT"));
	m_list_title.insert(m_list_title.end(), arr_note.begin(), arr_note.end());
}



void CTestPage::init_func(void)
{
	(*m_uniq_map_func).emplace(std::make_pair(static_cast<size_t>(FUNC::ONE)
		, [&](const void* _p_void) -> std::shared_ptr<void> {
			auto dir_path = CFileMgr::GetOpenFolderDialg(); //폴더 선택 및 출력경로 반환
			auto vec_file = CFIOMgr::GetRecursiveFilesInDirectory(dir_path); //폴더 내 모든 파일명 vector 컨테이너에 담기
			std::for_each(vec_file.cbegin(), vec_file.cend(), [](const TString& _file_path) {std::tcout << _file_path << std::endl; });//출력
			return nullptr; }));
	(*m_uniq_map_func).emplace(std::make_pair(static_cast<size_t>(FUNC::TWO)
		, [&](const void* _p_void) -> std::shared_ptr<void> {
			//1. 파일명 목록 컨테이너 생성
			auto dir_path = CINIMgr::GetPrivateProfileString_INI(_T("PATH"), _T("DIARY_PATH"));
		  auto vec_file_name = CFIOMgr::GetFilesInDirectory(dir_path);

			//2. 검색된 파일 하나씩 열어보며 diary의 TheWords에 내용이 있는지 확인
			//  2-1. 내용이 있다면 map에 저장
			std::map<TString, TString> map_note;
			TString section = _T("TheWords");
			//section = _T("Record");
			//section = _T("Mission");
			//section = _T("Fellowship");
			//section = _T("Praise");
			std::for_each(vec_file_name.cbegin(), vec_file_name.cend()
				, [&map_note, section](const TString& _file_path) {
				auto note = CDiaryMgr::GetInstance().GetDiarySelectedSection(_file_path, section);

				if (true == note._Equal(_T("\n")))
					return;
				else if (true == note._Equal(_T("\n\n")))
					return;
				else if (true == note._Equal(_T("\n\n\n")))
					return;

				map_note.emplace(std::make_pair(_file_path, note));
				});

			for (std::pair<TString, TString> pair_note : map_note)
				//std::tcout << pair_note.first << _T("\n") << pair_note.second;
				std::tcout << pair_note.first << _T("\n");

			return nullptr; }));
	(*m_uniq_map_func).emplace(std::make_pair(static_cast<size_t>(FUNC::THREE)
		, [&](const void* _p_void) -> std::shared_ptr<void> {

			//0. 변수 초기화
			//TString caleb_root_path = CFileMgr::GetOpenFolderDialg(); //잘 됨.
			TString caleb_root_path = _T("C:\\Caleb\\CalebRecord");
			//  vector 컨테이너에 CalebRecord에 담긴 파일 path 입력
			auto vec_caleb_path = CFIOMgr::GetFilesInDirectory(caleb_root_path);
			std::map<TString, std::map<TString, int>> map_cnt;

			//1. 검색어 설정
			//auto search_key = CIO::ask_and_return_string(); //잘 안 됨.
			std::list<TString> list_search_key;
						TString wstr_arr_keyword = CINIMgr::GetPrivateProfileString_INI(_T("SEARCH"), _T("ARR_KEYWORD"));
			auto vec_keyword = CMyEtc::Split(wstr_arr_keyword, _T('|'));
			for (const TString& _search_key : vec_keyword)
				list_search_key.emplace_back(_search_key);

			//2. Caleb 내에서 검색 횟수 확인
			//  1) 컨테이너에서 caleb_path 선택
			for (const TString& _caleb_path : vec_caleb_path)
			{
				//  2) caleb.txt 파일에 담긴 모든 내용 입력
				TString caleb = CFIOMgr::GetFileLines(_caleb_path);
				for (TString _search_key : list_search_key)
				{
					//  3) 한 Caleb에서 검색어 별 언급횟수 확인 및 입력
					auto uniq_vec_address = KMP::GetSearchedAddress(caleb, _search_key);
					if ((*uniq_vec_address).size() != 0)
						map_cnt[_caleb_path][_search_key] += (*uniq_vec_address).size();
				}
			}
			std::map<TString, int> map_total_cnt;
			//3. Console창에 각각의 Caleb에서 검색어 별 언급횟수 출력
			for (std::pair<TString, std::map<TString, int>> _pair_cnt : map_cnt)
			{
				CPage::render( _T("=====") + _pair_cnt.first + _T("====="));
				for (std::pair<TString, int> _pair : _pair_cnt.second)
				{
					CPage::render(_T("    ") + _pair.first + _T(" == ") + std::to_tstring(_pair.second));
					map_total_cnt[_pair.first] += _pair.second;
				}
			}

			//4. 전체 언급횟수 확인
			for (std::pair<TString, int> _pair : map_total_cnt)
				CPage::render(_T("KEY: ") + _pair.first + _T("  |  VALUE: ") + std::to_tstring(_pair.second) + _T("개"));


			return nullptr; }));
	(*m_uniq_map_func).emplace(std::make_pair(static_cast<size_t>(FUNC::FOUR)
		, [&](const void* _p_void) -> std::shared_ptr<void> {
			//0. 변수초기화
			//  가. calebRecord 파일명 리스트 vector 컨테이너에 담기
			TString path = CINIMgr::GetPrivateProfileString_INI(_T("PATH"), _T("DIARY_PATH"));
			//  나. vector 컨테이너에 CalebRecord에 담긴 파일 path 입력
			auto vec_caleb_path = CFIOMgr::GetFilesInDirectory(path);



			//1. Record 섹션 데이터 가져오기
			std::map<TString, TString> listCalebRecord;
			for (auto& _path : vec_caleb_path) {
				auto record = CDiaryMgr::GetInstance().GetDiarySelectedSection(_path, _T("Record"));
				auto InDate = _T("CONVERT(DATE, '") + CFileMgr::GetFileName(_path) + _T("')");

				record = StringEditor::ReplaceAll(record, _T("\'"), _T("\'\'"));
				listCalebRecord[InDate] = record;
			}


			//2. InserQuery 만들기 
			//  #temp 테이블 사용
			StringBuilder strBuil;

			strBuil.Append_endl(_T("USE [Caleb]"));
			strBuil.Append_endl(_T("BEGIN"));
			strBuil.Append_endl(_T("	IF OBJECT_ID(N'TEMPDB..#TCDiary') IS NOT NULL"));
			strBuil.Append_endl(_T("	  DROP TABLE #TCDiary"));
			strBuil.Append_endl(_T("	CREATE TABLE #TCDiary( "));
			strBuil.Append_endl(_T("		[ChurchSeq] [int] NULL,--교회내부코드 "));
			strBuil.Append_endl(_T("		[DiarySeq] [int] NULL, --일기 고유키 "));
			strBuil.Append_endl(_T("		[InDate] [DATE] NULL, --일기 일자 "));
			strBuil.Append_endl(_T("		[Title] [nvarchar](256) NULL, --일기 제목 "));
			strBuil.Append_endl(_T("		[Record] [nvarchar](max) NULL, --일기 "));
			strBuil.Append_endl(_T("		[Remark] [nvarchar](200) NULL, --비고 "));
			strBuil.Append_endl(_T("		[LastUserSeq] [int] NULL, --최종작업자 "));
			strBuil.Append_endl(_T("		[LastDateTime] [datetime] NULL --최종작업일시 "));
			strBuil.Append_endl(_T("	); "));
			strBuil.Append_endl(_T("	SET NOCOUNT ON"));
			for (auto& _record : listCalebRecord) {
				strBuil.Append(_T("	INSERT INTO #TCDiary(ChurchSeq, InDate, Title, Record, Remark, LastUserSeq, LastDateTime) VALUES("));
				strBuil.Append(_T("	1")); //ChurchSeq
				strBuil.Append(_T("	,") + _record.first ); // InDate: DiaryPath의 마지막 일자.
				strBuil.Append(_T("	,N''")); // Title: 없음.
				strBuil.Append(_T("	,N'") + _record.second + _T("'")); // Record
				strBuil.Append(_T("	,N''")); // Remark: 없음.
				strBuil.Append(_T("	, 2")); //LastUserSeq: 2 == 강태경
				strBuil.Append(_T("	, GETDATE()")); //LastDateTime
				strBuil.Append_endl(_T("	);"));
			}

			strBuil.Append_endl(_T(" "));
			strBuil.Append_endl(_T("	SET NOCOUNT OFF"));
			strBuil.Append_endl(_T("	PRINT 'UPDATE에 사용할 임시 테이블' "));
			strBuil.Append_endl(_T("	IF OBJECT_ID(N'TEMPDB..#TCDiaryUpdate') IS NOT NULL "));
			strBuil.Append_endl(_T("		DROP TABLE #TCDiaryUpdate "));
			strBuil.Append_endl(_T("	SELECT A.ChurchSeq, B.DiarySeq, A.InDate, A.Title, A.Record, A.LastUserSeq, A.LastDateTime "));
			strBuil.Append_endl(_T("	INTO #TCDiaryUpdate "));
			strBuil.Append_endl(_T("	FROM #TCDiary AS A "));
			strBuil.Append_endl(_T("	INNER JOIN _TCDiary AS B WITH(NOLOCK) ON A.InDate = B.InDate AND A.Record <> B.Record "));
			strBuil.Append_endl(_T(" "));
			strBuil.Append_endl(_T("	PRINT 'INSERT에 사용할 임시 테이블' "));
			strBuil.Append_endl(_T("	UPDATE #TCDiary "));
			strBuil.Append_endl(_T("	SET DiarySeq = B.DiarySeq "));
			strBuil.Append_endl(_T("	FROM #TCDiary AS A "));
			strBuil.Append_endl(_T("	INNER JOIN _TCDiary AS B ON A.ChurchSeq = B.ChurchSeq AND A.InDate = B.InDate "));
			strBuil.Append_endl(_T(" "));
			strBuil.Append_endl(_T("	IF OBJECT_ID(N'TEMPDB..#TCDiaryInsert') IS NOT NULL "));
			strBuil.Append_endl(_T("		DROP TABLE #TCDiaryInsert "));
			strBuil.Append_endl(_T("	SELECT A.ChurchSeq, A.InDate, A.Title,A.Record, A.Remark, A.LastUserSeq, A.LastDateTime "));
			strBuil.Append_endl(_T("	INTO #TCDiaryInsert "));
			strBuil.Append_endl(_T("	FROM #TCDiary AS A "));
			strBuil.Append_endl(_T("	WHERE 1=1 "));
			strBuil.Append_endl(_T("	AND A.DiarySeq IS NULL "));
			strBuil.Append_endl(_T(" "));
			strBuil.Append_endl(_T("	SELECT * FROM #TCDiary ORDER BY DiarySeq "));
			strBuil.Append_endl(_T("	SELECT * FROM #TCDiaryUpdate "));
			strBuil.Append_endl(_T("	SELECT * FROM #TCDiaryInsert "));
			strBuil.Append_endl(_T(" "));
			strBuil.Append_endl(_T("	BEGIN TRAN "));
			strBuil.Append_endl(_T("		IF (0 <= (SELECT COUNT(*) FROM #TCDiaryUpdate)) OR (0 <= (SELECT COUNT(*) FROM #TCDiaryInsert )) "));
			strBuil.Append_endl(_T("		BEGIN "));
			strBuil.Append_endl(_T("			PRINT 'UPDATE' "));
			strBuil.Append_endl(_T("			UPDATE _TCDiary "));
			strBuil.Append_endl(_T("			SET InDate = A.InDate "));
			strBuil.Append_endl(_T("			, ChurchSeq = A.ChurchSeq"));
			strBuil.Append_endl(_T("			, Title = A.Title"));
			strBuil.Append_endl(_T("			, Record = A.Record "));
			strBuil.Append_endl(_T("			, LastUserSeq = A.LastUserSeq "));
			strBuil.Append_endl(_T("			, LastDateTime = A.LastDateTime "));
			strBuil.Append_endl(_T("			FROM #TCDiaryUpdate AS A "));
			strBuil.Append_endl(_T("			INNER JOIN _TCDiary AS B WITH(NOLOCK) ON A.ChurchSeq = B.ChurchSeq AND A.DiarySeq = B.DiarySeq"));
			strBuil.Append_endl(_T(" "));
			strBuil.Append_endl(_T("			PRINT 'INSERT' "));
			strBuil.Append_endl(_T("			INSERT INTO _TCDiary(ChurchSeq, Indate, Title, Record, Remark, LastUserSeq, LastDateTime) "));
			strBuil.Append_endl(_T("			SELECT ChurchSeq, Indate, Title, Record, Remark, LastUserSeq, LastDateTime "));
			strBuil.Append_endl(_T("			FROM #TCDiaryInsert AS A "));
			strBuil.Append_endl(_T(" "));
			strBuil.Append_endl(_T("			SELECT * FROM _TCDiary ORDER BY InDate DESC"));
			strBuil.Append_endl(_T("		ROLLBACK TRAN "));
			strBuil.Append_endl(_T("		--COMMIT; "));
			strBuil.Append_endl(_T("		END "));
			strBuil.Append_endl(_T("END "));
			strBuil.Append_endl(_T("--TRUNCATE TABLE _TCDiary "));
			strBuil.Append_endl(_T("--DROP TABLE _TCDIARY "));

			//3. InserQuery sql 파일 제작
			auto savePath = CFileMgr::GetEXEFilePath() + _T("\\InserRecordSQL.sql");
			CFileMgr::CreateNewFile(savePath);
			CFileMgr::WriteData(savePath, strBuil.str());

			
			
			return nullptr; }));
	(*m_uniq_map_func).emplace(std::make_pair(static_cast<size_t>(FUNC::GET_DATA)
		, [&](const void* _p_void) -> std::shared_ptr<void> {
			
			//0. 변수초기화
			TString path = CFileMgr::GetOpenFileDialg();

			TString str = StringEditor::ReplaceAll(CFileMgr::ReadData(path), _T("\r"), _T(""));
			
			auto vec = StringEditor::Split(str, _T('\n'));
			std::vector<TString> columnTitles = StringEditor::Split(vec[0], _T(','));
			std::vector<std::vector<TString>> tables;
			tables.reserve(vec.size() + 1);

			KMP kmp;
			for (int i = 1; i < vec.size(); ++i) {
				auto line = vec[i];
				std::vector<TString> row1 = StringEditor::Split(line, _T(','));
				std::vector<TString> row2;
				StringBuilder strBuil;
				if (row1.size() == columnTitles.size()) {
					tables.push_back(row1);
				}
				else{
					//line = StringEditor::ReplaceAll(line, _T("\""), _T("\"\""));

					auto iter = row1.begin();
					auto iter_end = row1.end();
					for (; iter != iter_end; iter++) {
						TString data;
						strBuil.clear();
						if (TString::npos != (*iter).find(_T("\""))) {
							strBuil.Append((*iter));
							for (; TString::npos == (*++iter).find(_T("\"")); ) {
								strBuil.Append((*iter));
							}
							strBuil.Append((*iter));
							data = strBuil.str();
						}
						else {
							data = (*iter);
						}
						row2.push_back(data);
					}
					tables.push_back(row2);
				}
			}

			for (std::vector<TString>& _vec : tables) {
				for (TString& _data : _vec) {
					//날짜변경
					if (TString::npos != _data.find(_T("년 "))
						&& TString::npos != _data.find(_T("월 "))
						&& TString::npos != _data.find(_T("일"))) {
						_data = StringEditor::ParseNotionDate(_data);
					}
					//큰따옴표 삭제
					if (TString::npos != _data.find(_T("\""))) {
						_data = StringEditor::ReplaceAll(_data, _T("\""), _T(""));
					}
				}



			}

			std::tcout << path << std::endl;
			
			return std::make_shared< std::vector<std::vector<TString>>>(tables); }));

	/*
	* 카테고리
	1	헌금
	2	전도비
	3	교제비
	4	적금
	5	생활비
	6	비상금
	7	식비

	* 쿼리 예시
	INSERT INTO _TCFinancialLedger(ChurchSeq, History, Price, CategoryMaj, CategoryMir,InDate, LastDateTime, LastUserSeq) VALUES(1, N'주택청약','20000' ,4,1,CAST(N'2023-11-1' AS DATETIME ),CAST('2023-11-1 12:26' AS DATETIME), 2)


	*/
	(*m_uniq_map_func).emplace(std::make_pair(static_cast<size_t>(FUNC::CREATE_QUERY)
		, [&](const void* _p_void) -> std::shared_ptr<void> {
			//내역, 금액, 카테고리, 지출일자, 최종 수정일자
			auto datas = *(std::vector<std::vector<TString>>*)_p_void;

			std::map<TString, int> mapCategory;
			mapCategory.emplace(_T("헌금"), 1);
			mapCategory.emplace(_T("전도비"), 2);
			mapCategory.emplace(_T("교제비"), 3);
			mapCategory.emplace(_T("적금"), 4);
			mapCategory.emplace(_T("생활비"), 5);
			mapCategory.emplace(_T("비상금"), 6);
			mapCategory.emplace(_T("식비"), 7);


			StringBuilder strBuil;
			strBuil.Append_endl(_T("USE [Caleb]"));
			strBuil.Append_endl(_T("TRUNCATE TABLE[_TCFinancialLedger]"));
			//교회 내부코드, 내역, 금액, 카테고리1,카테고리2, 지출일자, 최종수정일자, 최종수정자
			for (auto& _vec : datas) {
				strBuil.Append_endl(_T("INSERT INTO _TCFinancialLedger(ChurchSeq, History, Price, CategoryMaj, CategoryMir,InDate, LastDateTime, LastUserSeq) VALUES("));
				strBuil.Append_endl(_T("1 "));
				strBuil.Append_endl(_T(", N'") + _vec[0] + _T("'"));
				strBuil.Append_endl(_T(", N'") + _vec[1] + _T("'"));
				strBuil.Append_endl(_T(", 4"));
				strBuil.Append_endl(_T(", ") + StringEditor::ToString(mapCategory[_vec[2]]) );
				strBuil.Append_endl(_T(", CAST(N'") + _vec[3] + _T("' AS DATETIME )"));
				strBuil.Append_endl(_T(", CAST(N'") + _vec[4] + _T("' AS DATETIME )"));
				strBuil.Append_endl(_T(", 2)"));
			}
			strBuil.Append_endl(_T("SELECT * FROM _TCFinancialLedger ORDER BY InDate DESC"));


			//3. InserQuery sql 파일 제작
			auto savePath = CFileMgr::GetEXEFilePath() + _T("\\InserFinancialLedger.sql");
			CFileMgr::CreateNewFile(savePath);
			CFileMgr::WriteData(savePath, strBuil.str());
			return nullptr; }));

}

void CTestPage::init_selected_func(void)
{
	(*m_uniq_map_selected_func).emplace(std::make_pair(static_cast<size_t>(SELECTED_FUNC::ONE)
		, [&](const void* _p_void) -> std::shared_ptr<void> {
			(*m_uniq_map_func)[static_cast<size_t>(FUNC::ONE)](nullptr);
	return nullptr; }));
	(*m_uniq_map_selected_func).emplace(std::make_pair(static_cast<size_t>(SELECTED_FUNC::TWO)
		, [&](const void* _p_void) -> std::shared_ptr<void> {
			(*m_uniq_map_func)[static_cast<size_t>(FUNC::TWO)](nullptr);
			return nullptr; }));
	(*m_uniq_map_selected_func).emplace(std::make_pair(static_cast<size_t>(SELECTED_FUNC::THREE)
		, [&](const void* _p_void) -> std::shared_ptr<void> {
			(*m_uniq_map_func)[static_cast<size_t>(FUNC::THREE)](nullptr);
			return nullptr; }));
	(*m_uniq_map_selected_func).emplace(std::make_pair(static_cast<size_t>(SELECTED_FUNC::FOUR)
		, [&](const void* _p_void) -> std::shared_ptr<void> {
			(*m_uniq_map_func)[static_cast<size_t>(FUNC::FOUR)](nullptr);
			return nullptr; }));
	(*m_uniq_map_selected_func).emplace(std::make_pair(static_cast<size_t>(SELECTED_FUNC::FIVE)
		, [&](const void* _p_void) -> std::shared_ptr<void> {
			auto sharDatas = (*m_uniq_map_func)[static_cast<size_t>(FUNC::GET_DATA)](nullptr);
			auto& datas = *(std::vector<std::vector<TString>>*)sharDatas.get();
			
			(*m_uniq_map_func)[static_cast<size_t>(FUNC::CREATE_QUERY)](&datas);



			return nullptr; }));
}

void CTestPage::render(void)
{
	(*m_uniq_map_selected_func)[static_cast<size_t>(COMMON_SELECTED_FUNC::PRINT_TITLE)](nullptr);
}

int CTestPage::update(int _event)
{
	int result;

	switch (CIO::ask_and_return_integer())
	{
	case static_cast<size_t>(SELECTED_FUNC::ONE):
		(*m_uniq_map_selected_func)[static_cast<size_t>(SELECTED_FUNC::ONE)](nullptr);
		break;
	case static_cast<size_t>(SELECTED_FUNC::TWO):
		(*m_uniq_map_selected_func)[static_cast<size_t>(SELECTED_FUNC::TWO)](nullptr);
		break;
	case static_cast<size_t>(SELECTED_FUNC::THREE):
		(*m_uniq_map_selected_func)[static_cast<size_t>(SELECTED_FUNC::THREE)](nullptr);
		break;
	case static_cast<size_t>(SELECTED_FUNC::FOUR):
		(*m_uniq_map_selected_func)[static_cast<size_t>(SELECTED_FUNC::FOUR)](nullptr);
		break;
	case static_cast<size_t>(SELECTED_FUNC::FIVE):
		(*m_uniq_map_selected_func)[static_cast<size_t>(SELECTED_FUNC::FIVE)](nullptr);
		break;

	default:
		return m_r_page_mgr.SetPage(static_cast<int>(PAGES::P_SELECT));
		break;
	}
	result = m_r_page_mgr.SetPage(static_cast<int>(PAGES::P_TEST));
   return result;
}

void CTestPage::release(void)
{
}
