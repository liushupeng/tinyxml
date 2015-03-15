/* TiXmlNode类是它的派生类的基础类，方法对应着不同的子类，会有不同的含义 */
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

    #ifdef TIXML_USE_STL
  /** Return Value() as a std::string. If you only use STL,
      this is more efficient than calling Value().
    Only available in STL mode.
  */
  const std::string& ValueStr() const { return value; }
  #endif

  const TIXML_STRING& ValueTStr() const { return value; }

  /** Changes the value of the node. Defined as:
    @verbatim
    Document: filename of the xml file
    Element:  name of the element
    Comment:  the comment text
    Unknown:  the tag contents
    Text:   the text string
    @endverbatim
  */
  void SetValue(const char * _value) { value = _value;}
  
    /// Delete all the children of this node. Does not affect 'this'.
  void Clear();

  /// One step up the DOM.
  TiXmlNode* Parent()             { return parent; }
  const TiXmlNode* Parent() const       { return parent; }

  const TiXmlNode* FirstChild() const   { return firstChild; }  ///< The first child of this node. Will be null if there are no children.
  TiXmlNode* FirstChild()           { return firstChild; }
  const TiXmlNode* FirstChild( const char * value ) const;      ///< The first child of this node with the matching 'value'. Will be null if none found.
  /// The first child of this node with the matching 'value'. Will be null if none found.
  TiXmlNode* FirstChild( const char * _value ) {
    // Call through to the const version - safe since nothing is changed. Exiting syntax: cast this to a const (always safe)
    // call the method, cast the return back to non-const.
    return const_cast< TiXmlNode* > ((const_cast< const TiXmlNode* >(this))->FirstChild( _value ));
  }
  const TiXmlNode* LastChild() const  { return lastChild; }   /// The last child of this node. Will be null if there are no children.
  TiXmlNode* LastChild()  { return lastChild; }

  const TiXmlNode* LastChild( const char * value ) const;     /// The last child of this node matching 'value'. Will be null if there are no children.
  
   TiXmlNode* LastChild( const char * _value ) {
    return const_cast< TiXmlNode* > ((const_cast< const TiXmlNode* >(this))->LastChild( _value ));
  }

    #ifdef TIXML_USE_STL
  const TiXmlNode* FirstChild( const std::string& _value ) const  { return FirstChild (_value.c_str ());  } ///< STL std::string form.
  TiXmlNode* FirstChild( const std::string& _value )        { return FirstChild (_value.c_str ());  } ///< STL std::string form.
  const TiXmlNode* LastChild( const std::string& _value ) const { return LastChild (_value.c_str ()); } ///< STL std::string form.
  TiXmlNode* LastChild( const std::string& _value )       { return LastChild (_value.c_str ()); } ///< STL std::string form.
  #endif
  
    /** An alternate way to walk the children of a node.
    One way to iterate over nodes is:
    @verbatim
      for( child = parent->FirstChild(); child; child = child->NextSibling() )
    @endverbatim

    IterateChildren does the same thing with the syntax:
    @verbatim
      child = 0;
      while( child = parent->IterateChildren( child ) )
    @endverbatim

    IterateChildren takes the previous child as input and finds
    the next one. If the previous child is null, it returns the
    first. IterateChildren will return null when done.
  */
  const TiXmlNode* IterateChildren( const TiXmlNode* previous ) const;
  TiXmlNode* IterateChildren( const TiXmlNode* previous ) {
    return const_cast< TiXmlNode* >( (const_cast< const TiXmlNode* >(this))->IterateChildren( previous ) );
  }
  
    /// This flavor of IterateChildren searches for children with a particular 'value'
  const TiXmlNode* IterateChildren( const char * value, const TiXmlNode* previous ) const;
  TiXmlNode* IterateChildren( const char * _value, const TiXmlNode* previous ) {
    return const_cast< TiXmlNode* >( (const_cast< const TiXmlNode* >(this))->IterateChildren( _value, previous ) );
  }

    #ifdef TIXML_USE_STL
  const TiXmlNode* IterateChildren( const std::string& _value, const TiXmlNode* previous ) const  { return IterateChildren (_value.c_str (), previous); } ///< STL std::string form.
  TiXmlNode* IterateChildren( const std::string& _value, const TiXmlNode* previous ) {  return IterateChildren (_value.c_str (), previous); } ///< STL std::string form.
  #endif

  /** Add a new node related to this. Adds a child past the LastChild.
    Returns a pointer to the new object or NULL if an error occured.
  */
  TiXmlNode* InsertEndChild( const TiXmlNode& addThis );
  
    /** Add a new node related to this. Adds a child past the LastChild.

    NOTE: the node to be added is passed by pointer, and will be
    henceforth owned (and deleted) by tinyXml. This method is efficient
    and avoids an extra copy, but should be used with care as it
    uses a different memory model than the other insert functions.

    @sa InsertEndChild
  */
  TiXmlNode* LinkEndChild( TiXmlNode* addThis );

  /** Add a new node related to this. Adds a child before the specified child.
    Returns a pointer to the new object or NULL if an error occured.
  */
  TiXmlNode* InsertBeforeChild( TiXmlNode* beforeThis, const TiXmlNode& addThis );

  /** Add a new node related to this. Adds a child after the specified child.
    Returns a pointer to the new object or NULL if an error occured.
  */
  TiXmlNode* InsertAfterChild(  TiXmlNode* afterThis, const TiXmlNode& addThis );

  /** Replace a child of this node.
    Returns a pointer to the new object or NULL if an error occured.
  */
  TiXmlNode* ReplaceChild( TiXmlNode* replaceThis, const TiXmlNode& withThis );
  
   /// Delete a child of this node.
  bool RemoveChild( TiXmlNode* removeThis );

  /// Navigate to a sibling node.
  const TiXmlNode* PreviousSibling() const      { return prev; }
  TiXmlNode* PreviousSibling()            { return prev; }

  /// Navigate to a sibling node.
  const TiXmlNode* PreviousSibling( const char * ) const;
  TiXmlNode* PreviousSibling( const char *_prev ) {
    return const_cast< TiXmlNode* >( (const_cast< const TiXmlNode* >(this))->PreviousSibling( _prev ) );
  }
  
    /// Navigate to a sibling node.
  const TiXmlNode* NextSibling() const        { return next; }
  TiXmlNode* NextSibling()              { return next; }

  /// Navigate to a sibling node with the given 'value'.
  const TiXmlNode* NextSibling( const char * ) const;
  TiXmlNode* NextSibling( const char* _next ) {
    return const_cast< TiXmlNode* >( (const_cast< const TiXmlNode* >(this))->NextSibling( _next ) );
  }

  /** Convenience function to get through elements.
    Calls NextSibling and ToElement. Will skip all non-Element
    nodes. Returns 0 if there is not another element.
  */
  const TiXmlElement* NextSiblingElement() const;
  TiXmlElement* NextSiblingElement() {
    return const_cast< TiXmlElement* >( (const_cast< const TiXmlNode* >(this))->NextSiblingElement() );
  }

  /** Convenience function to get through elements.
    Calls NextSibling and ToElement. Will skip all non-Element
    nodes. Returns 0 if there is not another element.
  */
  const TiXmlElement* NextSiblingElement( const char * ) const;
  TiXmlElement* NextSiblingElement( const char *_next ) {
    return const_cast< TiXmlElement* >( (const_cast< const TiXmlNode* >(this))->NextSiblingElement( _next ) );
  }
  
    /// Convenience function to get through elements.
  const TiXmlElement* FirstChildElement() const;
  TiXmlElement* FirstChildElement() {
    return const_cast< TiXmlElement* >( (const_cast< const TiXmlNode* >(this))->FirstChildElement() );
  }

  /// Convenience function to get through elements.
  const TiXmlElement* FirstChildElement( const char * _value ) const;
  TiXmlElement* FirstChildElement( const char * _value ) {
    return const_cast< TiXmlElement* >( (const_cast< const TiXmlNode* >(this))->FirstChildElement( _value ) );
  }

    #ifdef TIXML_USE_STL
  const TiXmlElement* FirstChildElement( const std::string& _value ) const  { return FirstChildElement (_value.c_str ()); } ///< STL std::string form.
  TiXmlElement* FirstChildElement( const std::string& _value )        { return FirstChildElement (_value.c_str ()); } ///< STL std::string form.
  #endif

  /** Query the type (as an enumerated value, above) of this node.
    The possible types are: TINYXML_DOCUMENT, TINYXML_ELEMENT, TINYXML_COMMENT,
                TINYXML_UNKNOWN, TINYXML_TEXT, and TINYXML_DECLARATION.
  */
  int Type() const  { return type; }
  
    const TiXmlDocument* GetDocument() const;
  TiXmlDocument* GetDocument() {
    return const_cast< TiXmlDocument* >( (const_cast< const TiXmlNode* >(this))->GetDocument() );
  }

  /// Returns true if this node has no children.
  bool NoChildren() const           { return !firstChild; }

  virtual const TiXmlDocument*    ToDocument()    const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
  virtual const TiXmlElement*     ToElement()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
  virtual const TiXmlComment*     ToComment()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
  virtual const TiXmlUnknown*     ToUnknown()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
  virtual const TiXmlText*        ToText()        const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
  virtual const TiXmlDeclaration* ToDeclaration() const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.

  virtual TiXmlDocument*          ToDocument()    { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
  virtual TiXmlElement*           ToElement()     { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
  
    virtual TiXmlComment*           ToComment()     { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
  virtual TiXmlUnknown*           ToUnknown()     { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
  virtual TiXmlText*              ToText()        { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
  virtual TiXmlDeclaration*       ToDeclaration() { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.

  /** Create an exact duplicate of this node and return it. The memory must be deleted
    by the caller. 
  */
  virtual TiXmlNode* Clone() const = 0;
  
  protected:
  TiXmlNode( NodeType _type );

  // Copy to the allocated object. Shared functionality between Clone, Copy constructor,
  // and the assignment operator.
  void CopyTo( TiXmlNode* target ) const;

  #ifdef TIXML_USE_STL
      // The real work of the input operator.
  virtual void StreamIn( std::istream* in, TIXML_STRING* tag ) = 0;
  #endif

  // Figure out what is at *p, and parse it. Returns null if it is not an xml node.
  TiXmlNode* Identify( const char* start, TiXmlEncoding encoding );

  TiXmlNode*    parent;
  NodeType    type;

  TiXmlNode*    firstChild;
  TiXmlNode*    lastChild;

  TIXML_STRING  value;

  TiXmlNode*    prev;
  TiXmlNode*    next;

private:
  TiXmlNode( const TiXmlNode& );        // not implemented.
  void operator=( const TiXmlNode& base );  // not allowed.
};
