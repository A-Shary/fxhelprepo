#include "stdafx.h"

#include "FamilyDataCenterUI/FamilyDataController.h"

#include "FamilyDataCenterUI/Application.h"
#include "FamilyDataCenterUI/Range.h"
#include "FamilyDataCenterUI/Workbook.h"
#include "FamilyDataCenterUI/Workbooks.h"
#include "FamilyDataCenterUI/Worksheet.h"
#include "FamilyDataCenterUI/Worksheets.h"

#include "FamilyDataCenterUI/family_background/FamilyBackground.h"

#include "third_party/chromium/base/path_service.h"
#include "third_party/chromium/base/strings/utf_string_conversions.h"
#include "third_party/chromium/base/strings/string_number_conversions.h"

//
//struct SingerSummaryData
//{
//    uint32 singerid;
//    uint32 roomid;
//    std::string nickname;
//    std::string singerlevel;    //�����ȼ�
//    uint32 onlinecount;         //��������
//    uint32 onlineminute;        //�ۼ�ֱ��ʱ�������ӣ�
//    uint32 effectivecount;      //��Чֱ������������1��Сʱ��
//    uint32 maxusers;            //ֱ�����������
//    double revenue;             // �Ƕ�����
//};
//

namespace{
    const wchar_t* patternName = L"pattern.xlsx";

    bool SingerSummaryDataToGridData(
        const std::vector<SingerSummaryData>& singerSummaryData,
        GridData* griddata)
    {
        if (!griddata)
            return false;

        for (const auto& it:singerSummaryData)
        {
            RowData rowdata;
            rowdata.push_back(base::UTF8ToWide(it.nickname));
            rowdata.push_back(base::UTF8ToWide(it.singerlevel));
            rowdata.push_back(base::UintToString16(it.onlinecount));
            rowdata.push_back(base::UintToString16(it.onlineminute));
            rowdata.push_back(base::UintToString16(it.effectivecount));
            rowdata.push_back(base::UintToString16(it.maxusers));
            std::wstring revenue = base::UTF8ToWide(base::DoubleToString(it.revenue));
            rowdata.push_back(revenue);
            griddata->push_back(rowdata);
        }

        return true;
    }
}

FamilyDataController::FamilyDataController()
{
    base::FilePath path;
    PathService::Get(base::DIR_EXE, &path);
    exePath_ = path;
}

FamilyDataController::~FamilyDataController()
{
}

bool FamilyDataController::Init()
{
    familyBackground_.reset(new FamilyBackground);
    return false;
}

bool FamilyDataController::Login(const std::string& username, 
    const std::string& password)
{
    if (!familyBackground_)
        return false;
    if (!familyBackground_->Init())
        return false;

    if (!familyBackground_->Login(username, password))
        return false;

    return true;
}

bool FamilyDataController::Login(const std::wstring& wusername,
    const std::wstring& wpassword)
{
    std::string username;
    std::string password;
    base::WideToUTF8(wusername.c_str(), wusername.length(), &username);
    base::WideToUTF8(wpassword.c_str(), wpassword.length(), &password);
    bool result = Login(username, password);
    return result;
}

bool FamilyDataController::GetSingerFamilyData(
    const base::Time& begintime,
    const base::Time& endtime,
    GridData* griddata)
{
    if (!familyBackground_)
        return false;

    singerSummaryData_.reset(new std::vector<SingerSummaryData>);
    std::vector<SingerSummaryData> singerSummaryData;
    bool result = familyBackground_->GetSummaryData(
        begintime, endtime, &singerSummaryData);

    //std::wstring filepath = L"d:/now.xlsx";
    //return ExportToExcel(filepath);
}

bool FamilyDataController::ExportToExcel(const std::wstring& filepath)
{
    CApplication ExcelApp;
    CWorkbooks books;
    CWorkbook book;
    CWorksheets sheets;
    CWorksheet sheet;
    CRange range;
    LPDISPATCH lpDisp = NULL;

    //����Excel ������(����Excel)
    if (!ExcelApp.CreateDispatch(_T("Excel.Application"), NULL))
    {
        AfxMessageBox(_T("����Excel������ʧ��!"));
        return false;
    }

    /*�жϵ�ǰExcel�İ汾*/
    CString strExcelVersion = ExcelApp.get_Version();
    int iStart = 0;
    strExcelVersion = strExcelVersion.Tokenize(_T("."), iStart);
    if (_T("11") == strExcelVersion)
    {
        AfxMessageBox(_T("��ǰExcel�İ汾��2003��"));
    }
    else if (_T("12") == strExcelVersion)
    {
        AfxMessageBox(_T("��ǰExcel�İ汾��2007��"));
    }
    else if (_T("14") == strExcelVersion)
    {
        AfxMessageBox(_T("��ǰExcel�İ汾��2010��"));
    }
    else
    {
        AfxMessageBox(_T("��ǰExcel�İ汾�������汾��"));
    }

    ExcelApp.put_Visible(TRUE);
    ExcelApp.put_UserControl(FALSE);

    /*�õ�����������*/
    books.AttachDispatch(ExcelApp.get_Workbooks());

    /*��һ�����������粻���ڣ�������һ��������*/
    std::wstring patternPathFile = exePath_.Append(base::FilePath(patternName)).value();
    try
    {
        /*��һ��������*/
        lpDisp = books.Open(patternPathFile.c_str(),
            vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
            vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
            vtMissing, vtMissing, vtMissing, vtMissing);
        book.AttachDispatch(lpDisp);
    }
    catch (...)
    {
        /*����һ���µĹ�����*/
        //lpDisp = books.Add(vtMissing);
        //book.AttachDispatch(lpDisp);
        AfxMessageBox(_T("�޷���ģ���ļ�"));
        return false;
    }


    /*�õ��������е�Sheet������*/
    sheets.AttachDispatch(book.get_Sheets());

    /*��һ��Sheet���粻���ڣ�������һ��Sheet*/
    CString strSheetName = _T("FamilyData");

    try
    {
        /*��һ�����е�Sheet*/
        lpDisp = sheets.get_Item(_variant_t(strSheetName));
        sheet.AttachDispatch(lpDisp);
    }
    catch (...)
    {
        /*����һ���µ�Sheet*/
        lpDisp = sheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
        sheet.AttachDispatch(lpDisp);
        sheet.put_Name(strSheetName);
    }

    /*��Sheet��д������Ԫ��,��ģΪ10*10 */
    lpDisp = sheet.get_Range(_variant_t("A2"), _variant_t("G11"));
    range.AttachDispatch(lpDisp);

    VARTYPE vt = VT_I4; /*����Ԫ�ص����ͣ�long*/
    SAFEARRAYBOUND sabWrite[2]; /*���ڶ��������ά�����±����ʼֵ*/
    sabWrite[0].cElements = 10;
    sabWrite[0].lLbound = 0;
    sabWrite[1].cElements = 10;
    sabWrite[1].lLbound = 0;

    COleSafeArray olesaWrite;
    olesaWrite.Create(vt, sizeof(sabWrite) / sizeof(SAFEARRAYBOUND), sabWrite);

    /*ͨ��ָ�������ָ�����Զ�ά�����Ԫ�ؽ��м�Ӹ�ֵ*/
    long(*pArray)[2] = NULL;
    olesaWrite.AccessData((void **)&pArray);
    memset(pArray, 0, sabWrite[0].cElements * sabWrite[1].cElements * sizeof(long));

    /*�ͷ�ָ�������ָ��*/
    olesaWrite.UnaccessData();
    pArray = NULL;

    /*�Զ�ά�����Ԫ�ؽ��������ֵ*/
    long index[2] = { 0, 0 };
    long lFirstLBound = 0;
    long lFirstUBound = 0;
    long lSecondLBound = 0;
    long lSecondUBound = 0;
    olesaWrite.GetLBound(1, &lFirstLBound);
    olesaWrite.GetUBound(1, &lFirstUBound);
    olesaWrite.GetLBound(2, &lSecondLBound);
    olesaWrite.GetUBound(2, &lSecondUBound);
    for (long i = lFirstLBound; i <= lFirstUBound; i++)
    {
        index[0] = i;
        for (long j = lSecondLBound; j <= lSecondUBound; j++)
        {
            index[1] = j;
            long lElement = i * sabWrite[1].cElements + j;
            olesaWrite.PutElement(index, &lElement);
        }
    }

    /*��ColesaWritefeArray����ת��ΪVARIANT,��д�뵽Excel�����*/
    VARIANT varWrite = (VARIANT)olesaWrite;
    range.put_Value2(varWrite);

    /*�����ļ��ĺ�׺��ѡ�񱣴��ļ��ĸ�ʽ*/
    CString strSaveAsName = _T("d:\\new.xlsx");
    XlFileFormat NewFileFormat = xlOpenXMLWorkbook;

    book.SaveAs(_variant_t(strSaveAsName), _variant_t((long)NewFileFormat), 
        vtMissing, vtMissing, vtMissing, vtMissing, 0, vtMissing, vtMissing, 
        vtMissing, vtMissing, vtMissing);


    /*�ͷ���Դ*/
    sheet.ReleaseDispatch();
    sheets.ReleaseDispatch();
    book.ReleaseDispatch();
    books.ReleaseDispatch();
    ExcelApp.Quit();
    ExcelApp.ReleaseDispatch();
    return true;
}
