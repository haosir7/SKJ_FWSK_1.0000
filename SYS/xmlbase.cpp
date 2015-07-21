/*! \file    xmlbase.cpp
   \brief    xml接口层实现
   \author   zl
 */

#include "IncludeMe.h"
#include "commonFunc.h"
#include "xmlbase.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


#if (USE_XML_FUNC!=0)



XMLParse::XMLParse()
{
	m_signNode = NULL;
	m_signNodeLayer = 0;
}

XMLParse::~XMLParse()
{
	
}

int XMLParse::XMLParseBegin( const char *file, int charType )
{
	//创建一个XML的文档对象。
    m_myDocument = new TiXmlDocument(file);
	m_myDocument->SetCharType(charType);
    m_myDocument->LoadFile();

    //获得根元素，即
    m_RootElement = m_myDocument->RootElement();
	m_parentElement[0] = m_RootElement;
	
	if (m_RootElement == NULL)
		return FAILURE;
	else
		return SUCCESS;
}

void XMLParse::XMLParseEnd()
{	
	if (NULL != m_myDocument)
	{
		delete m_myDocument;
		m_myDocument = NULL;
	}	
	return;
}

int XMLParse::LocateNode( TiXmlElement *ParentElement, int ChildNo )
{
	int j;
	
	m_Child = ParentElement->FirstChildElement();
		
	if( ChildNo > 0 ) //如果ChildNo大于0，则获得第ChildNo+1个节点
	{
		for( j=0; j<ChildNo; j++ )
		{
			m_Child = m_Child->NextSiblingElement();
		}
	}

	if( m_Child==NULL )
	{
		return FAILURE;
	}
	else
	{
		return SUCCESS;
	}
}

int XMLParse::LocateNodeByName( TiXmlElement *ParentElement, char* strName, int ChildNo)
{
	int i;

	m_Child = ParentElement->FirstChildElement(strName);
	for (i=0;i<ChildNo;i++)
	{
		if (m_Child == NULL)
		{
			DBG_PRINT(("m_Child == NULL break i : %d", i));
			break;
		}
		m_Child = m_Child->NextSiblingElement(strName);
	}
	
	if( m_Child == NULL )
	{		
		DBG_PRINT(("m_Child == NULL %s", strName));
		return FAILURE;
	}

	return SUCCESS;
}

string XMLParse::GetText()
{
	string oriText("");
	string nodeText("");
	char *pout=NULL;

	if (m_Child == NULL)
		return nodeText;

	oriText = m_Child->GetText();
	
#if (USE_ICONV_LIB!=0)
	int ret;
	if(m_myDocument->m_charType == UTF8_CHAR_TYPE)
	{
		ret = code_convert("utf-8", "gbk", oriText.c_str(), oriText.length(), &pout);		
		if (ret != 0)
		{
			DBG_PRINT(("utf8转GBK错误: %d", ret));
		}
		else
		{
			nodeText.assign(pout);
		}
	}
	else
	{
		nodeText = oriText;
	}

#else
	nodeText = oriText;
#endif		//USE_ICONV_LIB

	return nodeText;
}

//-------------------------------------------------------
//功能：获得节点的属性
//输入：属性名
//输出：属性值
//说明：本函数是获取LocateNode指向的节点的属性，即m_Child指向的节点的属性
//举例：
//-------------------------------------------------------
string XMLParse::GetAttr( string AttrName )
{
	string AttrValue("");
	
	if (m_Child != NULL)
	{
		AttrValue = m_Child->Attribute(AttrName.c_str());
	}
		
	return AttrValue;
}

int XMLParse::SignNode(TiXmlElement *node)
{
	int layer=-1;

	if (node == NULL)
	{
		return FAILURE;
	}
	m_signNode = node;

	layer = getNodeLayer(node);
	if (layer==-1)	
	{
		return FAILURE;
	}
	m_signNodeLayer = layer;

	return SUCCESS;
}

int XMLParse::getNodeLayer(TiXmlElement *node)
{
	int layer=0;
	TiXmlNode* p=NULL;

	if (node == m_RootElement)
	{
		layer = 0;
	}
	else
	{
		p = node;
		do {
			layer++;
			p = node->Parent();
		}while(p != m_RootElement);
	}
	return layer;
}



XMLConstruct::XMLConstruct()
{
	
}

XMLConstruct::~XMLConstruct()
{
	
}

void XMLConstruct::XMLConstructBegin(int charType)
{
	
	//创建一个XML的文档对象。
    m_myDocument = new TiXmlDocument();
	m_xmlDeclaration = new TiXmlDeclaration("1.0", "gbk", "");

	m_myDocument->SetCharType(charType);
    //创建一个根元素并连接。
    m_RootElement = new TiXmlElement("root");
	m_myDocument->LinkEndChild(m_xmlDeclaration);
    m_myDocument->LinkEndChild(m_RootElement);

	//此时m_RootElement就是根节点的指针
	m_parentElement[0] = m_RootElement;
	
	return;
}

void XMLConstruct::XMLConstructBegin(string ver, string character, string rootName,int charType)
{
	//创建一个XML的文档对象。
    m_myDocument = new TiXmlDocument();
	m_xmlDeclaration = new TiXmlDeclaration(ver.c_str(), character.c_str(), "");

	m_myDocument->SetCharType(charType);

    //创建一个根元素并连接。
    m_RootElement = new TiXmlElement(rootName.c_str());
	m_myDocument->LinkEndChild(m_xmlDeclaration);
    m_myDocument->LinkEndChild(m_RootElement);

	//此时m_RootElement就是根节点的指针
	m_parentElement[0] = m_RootElement;
	
	return;
}

void XMLConstruct::XMLConstructBeginSimple(string rootName, int charType)
{	
	//创建一个XML的文档对象。
    m_myDocument = new TiXmlDocument();

	m_myDocument->SetCharType(charType);
    //创建一个根元素并连接。
    m_RootElement = new TiXmlElement(rootName.c_str());
    m_myDocument->LinkEndChild(m_RootElement);
	m_parentElement[0] = m_RootElement;
	
	return;
}

int XMLConstruct::XMLConstructEnd(const char *XmlPath )
{
	INT32 ret;
    //保存到文件
    if( true == m_myDocument->SaveFile(XmlPath) )
	{		
		DBG_PRINT(("*************TRUE******************"));
		ret = SUCCESS;
	}
	else
	{
		DBG_PRINT(("*************FALSE******************"));
		ret = FAILURE;
	}
	
	if (NULL != m_myDocument)
	{
		delete m_myDocument;
		m_myDocument = NULL;
	}

	return ret;
}

void XMLConstruct::XMLConstructDelete( )
{	
	if (NULL != m_myDocument)
	{
		delete m_myDocument;
		m_myDocument = NULL;
	}
	return;
}

int XMLConstruct::AddNode( TiXmlElement *ParentElement, string nodeName )
{
	//创建一个名为nodeName的元素并连接到父节点上。
    m_NewElement = new TiXmlElement(nodeName.c_str());
    ParentElement->LinkEndChild(m_NewElement);
	//此时m_NewElement就是新节点的指针。

	return SUCCESS;
}

int XMLConstruct::InsertNode(TiXmlElement *pParent, TiXmlElement *pNode)
{
	if (pParent->InsertEndChild(*pNode) != NULL)
		return SUCCESS;
	else
		return FAILURE;
}

int XMLConstruct::InsertAllChildNode(TiXmlElement *pParent, TiXmlElement *pNode)
{
	if (pNode->Type() != TiXmlNode::ELEMENT)			//但是无法排除随机指针pNode的Type()值碰巧为0x01的情况
	{
		printf("!!! pNode is not a TiXmlElement point !! \n");	
		printf("Error in function \"InsertAllChildNode()\" \n");	
		exit(-1);
	}

	TiXmlElement* m_Child = pNode->FirstChildElement();
	
	while( m_Child != NULL )
	{
		if (pParent->InsertEndChild(*m_Child) == NULL)
			return FAILURE;
		
		m_Child = m_Child->NextSiblingElement();
	}
	
	return SUCCESS;
}

int XMLConstruct::AddText( string TextValue, int type )
{
	char *pout=NULL;
	string utf8Str("");

#if (USE_ICONV_LIB!=0)
	int ret;
	if(m_myDocument->m_charType == UTF8_CHAR_TYPE)
	{
		ret = code_convert("gbk", "utf-8", TextValue.c_str(), TextValue.length(), &pout);
		if (ret != 0)
		{
			DBG_PRINT(("GBK转utf8错误: %d", ret));
		}
		else
		{
			utf8Str.assign(pout);
		}
	}
	else
	{
		utf8Str = TextValue;
	}
	
#else
	utf8Str = TextValue;
#endif	//USE_ICONV_LIB


	//创建一个名为TextValue的元素并将其赋给节点。
    m_Content = new TiXmlText(utf8Str.c_str());
	if (type == XML_SET_TEXT_CDATA)
	{
		m_Content->SetCDATA(true);
	}
    m_NewElement->LinkEndChild(m_Content);
	//此时m_Content就是新的TextValue元素的指针。
	
	return SUCCESS;
}

int XMLConstruct::AddAttr( string AttrName, string AttrValue )
{    
    m_NewElement->SetAttribute(AttrName.c_str(), AttrValue.c_str());
	return SUCCESS;
}

#endif		//USE_XML_FUNC




