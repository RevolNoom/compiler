#!/usr/bin/bash

# GENERATE CLASSES DECLARATIONS
# Called from GenEverything.bash

# Abstract Class Token

TOKEN_TYPES=(Program Const Type Var Char Integer\
            Array Of Function Procedure Begin End Call If Then\
            Else While Do For To Space Semicolon Colon Period Comma\
            Assign Equal Unequal Less LessEqual Greater GreaterEqual\
            Plus Minus Times Slash ParenthesisOpen ParenthesisClose\
            ArrayIndexOpen ArrayIndexClose)

echo "/*"
echo "  THIS FILE IS GENERATED. ANY CHANGES WILL BE LOST"
echo "  GENERATED BY: $(realpath $0)"
echo
echo "  PROTOTYPE + VISITOR"
echo "  A uniform interface of all KPL token "
echo "*/"
echo "#ifndef TOKEN_HPP"
echo "#define TOKEN_HPP"
echo
echo "#include <list>"
echo "#include <memory>"
echo "#include <string>"
echo
echo "using namespace std;"
echo
echo "namespace TokenType"
echo "{"
echo "    class Token;"
echo "}"
echo
echo "using TokenStream = list<shared_ptr<TokenType::Token>>;"
echo
echo "namespace TokenType"
echo "{"
echo "	class Token"
echo "	{"
echo "	public:"
echo
echo "	    int GetCol()  {return _col;}"
echo "	    int GetLine() {return _line;}"
echo
echo "	    virtual string GetTokenType() const = 0;"
echo
echo "	    // PROTOTYPE"
echo "	    // Return a new token of current type on the specified line and column "
echo "	    virtual shared_ptr<Token> Clone(int line, int col) const = 0;"
echo
echo "	protected:"
echo "	    Token(int line, int col):_line(line), _col(col)"
echo "	    {"
echo "	    }"
echo
echo "	private:"
echo "	    int _line, _col;"
echo "	};"
echo
echo
echo
echo



for type in ${TOKEN_TYPES[@]} 
do
    echo "	class" $type": public Token"
    echo "	{"
    echo "	public:"

    # This line is for Assignment week 1. They are practically useless
	echo "	  string GetTokenType() const override	{return \"$type\";}"
	#echo "      string ToString() const override;"
    echo "      virtual shared_ptr<Token> Clone(int line, int col) const override;"
    echo "      $type(int line, int col);"
    echo "	};"
    echo

    i=$(($i+1))
done




# TOKENS THAT NEED SPECIAL TREATMENT

SPEC_TOK=(Identifier Number Letter)
for TYPE in ${SPEC_TOK[@]}
do
    echo "	class $TYPE: public Token"
    echo "	{"
    echo "	public:"
	echo "	  string GetTokenType() const override	{return \"$TYPE\";}"
    echo "	  string ToString() const;"
    echo "	  virtual shared_ptr<Token> Clone(int line, int col) const;"

    if [ $TYPE == "Identifier" ]
    then
        echo "	  Identifier(int line, int col, string id);"
        echo
        echo "	private:"
        echo "	  string _id;"

    elif [ $TYPE == "Number" ]
    then
        echo "	  Number(int line, int col, int value);"
        echo
        echo "	private:"
        echo "      int _value;"

    elif [ $TYPE == "Letter" ]
    then
        echo "	  Letter(int line, int col, int charcode);"
        echo
        echo "	private:"
        echo "      int _charcode;"
    fi

    echo "	};"
    echo
done


echo "} /*namespace Token*/"
echo 
echo "#endif /*TOKEN_HPP*/"