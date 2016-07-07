//
//  CsvHelper.cpp
//  Cell
//
//  Created by Stan on 14-10-9.
//
//

#include "GCCsvHelper.h"
#include "cocos2d.h"

USING_NS_CC;

NS_GC_UTILS_BEGIN

CsvHelper::CsvHelper()
:m_seperator(",")
,m_data()
,m_colLength(0)
{
	
}

CsvHelper::~CsvHelper()
{
	
}

#pragma region reselove the content begin...

bool CsvHelper::openAndResolveFile(const char *fileName)
{
	std::string pathKey =FileUtils::getInstance()->fullPathForFilename(fileName);
	unsigned char* pBuffer = NULL;
	ssize_t	bufferSize = 0;
	pBuffer = FileUtils::getInstance()->getFileData(pathKey, "r", &bufferSize);
	std::string fileContent((char*)pBuffer, bufferSize);
	
	return resolveContent(fileContent.c_str());
}

bool CsvHelper::resolveContent(const char *fileContent)
{
	if (fileContent == NULL || strlen(fileContent) == 0) {
		CCLOGERROR("flieContent is null or empty");
		return false;
	}
	
	//remove BOM(\xef\xbb\xbf), if the document format is UTF-8 BOM.
	const char *content = NULL;
	if (fileContent[0] == '\xef') {
		//the document format is UTF-8 BOM.
		CCLOG("the current csv file format is UTF-8 BOM, need remove BOM");
		content = fileContent + 3;
	} else {
		content = fileContent;
	}
	
	std::vector<std::string> line;
	rowSplit(line, content, '\n');
	for (unsigned int i = 0; i < line.size(); ++i) {
		std::vector<std::string> fieldVector;
		fieldSplit(fieldVector, line[i]);
		m_data.push_back(fieldVector);
		m_colLength = (std::max)(m_colLength, (int)fieldVector.size());
	}
	
	return true;
}

void CsvHelper::rowSplit(std::vector<std::string> &rows, const std::string &content, const char &rowSeperator)
{
	std::string::size_type lastIndex = content.find_first_not_of(rowSeperator, 0);
	std::string::size_type	currentIndex = content.find_first_of(rowSeperator,lastIndex);
	
	while (std::string::npos != currentIndex || std::string::npos != lastIndex) {
		rows.push_back(content.substr(lastIndex, currentIndex - lastIndex));
		lastIndex = content.find_first_not_of(rowSeperator, currentIndex);
		currentIndex = content.find_first_of(rowSeperator, lastIndex);
	}
}

void CsvHelper::fieldSplit(std::vector<std::string> &fields, std::string line)
{
	if (line[line.length() - 1] == '\r') {
		line = line.substr(0, line.length() - 1);
	}
	
	std::string field;
	unsigned int i = 0, j = 0;
	while (j < line.length()) {
		if (line[i] == '"') {
			//有引号/
			j = getFieldWithQuoted(line, field, i);
		} else {
			j = getFieldNoQuoted(line, field, i);
		}
		
		fields.push_back(field);
		i = j + 1; //解析下一个field， ＋1为了跳过当前的分隔符/
	}
}

int CsvHelper::getFieldWithQuoted(const std::string &line, std::string &field, int i)
{
	unsigned int j = 0;
	field = std::string();
	if (line[i] != '"') {
		//不是引号起始，有问题/
		CCLOGERROR("start char is not quote when call %s", __FUNCTION__);
		return -1;
	}
	
	for (j = i + 1; j < line.length() - 1; ++j) {
		if (line[j] != '"') {
			//当前char不为引号，则是field内容(包括逗号)/
			field += line[j];
		} else {
			//遇到field结束时的引号，可以返回，注意这里返回的是下一个逗号分隔符的index/
			return j + 1;
			break;
		}
	}
	
	if (j == line.length()) {
		//没有找到成对的结束引号/
		CCLOGERROR("resoleve the line error: no pair quote, line:%s, field:%s, start index:%d", line.c_str(), field.c_str(), i);
	}
	
	return j;
}

int CsvHelper::getFieldNoQuoted(const std::string &line, std::string &field, int index)
{
	unsigned int j = 0;
	//找到下一个分隔符位置/
	j = line.find_first_of(m_seperator, index);
	if (j > line.length()) {
		j = line.length();
	}
	
	field = std::string(line, index, j - index);
	
	return j;
}

#pragma endregion

///////search data
const char *CsvHelper::getData(unsigned int rowIndex, unsigned int colIndex)
{
	if (rowIndex >= getRowLength() || colIndex >= getColLength()) {
		return "";
	}
	
	if (colIndex >= m_data[rowIndex].size()) {
		return "";
	}
	
	return m_data[rowIndex][colIndex].c_str();
}

std::string  CsvHelper::getRowData(int rowIndex) const
{
	if (rowIndex >= getRowLength())
	{
		return "";
	}
	
	std::string strRet;
	
	std::vector<std::string>::const_iterator itor = m_data[rowIndex].begin();
	for (; itor != m_data[rowIndex].end(); ++itor)
	{
		strRet += *itor + ",";
	}
	
	return strRet;
}


NS_GC_UTILS_END

