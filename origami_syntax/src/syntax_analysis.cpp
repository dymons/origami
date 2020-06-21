#include "origami_syntax/parsing/syntax_analysis.hpp"

namespace origami::parser {
void AstNode::addLeft(const std::shared_ptr<AstNode>& t_child) { m_left = t_child; }
void AstNode::addRight(const std::shared_ptr<AstNode>& t_child) { m_right = t_child; };
std::shared_ptr<AstNode> AstNode::left() const { return m_left; }
std::shared_ptr<AstNode> AstNode::right() const { return m_right; }
}// namespace origami::parser