/* 类 */
class TiXmlDocument : public TiXmlNode
{
public:
  /* 构造函数 */
  TiXmlDocument();
  TiXmlDocument( const char * documentName );

  TiXmlDocument( const TiXmlDocument& copy );
  TiXmlDocument& operator=( const TiXmlDocument& copy );

  virtual ~TiXmlDocument() {}

  bool LoadFile( TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING );
  bool SaveFile() const;
  bool LoadFile( const char * filename, TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING );
  bool SaveFile( const char * filename ) const;
  bool LoadFile( FILE*, TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING );
  bool SaveFile( FILE* ) const;

  /* 将文件数据解析成树状图 */
  virtual const char* Parse( const char* p, TiXmlParsingData* data = 0, TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING );
  
  const TiXmlElement* RootElement() const { return FirstChildElement(); }
  TiXmlElement* RootElement() { return FirstChildElement(); }

  bool Error() const { return error; }
  const char * ErrorDesc() const { return errorDesc.c_str (); }
  int ErrorId() const       { return errorId; }

  int ErrorRow() const { return errorLocation.row+1; }
  int ErrorCol() const { return errorLocation.col+1; }
  
  void SetTabSize( int _tabsize ) { tabsize = _tabsize; }
  int TabSize() const { return tabsize; }

  void ClearError()
  {
    error = false;
    errorId = 0;
    errorDesc = "";
    errorLocation.row = errorLocation.col = 0;
  }

  /** Write the document to standard out using formatted printing ("pretty print"). */
  void Print() const { Print( stdout, 0 ); }
  
  /* [internal use] */
  void SetError( int err, const char* errorLocation, TiXmlParsingData* prevData, TiXmlEncoding encoding );
  virtual bool Accept( TiXmlVisitor* content ) const;

protected:
  virtual TiXmlNode* Clone() const;

private:
  void CopyTo( TiXmlDocument* target ) const;

  bool error;
  int  errorId;
  TIXML_STRING errorDesc;
  int  tabsize;
  TiXmlCursor errorLocation;
  bool useMicrosoftBOM;
};

/* 方法 */

/* 从文件中使用fread()函数一次性读取全部内容，依次遍历，将其中的'\r'或'\r\n'转换为'\n'
 * 并用Parse()函数解析出内容
 */
bool TiXmlDocument::LoadFile( FILE* file, TiXmlEncoding encoding )
{
  if ( !file ) 
  {     
    SetError( TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN );
    return false;
  }     

  Clear();
  location.Clear();

  /* 获取文件大小，便于一次性分配足够空间存储 */
  long length = 0;
  fseek( file, 0, SEEK_END );
  length = ftell( file );
  fseek( file, 0, SEEK_SET );

  if (length <= 0 )
  {
    SetError( TIXML_ERROR_DOCUMENT_EMPTY, 0, 0, TIXML_ENCODING_UNKNOWN );
    return false;
  }
  
  /* 一种存在bug的获取数据的方法，使用fread()一次性获取避免bug的产生
  while( fgets( buf, sizeof(buf), file ) )
  {
    data += buf;
  }
  */

  char* buf = new char[ length+1 ];
  buf[0] = 0;

  if ( fread( buf, length, 1, file ) != 1 ) 
  {
    delete [] buf;
    SetError( TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN );
    return false;
  }

  const char* p = buf;  // the read head
  char* q = buf;      // the write head
  const char CR = 0x0d;
  const char LF = 0x0a;

  buf[length] = 0;
  while( *p ) 
  {
    assert( p < (buf+length) );
    assert( q <= (buf+length) );
    assert( q <= p );

    if ( *p == CR ) 
    {
      *q++ = LF;
      p++;
      if ( *p == LF ) 
      {
        p++;
      }
    }
    else 
    {
      *q++ = *p++;
    }
  }
  assert( q <= (buf+length) );
  *q = 0;

  Parse( buf, 0, encoding );

  delete [] buf;
  return !Error();
}

/* 将所有内容连接成树状图 */
const char* TiXmlDocument::Parse( const char* p, TiXmlParsingData* prevData, TiXmlEncoding encoding )
{
  ClearError();

  if ( !p || !*p )
  {
    SetError( TIXML_ERROR_DOCUMENT_EMPTY, 0, 0, TIXML_ENCODING_UNKNOWN );
    return 0;
  }

  location.Clear();
  if ( prevData )
  {
    location.row = prevData->cursor.row;
    location.col = prevData->cursor.col;
  }
  else 
  {
    location.row = 0; 
    location.col = 0; 
  }
  TiXmlParsingData data( p, TabSize(), location.row, location.col );
  location = data.Cursor();
  
  p = SkipWhiteSpace( p, encoding );
  if ( !p )
  {   
    SetError( TIXML_ERROR_DOCUMENT_EMPTY, 0, 0, TIXML_ENCODING_UNKNOWN );
    return 0;
  } 
  
  while ( p && *p )
  { 
    /* 这块是形成树状图的关键位置 */
    TiXmlNode* node = Identify( p, encoding );
    if ( node )
    {
      p = node->Parse( p, &data, encoding );
      LinkEndChild( node );
    }
    else
    {
      break;
    }

    /* Did we get encoding info? */
    if (encoding == TIXML_ENCODING_UNKNOWN && node->ToDeclaration() )
    {
      TiXmlDeclaration* dec = node->ToDeclaration();
      const char* enc = dec->Encoding();
      assert( enc );

      if ( *enc == 0 )
        encoding = TIXML_ENCODING_UTF8;
      else if ( StringEqual( enc, "UTF-8", true, TIXML_ENCODING_UNKNOWN ) )
        encoding = TIXML_ENCODING_UTF8;
      else if ( StringEqual( enc, "UTF8", true, TIXML_ENCODING_UNKNOWN ) )
        encoding = TIXML_ENCODING_UTF8; // incorrect, but be nice
      else
        encoding = TIXML_ENCODING_LEGACY;
    }
    p = SkipWhiteSpace( p, encoding );
  }

  if ( !firstChild ) {
    SetError( TIXML_ERROR_DOCUMENT_EMPTY, 0, 0, encoding );
    return 0;
  }

  return p;
}
    
