#include <cassert>
#include <stack>
#include <vector>
#include <cmath>

#include "IntegerSet.h"

template<typename T>
inline T take_top(std::stack<T>& stack)
{
	auto& t = stack.top();
	stack.pop();
	return t;
}

#define XOR(a, b)    ((a) && (!b) || (!a) && (b))

/*
 * Object lifetime
 */

IntegerSet::IntegerSet(const IntegerSet& rhs)
	: m_size{ rhs.m_size }, m_root{ clone_node(rhs.m_root.get()) }
{
}

IntegerSet& IntegerSet::operator=(const IntegerSet& rhs)
{
	m_root = clone_node(rhs.m_root.get());
	m_size = rhs.m_size;
	return *this;
}

IntegerSet::IntegerSet(IntegerSet&& rhs) noexcept
	: m_root{ rhs.m_root.release() }, m_size{ rhs.m_size }
{
	assert(rhs.m_root.get() == nullptr);
	rhs.m_size = 0;
}

IntegerSet& IntegerSet::operator=(IntegerSet&& rhs) noexcept
{
	m_root.reset(rhs.m_root.release());
	m_size = rhs.m_size;
	rhs.m_size = 0;
	return *this;
}

size_t IntegerSet::size() const
{
	return m_size;
}

bool IntegerSet::equals(const IntegerSet& other) const
{
	if (size() != other.size())
		return false;
	if (this == &other)
		return true;
	for (auto it = begin(), other_it = other.begin();
		 it != end() && other_it != other.end(); ++it, ++other_it)
		if (it->value != other_it->value)
			return false;
	return true;
}

bool IntegerSet::contains(int elem) const
{
	for (auto node_ptr : *this)
		if (node_ptr->value == elem)
			return true;
	return false;
}

bool IntegerSet::subsetOf(const IntegerSet& other) const
{
	if (m_size > other.m_size) return false;
	if (this == &other) return true;
	auto it = begin();
	for (auto other_it = other.begin();
		 it != end() && other_it != other.end(); ++other_it)
		if (it->value == other_it->value)
			++it;
	return it == end();
}

IntegerSet IntegerSet::getUnion(const IntegerSet& other) const
{
	if (this == &other) return *this;
	IntegerSet result;
	for (auto node_ptr : *this)
		result.add(node_ptr->value);
	for (auto node_ptr : other)
		result.add(node_ptr->value);
	return result;
}

IntegerSet IntegerSet::getIntersection(const IntegerSet& other) const
{
	if (this == &other) return *this;
	IntegerSet result;
	for (auto it = begin(), other_it = other.begin();
		 it != end() && other_it != other.end();)
		if (it->value == other_it->value)
		{
			result.add(it->value);
			++it, ++other_it;
		}
		else
		{
			if (it->value < other_it->value)
				++it;
			else if (it->value > other_it->value)
				++other_it;
		}
	return result;
}

void IntegerSet::deserialize(std::istream& in)
{
	std::vector<int> nodes;
	size_t expected_count = 0, passed_count = 0;
	in >> expected_count;
	for (int val; passed_count < expected_count && in >> val; ++passed_count)
	{
		if (!nodes.empty() && val <= nodes.back())
			throw std::runtime_error{ "unsorted input passed to deserialize()" };
		nodes.emplace_back(val);
	}

	if (passed_count != expected_count)
		throw std::runtime_error{ "insufficient amount of values passed to deserialize()" };

	*this = from_sorted_values(nodes);
}

void IntegerSet::serialize(std::ostream& out) const
{
	out << m_size << (m_size > 0 ? " " : "");
	auto it = begin();
	for (size_t seen = 0; it != end(); ++it, ++seen)
		out << (*it)->value << (seen < m_size - 1 ? " " : "");
}

/*
 * Helpers
 */

ScopedPtr<IntegerSet::Node> IntegerSet::make_balanced_root(
	std::vector<int>::const_iterator start,
	std::vector<int>::const_iterator end)
{
	if (start == end)
		return {};
	auto it = start;
	const auto dist = std::distance(start, end);
	std::advance(it, dist / 2);
	auto root = make_scoped_ptr<Node>(*it);
	root->left = make_balanced_root(start, it);
	root->right = make_balanced_root(std::next(it), end);
	return root;
}

IntegerSet IntegerSet::from_sorted_values(const std::vector<int>& values)
{
	if (values.empty())
		return IntegerSet{};
	IntegerSet is;
	is.m_root = make_balanced_root(values.cbegin(), values.cend());
	is.m_size = values.size();
	return is;
}

bool IntegerSet::cmp_node(const IntegerSet::Node *const lhs, const IntegerSet::Node *const rhs) const
{
	if (XOR(lhs, rhs)) return false;
	return lhs->value == rhs->value
		&& cmp_node(lhs->left.get(), rhs->left.get())
		&& cmp_node(lhs->right.get(), rhs->right.get());
}

ScopedPtr<IntegerSet::Node> IntegerSet::clone_node(IntegerSet::Node *node)
{
	if (!node) return nullptr;
	auto root = make_scoped_ptr<Node>(node->value);
	root->left = clone_node(node->left.get());
	root->right = clone_node(node->right.get());
	return root;
}

/*
 * Iterator
 */

IntegerSet::ConstIterator IntegerSet::begin() const
{
	return IntegerSet::ConstIterator{ m_root.get() };
}

IntegerSet::ConstIterator IntegerSet::end() const
{
	return IntegerSet::ConstIterator{ nullptr };
}

void IntegerSet::add(int value)
{
	if (!m_root)
		m_root = make_scoped_ptr<Node>(value), ++m_size;
	add_node(value, m_root.get());
}

void IntegerSet::add_node(int value, Node *node)
{
	if (node->value > value)
	{
		if (node->left) add_node(value, node->left.get());
		else node->left = make_scoped_ptr<Node>(value), ++m_size;
	}
	else if (node->value < value)
	{
		if (node->right) add_node(value, node->right.get());
		else node->right = make_scoped_ptr<Node>(value), ++m_size;
	}
}

void IntegerSet::ConstIterator::add_path_to(const IntegerSet::Node *nod)
{
	for (const Node *curr = nod; curr; curr = curr->left.get())
		m_path.push(curr);
}

const IntegerSet::Node *IntegerSet::ConstIterator::operator*()
{
	return m_path.top();
}

const IntegerSet::Node *IntegerSet::ConstIterator::operator->()
{
	return m_path.empty() ? nullptr : m_path.top();
}

void IntegerSet::ConstIterator::operator++()
{
	auto *p = take_top(m_path);
	add_path_to(p->right.get());
}

bool IntegerSet::ConstIterator::operator==(const IntegerSet::ConstIterator& other) const
{
	if (m_path.empty() || other.m_path.empty())
		return m_path.empty() == other.m_path.empty();
	return m_path.top() == other.m_path.top();
}
bool IntegerSet::ConstIterator::operator!=(const IntegerSet::ConstIterator& other) const
{
	return !(*this == other);
}
