#pragma once

#include <iostream>
#include <stack>

#include "ScopedPtr.h"

class IntegerSet
{
 private:
	struct Node
	{
		explicit Node(int t_value) : value{ t_value }
		{
		}

		int value;
		ScopedPtr<Node> left{ nullptr };
		ScopedPtr<Node> right{ nullptr };
	};

 public:
	class ConstIterator
	{
	 private:
		void add_path_to(const Node *nod);

	 public:
		explicit ConstIterator(const Node *nod)
		{
			add_path_to(nod);
		}

		const Node *operator*();
		const Node *operator->();
		void operator++();
		bool operator==(const ConstIterator& other) const;
		bool operator!=(const ConstIterator& other) const;

	 private:
		std::stack<const Node *> m_path;
	};

 public: // Object lifetime
	IntegerSet() = default;
	IntegerSet(const IntegerSet&);
	IntegerSet& operator=(const IntegerSet&);
	IntegerSet(IntegerSet&&) noexcept;
	IntegerSet& operator=(IntegerSet&&) noexcept;
	~IntegerSet() noexcept = default;

 public: // Accessors
	[[nodiscard]] size_t size() const;

	[[nodiscard]] ConstIterator begin() const;
	[[nodiscard]] ConstIterator end() const;

 public: // Operations
	[[nodiscard]] bool equals(const IntegerSet&) const;
	[[nodiscard]] bool contains(int) const;
	[[nodiscard]] bool subsetOf(const IntegerSet&) const;
	[[nodiscard]] IntegerSet getUnion(const IntegerSet&) const;
	[[nodiscard]] IntegerSet getIntersection(const IntegerSet&) const;

	void add(int);

 private: // Helpers
	static IntegerSet from_sorted_values(const std::vector<int>&);
	static ScopedPtr<Node> make_balanced_root(std::vector<int>::const_iterator, std::vector<int>::const_iterator);

	bool cmp_node(const Node *const lhs, const Node *const rhs) const;
	ScopedPtr<Node> clone_node(Node *);
	void add_node(int, Node *node);

 public: // Serialization
	void deserialize(std::istream&);
	void serialize(std::ostream&) const;

 private: // Data members
	std::uint64_t m_size{ 0 };
	ScopedPtr<Node> m_root{ nullptr };
};

inline std::istream& operator>>(std::istream& in, IntegerSet& set)
{
	set.deserialize(in);
	return in;
}

inline std::ostream& operator<<(std::ostream& out, const IntegerSet& set)
{
	set.serialize(out);
	return out;
}
