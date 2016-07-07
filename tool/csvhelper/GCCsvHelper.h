//
//  CsvHelper.h
//  gc
//
//	version 1.0.1
//  Created by Stan on 14-10-9.
//  @note:	1.此工具类不依赖于任何配置文件
//			2.不继承于任何类，需要使用者管理内存
//

#ifndef __GC__CsvHelper__
#define __GC__CsvHelper__

#include <vector>
#include <string>
#include "base/gcMacros.h"

NS_GC_UTILS_BEGIN

class CsvHelper
{
public:
	CsvHelper();
	~CsvHelper();
	
	//通过文件名，打开文件并解析csv内容/
	bool openAndResolveFile(const char *fileName);
	
	//通过已经读取的csv内容进行解析/
	bool resolveContent(const char * fileContent);
	
	const char *getData(unsigned int rowIndex, unsigned int colIndex);
	
	inline int getColLength() const { return m_colLength; }
	inline int getRowLength() const {	return (int)m_data.size(); }
	
	std::string  getRowData(int rowIndex) const;

	std::vector<std::vector<std::string> > const &getAllData() const
	{
		return m_data;
	}
	
private:
	const std::string m_seperator;
	std::vector<std::vector<std::string> > m_data;
	
	//cols length
	int m_colLength;
	
	void rowSplit(std::vector<std::string> &rows, const std::string &content, const char &rowSeperator);
	void fieldSplit(std::vector<std::string> &fields, std::string line);
	
	//@brief 获取带引号的字段/
	//@return 下一个逗号分隔符的index/
	int getFieldWithQuoted(const std::string &line, std::string& field, int index);
	
	//@brief 获取无引号的字段/
	//@return 下一个逗号分隔符的index/
	int getFieldNoQuoted(const std::string &line, std::string &field, int index);
};

//csv 数据结构/
typedef std::vector<std::vector<std::string> > csvDataStructure;
typedef std::vector<std::vector<std::string> >::iterator iterator_csvRow;
typedef std::vector<std::vector<std::string> >::const_iterator const_iterator_csvRow;

NS_GC_UTILS_END

#endif /* defined(__GC__CsvHelper__) */
