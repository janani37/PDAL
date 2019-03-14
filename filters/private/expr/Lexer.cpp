
#include "Lexer.hpp"
#include <pdal/DimUtil.hpp>

namespace pdal
{
namespace expr
{

Token Lexer::get()
{
    char c;

    while (isspace(m_buf[m_pos]))
        m_pos++;

    if (m_pos == m_buf.size())
        return Token(TokenType::Eof);

    Token tok = get(m_buf[m_pos]);
    m_pos = tok.end();
    return tok;
}


Token Lexer::get(char c)
{
    Token tok(TokenType::Error);

    if (c == '+')
    {
        tok = number();
        if (!tok.valid())
            tok = Token(TokenType::Plus, m_pos, m_pos + 1);
    }
    else if (c == '-')
    {
        tok = number();
        if (!tok.valid())
            tok = Token(TokenType::Minus, m_pos, m_pos + 1);
    }
    else if (c == '/')
        tok = Token(TokenType::Divide, m_pos, m_pos + 1);
    else if (c == '*')
        tok = Token(TokenType::Multiply, m_pos, m_pos + 1);
    else if (c == '(')
        tok = Token(TokenType::Lparen, m_pos, m_pos + 1);
    else if (c == ')')
        tok = Token(TokenType::Rparen, m_pos, m_pos + 1);
    else if (isdigit(c))
        tok = number();
    else if (isalpha(c))
        tok = dimension();

    return tok;
}


Token Lexer::number()
{
    const char *start = m_buf.data() + m_pos;
    char *end;

    double v = strtod(start, &end);
    if (start == end)
        return Token(TokenType::Error);
    return Token(TokenType::Number, m_pos, end - m_buf.data(), v);
}


Token Lexer::dimension()
{
    size_t end = Dimension::extractName(m_buf, m_pos);
    return Token(TokenType::Dimension, m_pos, m_pos + end,
        m_buf.substr(m_pos, end));
}

} // namespace expr
} // namespace pdal