/*
  THIS FILE IS GENERATED. ANY CHANGES WILL BE LOST
  GENERATED BY: /home/rev/Memory/Knowledge/Compiler/Lab/Parser/cpp/GenTokenCpp.bash
*/
#include "Token.hpp"

using namespace TokenType;

shared_ptr<Token> Program::Clone(int line, int col) const
{
    return make_shared<Program>(line, col);
}

Program::Program(int line, int col): Token(line, col)
{}

shared_ptr<Token> Const::Clone(int line, int col) const
{
    return make_shared<Const>(line, col);
}

Const::Const(int line, int col): Token(line, col)
{}

shared_ptr<Token> Char::Clone(int line, int col) const
{
    return make_shared<Char>(line, col);
}

Char::Char(int line, int col): Token(line, col)
{}

shared_ptr<Token> Type::Clone(int line, int col) const
{
    return make_shared<Type>(line, col);
}

Type::Type(int line, int col): Token(line, col)
{}

shared_ptr<Token> Var::Clone(int line, int col) const
{
    return make_shared<Var>(line, col);
}

Var::Var(int line, int col): Token(line, col)
{}

shared_ptr<Token> Integer::Clone(int line, int col) const
{
    return make_shared<Integer>(line, col);
}

Integer::Integer(int line, int col): Token(line, col)
{}

shared_ptr<Token> Array::Clone(int line, int col) const
{
    return make_shared<Array>(line, col);
}

Array::Array(int line, int col): Token(line, col)
{}

shared_ptr<Token> Of::Clone(int line, int col) const
{
    return make_shared<Of>(line, col);
}

Of::Of(int line, int col): Token(line, col)
{}

shared_ptr<Token> Function::Clone(int line, int col) const
{
    return make_shared<Function>(line, col);
}

Function::Function(int line, int col): Token(line, col)
{}

shared_ptr<Token> Procedure::Clone(int line, int col) const
{
    return make_shared<Procedure>(line, col);
}

Procedure::Procedure(int line, int col): Token(line, col)
{}

shared_ptr<Token> Begin::Clone(int line, int col) const
{
    return make_shared<Begin>(line, col);
}

Begin::Begin(int line, int col): Token(line, col)
{}

shared_ptr<Token> End::Clone(int line, int col) const
{
    return make_shared<End>(line, col);
}

End::End(int line, int col): Token(line, col)
{}

shared_ptr<Token> Call::Clone(int line, int col) const
{
    return make_shared<Call>(line, col);
}

Call::Call(int line, int col): Token(line, col)
{}

shared_ptr<Token> If::Clone(int line, int col) const
{
    return make_shared<If>(line, col);
}

If::If(int line, int col): Token(line, col)
{}

shared_ptr<Token> Then::Clone(int line, int col) const
{
    return make_shared<Then>(line, col);
}

Then::Then(int line, int col): Token(line, col)
{}

shared_ptr<Token> Else::Clone(int line, int col) const
{
    return make_shared<Else>(line, col);
}

Else::Else(int line, int col): Token(line, col)
{}

shared_ptr<Token> While::Clone(int line, int col) const
{
    return make_shared<While>(line, col);
}

While::While(int line, int col): Token(line, col)
{}

shared_ptr<Token> Do::Clone(int line, int col) const
{
    return make_shared<Do>(line, col);
}

Do::Do(int line, int col): Token(line, col)
{}

shared_ptr<Token> For::Clone(int line, int col) const
{
    return make_shared<For>(line, col);
}

For::For(int line, int col): Token(line, col)
{}

shared_ptr<Token> To::Clone(int line, int col) const
{
    return make_shared<To>(line, col);
}

To::To(int line, int col): Token(line, col)
{}

shared_ptr<Token> Space::Clone(int line, int col) const
{
    return make_shared<Space>(line, col);
}

Space::Space(int line, int col): Token(line, col)
{}

shared_ptr<Token> Semicolon::Clone(int line, int col) const
{
    return make_shared<Semicolon>(line, col);
}

Semicolon::Semicolon(int line, int col): Token(line, col)
{}

shared_ptr<Token> Colon::Clone(int line, int col) const
{
    return make_shared<Colon>(line, col);
}

Colon::Colon(int line, int col): Token(line, col)
{}

shared_ptr<Token> Period::Clone(int line, int col) const
{
    return make_shared<Period>(line, col);
}

Period::Period(int line, int col): Token(line, col)
{}

shared_ptr<Token> Comma::Clone(int line, int col) const
{
    return make_shared<Comma>(line, col);
}

Comma::Comma(int line, int col): Token(line, col)
{}

shared_ptr<Token> Assign::Clone(int line, int col) const
{
    return make_shared<Assign>(line, col);
}

Assign::Assign(int line, int col): Token(line, col)
{}

shared_ptr<Token> Equal::Clone(int line, int col) const
{
    return make_shared<Equal>(line, col);
}

Equal::Equal(int line, int col): Token(line, col)
{}

shared_ptr<Token> Unequal::Clone(int line, int col) const
{
    return make_shared<Unequal>(line, col);
}

Unequal::Unequal(int line, int col): Token(line, col)
{}

shared_ptr<Token> Less::Clone(int line, int col) const
{
    return make_shared<Less>(line, col);
}

Less::Less(int line, int col): Token(line, col)
{}

shared_ptr<Token> LessEqual::Clone(int line, int col) const
{
    return make_shared<LessEqual>(line, col);
}

LessEqual::LessEqual(int line, int col): Token(line, col)
{}

shared_ptr<Token> Greater::Clone(int line, int col) const
{
    return make_shared<Greater>(line, col);
}

Greater::Greater(int line, int col): Token(line, col)
{}

shared_ptr<Token> GreaterEqual::Clone(int line, int col) const
{
    return make_shared<GreaterEqual>(line, col);
}

GreaterEqual::GreaterEqual(int line, int col): Token(line, col)
{}

shared_ptr<Token> Plus::Clone(int line, int col) const
{
    return make_shared<Plus>(line, col);
}

Plus::Plus(int line, int col): Token(line, col)
{}

shared_ptr<Token> Minus::Clone(int line, int col) const
{
    return make_shared<Minus>(line, col);
}

Minus::Minus(int line, int col): Token(line, col)
{}

shared_ptr<Token> Times::Clone(int line, int col) const
{
    return make_shared<Times>(line, col);
}

Times::Times(int line, int col): Token(line, col)
{}

shared_ptr<Token> Slash::Clone(int line, int col) const
{
    return make_shared<Slash>(line, col);
}

Slash::Slash(int line, int col): Token(line, col)
{}

shared_ptr<Token> ParenthesisOpen::Clone(int line, int col) const
{
    return make_shared<ParenthesisOpen>(line, col);
}

ParenthesisOpen::ParenthesisOpen(int line, int col): Token(line, col)
{}

shared_ptr<Token> ParenthesisClose::Clone(int line, int col) const
{
    return make_shared<ParenthesisClose>(line, col);
}

ParenthesisClose::ParenthesisClose(int line, int col): Token(line, col)
{}

shared_ptr<Token> ArrayIndexOpen::Clone(int line, int col) const
{
    return make_shared<ArrayIndexOpen>(line, col);
}

ArrayIndexOpen::ArrayIndexOpen(int line, int col): Token(line, col)
{}

shared_ptr<Token> ArrayIndexClose::Clone(int line, int col) const
{
    return make_shared<ArrayIndexClose>(line, col);
}

ArrayIndexClose::ArrayIndexClose(int line, int col): Token(line, col)
{}

string Identifier::ToString() const
{
    return _id;
}

shared_ptr<Token> Identifier::Clone(int line, int col) const
{
    return make_shared<Identifier>(line, col, _id);
}

Identifier::Identifier(int line, int col, string id): Token(line, col), _id(id)
{}

string Number::ToString() const
{
    return to_string(_value);
}

shared_ptr<Token> Number::Clone(int line, int col) const
{
    return make_shared<Number>(line, col, _value);
}

Number::Number(int line, int col, int value): Token(line, col), _value(value)
{}

string Letter::ToString() const
{
    auto result = string("\'x\'");
    result[1]=_charcode;
    return result;
}

shared_ptr<Token> Letter::Clone(int line, int col) const
{
    return make_shared<Letter>(line, col, _charcode);
}

Letter::Letter(int line, int col, int charcode): Token(line, col), _charcode(charcode)
{}
