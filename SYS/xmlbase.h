/*! \file    xmlbase.h
   \brief    xml接口层定义
   \author   zl
 */
#ifndef TIXML_BASE_INCLUDED
#define TIXML_BASE_INCLUDED

#include <string>
#include "SYSModuleHeader.h"
#include "tinyxml.h"
using namespace std;

#if (USE_XML_FUNC!=0)



#define XML_SET_TEXT_CDATA		1
#define XML_SET_TEXT_NORMAL		0


/*!
@class XMLParse
@brief 用于XML解析
*/
class XMLParse
{
public:
	
	XMLParse();	
	~XMLParse();

	/*!
	@brief 开始对一个XML文件的解析
	@param[in] file XML文件名
	@param[in] charType 字符集类型：GBK_CHAR_TYPE、UTF8_CHAR_TYPE
	*/
	int XMLParseBegin( const char *file, int charType = GBK_CHAR_TYPE );

	/*!
	@brief 结束当前的XML解析
	*/
	void XMLParseEnd();

	/*!
	@brief 依据序号定位一个子节点
	@param[in] ParentElement 父节点指针
	@param[in] ChildNo 子节点序号，以0为起始（0表示第一个子节点，1表示第二个）
	@return  1 - 找到了指定的子节点  0 - 未找到 
	*/
	int LocateNode( TiXmlElement *ParentElement, int ChildNo=0 );

	/*!
	@brief 依据名称和序号定位一个子节点，即定位符合给定名称的第N个节点。
	@param[in] ParentElement 父节点指针
	@param[in] strName 子结点名称
	@param[in] ChildNo 子节点序号，以0为起始（0表示第一个子节点，1表示第二个）
	@return  1 - 找到了指定的子节点  0 - 未找到 
	*/
	int LocateNodeByName( TiXmlElement *ParentElement, char* strName, int ChildNo=0);

	/*!
	@brief 取当前定位节点的值
	@return  节点值 
	*/
	string GetText();

	/*!
	@brief 取当前定位节点的某个属性
	@param[in] AttrName 属性名称
	@return  属性值 
	*/
	string GetAttr( string AttrName );

	int SignNode(TiXmlElement *ParentElement);

private:
	int getNodeLayer(TiXmlElement *node);

public:
	TiXmlDocument *m_myDocument;			/**< XML文档句柄	*/
	TiXmlElement *m_RootElement;			/**< 根节点的指针。设根节点是第0层，其下一层是第1层。	*/
	TiXmlElement *m_parentElement[11];		/**< 各层的指针，树的每一层对应一个指针。比如第1层的指针是m_parentElement[1]，该指针可以在该层的所有节点中移动。	*/
	TiXmlElement *m_Child;					/**< 儿子节点的指针	*/

	TiXmlElement *m_signNode;
	int m_signNodeLayer;

};


/*!
@class XMLConstruct
@brief 用于XML组装
*/
class XMLConstruct
{
public:
	
	XMLConstruct();
	~XMLConstruct();

	/*!
	@brief 开始组装一个XML文件，基本属性使用默认值。
	@param[in] charType 字符集类型，默认为GBK_CHAR_TYPE
	*/
	void XMLConstructBegin( int charType = GBK_CHAR_TYPE);

	/*!
	@brief 开始组装一个XML文件，基本属性使用传入参数
	@param[in] ver 版本属性
	@param[in] character 字符集属性，“UTF-8”或“GBK”
	@param[in] rootName 根节点名称
	@param[in] charType 字符集类型，应与character值相对应，默认为GBK_CHAR_TYPE
	*/
	void XMLConstructBegin(string ver, string character, string rootName, int charType = GBK_CHAR_TYPE);

	void XMLConstructBeginSimple(string rootName, int charType);

	/*!
	@brief 结束当前XML的组装并保存为文件。
	@param[in] XmlPath 生成的XML文件名与路径
	@return  1 - 成功  0 - 失败
	*/
	int XMLConstructEnd(const char *XmlPath );

	void XMLConstructDelete( );

	/*!
	@brief 添加一个子节点
	@param[in] ParentElement 父节点指针
	@param[in] nodeName 子节点名称
	@return  0 - 成功 
	*/
	int AddNode( TiXmlElement *ParentElement, string nodeName );

	/*!
	@brief 向新AddNode出的节点添加值
	@param[in] TextValue 节点值
	@param[in] type 节点值类型：XML_SET_TEXT_NORMAL或XML_SET_TEXT_CDATA
	@return  0 - 成功 
	*/
	int AddText( string TextValue, int type=XML_SET_TEXT_NORMAL );

	/*!
	@brief 向新AddNode出的节点添加属性
	@param[in] AttrName 属性名称
	@param[in] AttrValue 属性值
	@return  0 - 成功 
	*/
	int AddAttr( string AttrName, string AttrValue );

	int InsertNode(TiXmlElement *pParent, TiXmlElement *pNode);
	int InsertAllChildNode(TiXmlElement *pParent, TiXmlElement *pNode);

public:
    TiXmlDocument *m_myDocument;			/**< XML文档句柄	*/
	TiXmlDeclaration *m_xmlDeclaration;		/**< XML文档属性	*/
	TiXmlElement *m_RootElement;			/**< 根节点的指针。设根节点是第0层，其下一层是第1层。	*/
	TiXmlElement *m_parentElement[11];		/**< 各层的指针，树的每一层对应一个指针。比如第1层的指针是m_parentElement[1]，该指针可以在该层的所有节点中移动。	*/
	TiXmlElement *m_NewElement;				/**< 新加节点的指针	*/
	TiXmlText *m_Content;					/**< 节点值内容	*/



};

#endif		//USE_XML_FUNC

#endif		//TIXML_BASE_INCLUDED
