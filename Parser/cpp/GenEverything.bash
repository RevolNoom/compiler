#!/usr/bin/bash

TOKEN_TYPES=(Program Const Type Var Integer\
            Char Array Of Function Procedure Begin End Call If Then\
            Else While Do For To Space Semicolon Colon Period Comma\
            Assign Equal Unequal Less LessEqual Greater GreaterEqual\
            Plus Minus Times Slash ParenthesisOpen ParenthesisClose\
            ArrayIndexOpen ArrayIndexClose)

export TOKEN_TYPES

./GenTokenHpp.bash > Token.hpp
./GenTokenCpp.bash > Token.cpp
