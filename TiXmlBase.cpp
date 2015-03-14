/* 类 */
class TiXmlBase
{
  friend class TiXmlNode;
  friend class TiXmlElement;
  friend class TiXmlDocument;

public:
  TiXmlBase() : userData(0) {}
  virtual ~TiXmlBase()      {}
  
  /* 打印节点信息。这是一个纯虚函数，继承TiXmlBase的每个子类都必须实现它
   * 含有纯虚拟函数的类称为抽象类，它不能生成对象。因此TiXmlBase调用自身的成员函数的时候需要直接使用类名：
   * TiXmlBase::Print()
   */
  virtual void Print( FILE* cfile, int depth ) const = 0;

  /* 当XML中的内容有多个空格时，是全部显示还是只显示一个？这个选项是由condenseWhiteSpace来控制的
   * 默认是对多个空格进行合并。对condenseWhiteSpace的修改不是多线程安全的。
   * 如：<school>沈  阳  工  业  大  学</school>实际显示为<school>沈 阳 工 业 大 学</school>
   * 这两个函数是静态成员函数，因此condenseWhiteSpace一定是静态数据成员，它是由所有子类共享的
   */
  static void SetCondenseWhiteSpace( bool condense )  { condenseWhiteSpace = condense; }
  static bool IsWhiteSpaceCondensed() { return condenseWhiteSpace; }

  /* 通过游标类TiXmlCursor获得当前游标所在的行、列 */
  int Row() const     { return location.row + 1; }
  int Column() const  { return location.col + 1; }
  
  /* 获得指向用户数据的指针 */
  void  SetUserData( void* user )  { userData = user; }
  void* GetUserData()              { return userData; }
  const void* GetUserData() const  { return userData; }

  /* 存储与编码相关的信息 */
  static const int utf8ByteTable[256];
  
  /* 这也是个纯虚函数，用来解析实际信息。如TiXmlDeclaration::Parse()就是用来解析XML的声明 */
  virtual const char* Parse(const char* p, TiXmlParsingData* data, TiXmlEncoding encoding /*= TIXML_ENCODING_UNKNOWN */) = 0;
  
  /* 这是用来对字符串编码的。如果字符串里有不可见字符，需进行特殊处理。有<、>等字符，转换成&lt;和&gt;等
   * 就是把特殊字符转化为实体引用
   */
  static void EncodeString( const TIXML_STRING& str, TIXML_STRING* out );
  
  /* 这类似于#indef，从0开始定义错误码 */
  enum
  {
    TIXML_NO_ERROR = 0,
    TIXML_ERROR,
    TIXML_ERROR_OPENING_FILE,
    TIXML_ERROR_PARSING_ELEMENT,
    TIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME,
    TIXML_ERROR_READING_ELEMENT_VALUE,
    TIXML_ERROR_READING_ATTRIBUTES,
    TIXML_ERROR_PARSING_EMPTY,
    TIXML_ERROR_READING_END_TAG,
    TIXML_ERROR_PARSING_UNKNOWN,
    TIXML_ERROR_PARSING_COMMENT,
    TIXML_ERROR_PARSING_DECLARATION,
    TIXML_ERROR_DOCUMENT_EMPTY,
    TIXML_ERROR_EMBEDDED_NULL,
    TIXML_ERROR_PARSING_CDATA,
    TIXML_ERROR_DOCUMENT_TOP_ONLY,
    
    TIXML_ERROR_STRING_COUNT
  };
	
protected:
  /* 跳过空格 */
  static const char* SkipWhiteSpace( const char*, TiXmlEncoding encoding );
  inline static bool IsWhiteSpace( char c )		
  {
    return ( isspace( (unsigned char) c ) || c == '\n' || c == '\r' ); 
  }
  inline static bool IsWhiteSpace( int c )
  {
    if ( c < 256 )
      return IsWhiteSpace( (char) c );
    return false;
  }

  #ifdef TIXML_USE_STL
  static bool StreamWhiteSpace( std::istream * in, TIXML_STRING * tag );
  static bool StreamTo( std::istream * in, int character, TIXML_STRING * tag );
  #endif

  /* 从给的字符串中读取名字，读取到的内容放到name中。返回值指向名字最后一个字符的下一个位置 */
  static const char* ReadName( const char* p, TIXML_STRING* name, TiXmlEncoding encoding );
  
  /* 输入：in
   * 输出：text
   * 参数：trimWhiteSpace - 是否修剪多余的空格，作用与condenseWhiteSpace一致
   *       endTag - XML的结束标识符
   *       ignoreCase - 不区分大小写
   * 返回值：XML的结束标识符的下一个位置
   */
  static const char* ReadText(const char* in, TIXML_STRING* text, bool trimWhiteSpace, const char* endTag, bool ignoreCase, TiXmlEncoding encoding );	
  
  // If an entity has been found, transform it into a character.
  /* 实体引用
     在XML中，一些字符拥有特殊的意义。
     如果你把字符 "<" 放在 XML 元素中，会发生错误，这是因为解析器会把它当作新元素的开始。为了避免类似错误，用实体引用来代替 "<" 字符
     在 XML 中，有 5 个预定义的实体引用：
     &lt;        <           小于
     &gt;        >           大于
     &amp;       &           与号
     &apos;      '           单引号
     &quot;      "           引号
     
     GetEntity()的作用就是如果找到了实体引用，就将这个实体引用转化为实际对应的字符
     返回值是指向实体引用的下一个位置
   */
  static const char* GetEntity(const char* in, char* value, int* length, TiXmlEncoding encoding );
  
  /* 如果是一个普通字符，返回；如果是一个实体引用，将它转换为对应的字符后返回 (UTF-8d的字符编码返回的length比较长)*/
  inline static const char* GetChar(const char* p, char* _value, int* length, TiXmlEncoding encoding )
  {
    /* 具体实现代码 ... */
  }
  
  /* 如果p后紧跟的内容包含着endTag，返回true */
  static bool StringEqual(const char* p, const char* endTag, bool ignoreCase, TiXmlEncoding encoding );
  
  /* 包含着错误码与错误信息的对应关系 */
  static const char* errorString[ TIXML_ERROR_STRING_COUNT ];
  
  /* 游标信息 */
  TiXmlCursor location;
  
  /* 指向用户数据的指针 */
  void*  userData;
  
  /* 英文字母？*/
  static int IsAlpha(unsigned char anyByte, TiXmlEncoding encoding );
  /* 英文字母或阿拉伯数字？*/
  static int IsAlphaNum(unsigned char anyByte, TiXmlEncoding encoding );
  inline static int ToLower( int v, TiXmlEncoding encoding )
  {
    /* 具体代码实现 ... */
  }
  static void ConvertUTF32ToUTF8( unsigned long input, char* output, int* length );
  
private:
  TiXmlBase( const TiXmlBase& );	/* 没有实施 */
  void operator=( const TiXmlBase& base ); /* 不被允许使用 */
  
  /* 引用实体结构 */
  struct Entity
  {
    const char*   str;        /* 引用实体字符串 */
    unsigned int  strLength;  /* 引用实体字符串长度 */
    char          chr;        /* 对应的实际字符 */
  };
  enum
  {
    NUM_ENTITY = 5,
    MAX_ENTITY_LENGTH = 6
  };
  static Entity entity[ NUM_ENTITY ];
  
  static bool condenseWhiteSpace;
};

/* 方法 */
const char* TiXmlBase::errorString[ TiXmlBase::TIXML_ERROR_STRING_COUNT ] = 
{
  "No error",
  "Error",
  "Failed to open file",
  "Error parsing Element.",
  "Failed to read Element name",
  "Error reading Element value.",
  "Error reading Attributes.",
  "Error: empty tag.",
  "Error reading end tag.",
  "Error parsing Unknown.",
  "Error parsing Comment.",
  "Error parsing Declaration.",
  "Error document empty.",
  "Error null (0) or unexpected EOF found in input stream.",
  "Error parsing CDATA.",
  "Error when TiXmlDocument added to document, because TiXmlDocument can only be at the root.",
};

void TiXmlBase::EncodeString( const TIXML_STRING& str, TIXML_STRING* outString )
{
  int i=0;

  while( i<(int)str.length() )
  {     
    unsigned char c = (unsigned char) str[i];

    if (c == '&' && i < ( (int)str.length() - 2 ) && str[i+1] == '#' && str[i+2] == 'x') 
    {
      while ( i<(int)str.length()-1 )
      {                 
        outString->append( str.c_str() + i, 1 );
        ++i;
        if ( str[i] == ';' )
          break;
      }
    }
    else if ( c == '&' )
    {
      /* 这个顺序必须与tinyxmlparser.cpp:43的TiXmlBase::Entity TiXmlBase::entity()一致 */
      outString->append( entity[0].str, entity[0].strLength );
      ++i;
    }
    else if ( c == '<' )
    {
      outString->append( entity[1].str, entity[1].strLength );
      ++i;
    }
    else if ( c == '>' )
    {
      outString->append( entity[2].str, entity[2].strLength );
      ++i;
    }
    else if ( c == '\"' )
    {
      outString->append( entity[3].str, entity[3].strLength );
      ++i;
    }
    else if ( c == '\'' )
    {
      outString->append( entity[4].str, entity[4].strLength );
      ++i;
    }
    else if ( c < 32 )
    {
      char buf[ 32 ];

      #if defined(TIXML_SNPRINTF)   
        TIXML_SNPRINTF( buf, sizeof(buf), "&#x%02X;", (unsigned) ( c & 0xff ) );
      #else
        sprintf( buf, "&#x%02X;", (unsigned) ( c & 0xff ) );
      #endif

      outString->append( buf, (int)strlen( buf ) );
      ++i;
    }
    else
    {
      //char realc = (char) c;
      //outString->append( &realc, 1 );
      *outString += (char) c; // somewhat more efficient function call.
      ++i;
    }
  }
}

const char* TiXmlBase::ReadName( const char* p, TIXML_STRING * name, TiXmlEncoding encoding )
{
  /* 有的编译器下不支持name->clear()这种模式，所以用直接赋值的方式 */
  *name = "";

  /* name的命名方式：以下划线(_)和英文字母开头，其他字符可以是数字、字母、'_'、'.'和':' */
  if (p && *p && (IsAlpha( (unsigned char) *p, encoding ) || *p == '_' ))
  {
    const char* start = p; 
    while(p && *p &&  (IsAlphaNum((unsigned char ) *p, encoding) || *p == '_' || *p == '-' || *p == '.' || *p == ':' ))
    {
      ++p;
    }
    if ( p-start > 0 ) 
    {
      name->assign(start, p-start );
    }
    return p;
  }
  return 0;
}
