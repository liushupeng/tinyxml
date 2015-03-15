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
