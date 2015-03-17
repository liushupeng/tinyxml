/* TiXmlNode类是它的派生类的基础类，方法对应着不同的子类，会有不同的含义 
 * 个人理解，它就像Animal类，派生出来的子类是Dog, Bird, Cat
 */
class TiXmlNode : public TiXmlBase
{
  friend class TiXmlDocument;
  friend class TiXmlElement;
public:
  /** The types of XML nodes supported by TinyXml. (All the
      unsupported types are picked up by UNKNOWN.)
  */
  enum NodeType
  {
    TINYXML_DOCUMENT,
    TINYXML_ELEMENT,
    TINYXML_COMMENT,
    TINYXML_UNKNOWN,
    TINYXML_TEXT,
    TINYXML_DECLARATION,
    TINYXML_TYPECOUNT
  };

  virtual ~TiXmlNode();
  
  const char *Value() const { return value.c_str (); }
  const TIXML_STRING& ValueTStr() const { return value; }

  /*Changes the value of the node. (不同的子类，对应的value是不同的)
    Defined as:
    Document: filename of the xml file
    Element:  name of the element
    Comment:  the comment text
    Unknown:  the tag contents
    Text:   the text string
  */
  void SetValue(const char * _value) { value = _value;}
  
  /* 删除当前节点所有的子节点，但是对当前节点没有影响 */
  void Clear();

  /* 返回父节点 */
  TiXmlNode* Parent() { return parent; }
  const TiXmlNode* Parent() const { return parent; }
  
  /* 返回第一个孩子结点 */
  const TiXmlNode* FirstChild() const { return firstChild; }
  TiXmlNode* FirstChild() { return firstChild; }
  
  /* 返回匹配value的第一个孩子结点 */
  const TiXmlNode* FirstChild( const char * value ) const;
  TiXmlNode* FirstChild( const char * _value ) 
  {
    return const_cast< TiXmlNode* > ((const_cast< const TiXmlNode* >(this))->FirstChild( _value ));
  }
  
  /* 返回最后一个孩子结点 */
  const TiXmlNode* LastChild() const  { return lastChild; } 
  TiXmlNode* LastChild()  { return lastChild; }
  
  /* 返回匹配value的最后一个孩子结点 */
  const TiXmlNode* LastChild( const char * value ) const; 
  TiXmlNode* LastChild( const char * _value )
  {
    return const_cast< TiXmlNode* > ((const_cast< const TiXmlNode* >(this))->LastChild( _value ));
  }
  
  /* 一种遍历孩子结点的方法：
   * for( child = parent->FirstChild(); child; child = child->NextSibling())
   * 
   * IterateChildren()也是做同样的事，使用方式如下：
   * while( child = parent->IterateChildren( child ) )
   * 它使用前一个孩子节点作为输入，通过这个节点，找到下一个。如果前一个节点是NULL，它会返回第一个节点
   */
  const TiXmlNode* IterateChildren( const TiXmlNode* previous ) const;
  TiXmlNode* IterateChildren( const TiXmlNode* previous ) 
  {
    return const_cast< TiXmlNode* >( (const_cast< const TiXmlNode* >(this))->IterateChildren( previous ) );
  }
  
  /* 遍历孩子结点的时候，需要匹配value值 */
  const TiXmlNode* IterateChildren( const char * value, const TiXmlNode* previous ) const;
  TiXmlNode* IterateChildren( const char * _value, const TiXmlNode* previous ) 
  {
    return const_cast< TiXmlNode* >((const_cast< const TiXmlNode* >(this))->IterateChildren( _value, previous ));
  }

  /* 将当前节点加入到子节点末尾的两种方法，参数类型不同而已 */
  TiXmlNode* InsertEndChild( const TiXmlNode& addThis );
  TiXmlNode* LinkEndChild( TiXmlNode* addThis );

  /* 在当前节点前面加入子节点 */
  TiXmlNode* InsertBeforeChild( TiXmlNode* beforeThis, const TiXmlNode& addThis );
  /* 在当前节点后面加入子节点 */
  TiXmlNode* InsertAfterChild(  TiXmlNode* afterThis, const TiXmlNode& addThis );

  /* 替换当前子节点 */
  TiXmlNode* ReplaceChild( TiXmlNode* replaceThis, const TiXmlNode& withThis );
  /* 删除当前子节点 */
  bool RemoveChild( TiXmlNode* removeThis );

  /* 同级的前一个节点 */
  const TiXmlNode* PreviousSibling() const { return prev; }
  TiXmlNode* PreviousSibling() { return prev; }

  const TiXmlNode* PreviousSibling( const char * ) const;
  TiXmlNode* PreviousSibling( const char *_prev ) 
  {
    return const_cast< TiXmlNode* >( (const_cast< const TiXmlNode* >(this))->PreviousSibling( _prev ) );
  }
  
  /* 同级的下一个节点 */
  const TiXmlNode* NextSibling() const { return next; }
  TiXmlNode* NextSibling() { return next; }

  const TiXmlNode* NextSibling( const char * ) const;
  TiXmlNode* NextSibling( const char* _next ) {
    return const_cast< TiXmlNode* >( (const_cast< const TiXmlNode* >(this))->NextSibling( _next ) );
  }

  /* 下一个同级元素 */
  const TiXmlElement* NextSiblingElement() const;
  TiXmlElement* NextSiblingElement()
  {
    return const_cast< TiXmlElement* >((const_cast< const TiXmlNode* >(this))->NextSiblingElement());
  }
  
  const TiXmlElement* NextSiblingElement( const char * ) const;
  TiXmlElement* NextSiblingElement( const char *_next ) 
  {
    return const_cast< TiXmlElement* >( (const_cast< const TiXmlNode* >(this))->NextSiblingElement( _next ) );
  }
  
  const TiXmlElement* FirstChildElement() const;
  TiXmlElement* FirstChildElement() 
  {
    return const_cast< TiXmlElement* >( (const_cast< const TiXmlNode* >(this))->FirstChildElement() );
  }

  const TiXmlElement* FirstChildElement( const char * _value ) const;
  TiXmlElement* FirstChildElement( const char * _value ) 
  {
    return const_cast< TiXmlElement* >( (const_cast< const TiXmlNode* >(this))->FirstChildElement( _value ) );
  }

  /* 可选值列表：
   * TINYXML_DOCUMENT, 
   * TINYXML_ELEMENT, 
   * TINYXML_COMMENT,
   * TINYXML_UNKNOWN, 
   * TINYXML_TEXT, 
   *TINYXML_DECLARATION.
   */
  int Type() const { return type; }
  
  const TiXmlDocument* GetDocument() const;
  TiXmlDocument* GetDocument()
  {
    return const_cast< TiXmlDocument* >( (const_cast< const TiXmlNode* >(this))->GetDocument() );
  }

  /* 判断是否存在子节点 */
  bool NoChildren() const { return !firstChild; }
  
  /* 类型转换 */
  virtual const TiXmlDocument*    ToDocument()    const { return 0; }
  virtual const TiXmlElement*     ToElement()     const { return 0; }
  virtual const TiXmlComment*     ToComment()     const { return 0; } 
  virtual const TiXmlUnknown*     ToUnknown()     const { return 0; } 
  virtual const TiXmlText*        ToText()        const { return 0; } 
  virtual const TiXmlDeclaration* ToDeclaration() const { return 0; } 

  virtual TiXmlDocument*          ToDocument()    { return 0; } 
  virtual TiXmlElement*           ToElement()     { return 0; } 
  
  virtual TiXmlComment*           ToComment()     { return 0; } 
  virtual TiXmlUnknown*           ToUnknown()     { return 0; }
  virtual TiXmlText*              ToText()        { return 0; }
  virtual TiXmlDeclaration*       ToDeclaration() { return 0; } 

  /* 克隆函数 */
  virtual TiXmlNode* Clone() const = 0;
  
  protected:
  TiXmlNode( NodeType _type);

  void CopyTo( TiXmlNode* target ) const;

  /* 验证当前节点是否符合XML格式 */
  TiXmlNode* Identify( const char* start, TiXmlEncoding encoding );

  /* 数据成员 */
  TiXmlNode*  parent;
  NodeType    type;

  TiXmlNode*  firstChild;
  TiXmlNode*  lastChild;

  TIXML_STRING  value;

  TiXmlNode*  prev;
  TiXmlNode*  next;

private:
  /* 拷贝构造函数和复制运算符不允许调用 */
  TiXmlNode( const TiXmlNode& );
  void operator=( const TiXmlNode& base ); 
};

/* 方法 */

/* 根据输入的字符串判断当前节点的类型 */
TiXmlNode* TiXmlNode::Identify( const char* p, TiXmlEncoding encoding )
{
  TiXmlNode* returnNode = 0;

  p = SkipWhiteSpace( p, encoding );
  if( !p || !*p || *p != '<' )
  {
    return 0;
  }

  p = SkipWhiteSpace( p, encoding );

  if ( !p || !*p )
  {
    return 0;
  }

  const char* xmlHeader = { "<?xml" };       /* 声明 */
  const char* commentHeader = { "<!--" };    /* 注释 */
  const char* dtdHeader = { "<!" };          /* 未知类型 */
  const char* cdataHeader = { "<![CDATA[" };
  
  if ( StringEqual( p, xmlHeader, true, encoding ) )
  {
    #ifdef DEBUG_PARSER
      TIXML_LOG( "XML parsing Declaration\n" );
    #endif
    returnNode = new TiXmlDeclaration();
  }
  else if ( StringEqual( p, commentHeader, false, encoding ) )
  {
    #ifdef DEBUG_PARSER
      TIXML_LOG( "XML parsing Comment\n" );
    #endif
    returnNode = new TiXmlComment();
  }
  else if ( StringEqual( p, cdataHeader, false, encoding ) )
  {
    #ifdef DEBUG_PARSER
      TIXML_LOG( "XML parsing CDATA\n" );
    #endif
    TiXmlText* text = new TiXmlText( "" );
    text->SetCDATA( true );
    returnNode = text;
  }
  else if ( StringEqual( p, dtdHeader, false, encoding ) )
  {
    #ifdef DEBUG_PARSER
      TIXML_LOG( "XML parsing Unknown(1)\n" );
    #endif
    returnNode = new TiXmlUnknown();
  }
  else if (IsAlpha( *(p+1), encoding ) || *(p+1) == '_' )
  {
    #ifdef DEBUG_PARSER
      TIXML_LOG( "XML parsing Element\n" );
    #endif
    returnNode = new TiXmlElement( "" );
  }
  else
  {
    #ifdef DEBUG_PARSER
      TIXML_LOG( "XML parsing Unknown(2)\n" );
    #endif
    returnNode = new TiXmlUnknown();
  }

  if (returnNode)
  {
    // Set the parent, so it can report errors
    returnNode->parent = this;
  }
  return returnNode;
}

