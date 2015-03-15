/* 类 */
class TiXmlAttribute : public TiXmlBase
{
  friend class TiXmlAttributeSet;

public:
  /* 构造函数 */
  TiXmlAttribute() : TiXmlBase()
  {
    document = 0;
    prev = next = 0;
  }
  TiXmlAttribute(const char * _name, const char * _value)
  {
    name = _name;
    value = _value;
    document = 0;
    prev = next = 0;
  }
  
  /* 获取value */
  const char* Name() const   { return name.c_str(); }
  const char* Value() const  { return value.c_str(); }
  int IntValue() const;          /* 如果value为int类型，可以进行转换 */
  double DoubleValue() const;    /* 如果value为double类型，可以进行转换 */

  /* 更安全的获取value的方法。成功返回TIXML_SUCCESS，失败返回TIXML_WRONG_TYPE */
  int QueryIntValue( int* _value ) const;
  int QueryDoubleValue( double* _value ) const;
  
  /* 从DOM中获取下一下相邻的属性 */
  const TiXmlAttribute* Next() const;
  TiXmlAttribute* Next()
  {
    return const_cast< TiXmlAttribute* >( (const_cast< const TiXmlAttribute* >(this))->Next());
  }
  /* 从DOM中获取上一个相邻的属性 */
  const TiXmlAttribute* Previous() const;
  TiXmlAttribute* Previous()
  {
    return const_cast< TiXmlAttribute* >( (const_cast< const TiXmlAttribute* >(this))->Previous() );
  }
  
  /* 实现基类中声明的纯虚函数 */
  virtual const char* Parse( const char* p, TiXmlParsingData* data, TiXmlEncoding encoding );
  virtual void Print( FILE* cfile, int depth ) const 
  {
    Print( cfile, depth, 0 );
  }
  void Print( FILE* cfile, int depth, TIXML_STRING* str ) const;
  
private:
  /* 不需要具体实现的函数*/
  TiXmlAttribute( const TiXmlAttribute& );
  void operator=( const TiXmlAttribute& base );

  /* 指向document的指针，为了方便返回错误信息 */
  TiXmlDocument*  document;
  
  /* 数据成员 */
  TIXML_STRING name;
  TIXML_STRING value;
  TiXmlAttribute* prev;
  TiXmlAttribute* next;
};

/* 方法 */

/* 将类似于 name=test的内容以key-value的形式解析出来 */
const char* TiXmlAttribute::Parse( const char* p, TiXmlParsingData* data, TiXmlEncoding encoding )
{
  p = SkipWhiteSpace( p, encoding );
  if ( !p || !*p ) return 0;

  if ( data )
  {
    data->Stamp( p, encoding );
    location = data->Cursor();
  }
  // Read the name, the '=' and the value.
  const char* pErr = p; 
  p = ReadName( p, &name, encoding );
  if ( !p || !*p )
  {
    if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES, pErr, data, encoding );
    return 0;
  }
  p = SkipWhiteSpace( p, encoding );
  if ( !p || !*p || *p != '=' )
  {
    if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES, p, data, encoding );
    return 0;
  }
    ++p;  // skip '='
  p = SkipWhiteSpace( p, encoding );
  if ( !p || !*p )
  {
    if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES, p, data, encoding );
    return 0;
  }

  const char* end;
  const char SINGLE_QUOTE = '\'';
  const char DOUBLE_QUOTE = '\"';

  if ( *p == SINGLE_QUOTE )
  {
    ++p;
    end = "\'";   // single quote in string
    p = ReadText( p, &value, false, end, false, encoding );
  }
  else if ( *p == DOUBLE_QUOTE )
  {
    ++p;
    end = "\"";   // double quote in string
    p = ReadText( p, &value, false, end, false, encoding );
  }
    else
  {
    value = "";
    while (p && *p && !IsWhiteSpace(*p ) && *p != '/' && *p != '>' )
    {
      if ( *p == SINGLE_QUOTE || *p == DOUBLE_QUOTE )
      {
        if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES, p, data, encoding );
        return 0;
      }
      value += *p;
      ++p;
    }
  }
  return p;
}

/* 更安全的一种方法(未必) */
int TiXmlAttribute::QueryIntValue( int* ival ) const
{
  if ( TIXML_SSCANF( value.c_str(), "%d", ival ) == 1 )
    return TIXML_SUCCESS;
  return TIXML_WRONG_TYPE;
}
int TiXmlAttribute::QueryDoubleValue( double* dval ) const
{
  if ( TIXML_SSCANF( value.c_str(), "%lf", dval ) == 1 )
    return TIXML_SUCCESS;
  return TIXML_WRONG_TYPE;
}

/* Print()函数的实现 */
void TiXmlAttribute::Print( FILE* cfile, int /*depth*/, TIXML_STRING* str ) const
{
  TIXML_STRING n, v;

  EncodeString( name, &n );
  EncodeString( value, &v );

  if (value.find ('\"') == TIXML_STRING::npos) /* TIXML_STRING::npos 表示“未找到”，值为static const unsigned -1 */
  {
    if ( cfile ) 
    {
      fprintf (cfile, "%s=\"%s\"", n.c_str(), v.c_str() );
    }
    if ( str ) 
    {
      (*str) += n; (*str) += "=\""; (*str) += v; (*str) += "\"";
    }
  }
  else {
    if ( cfile ) 
    {
      fprintf (cfile, "%s='%s'", n.c_str(), v.c_str() );
    }
    if ( str ) 
    {
      (*str) += n; (*str) += "='"; (*str) += v; (*str) += "'";
    }
  }
}
