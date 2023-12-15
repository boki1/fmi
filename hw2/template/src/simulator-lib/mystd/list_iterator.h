class iterator {
	friend list<T, Allocator>;

public: // Lifetime
    iterator(node *node_ptr, const list<T, Allocator> *owner)
		: m_node_ptr{node_ptr}, m_owner{owner} {}

public:
	const T &operator*() const {
		assert(m_node_ptr);
		return m_node_ptr->data;
	}
	T &operator*() {
		assert(m_node_ptr);
		return m_node_ptr->data;
	}

	const T *operator->() const {
		assert(m_node_ptr);
		return &m_node_ptr->data;
	}
	T *operator->() {
		assert(m_node_ptr);
		return &m_node_ptr->data;
	}

    iterator &operator++() {
	    if (m_node_ptr)
            m_node_ptr = m_node_ptr->next;
        return *this;
    }
    iterator operator++(int) {
        auto it{*this};
        ++*this;
        return it;
    }

	bool operator==(const iterator &rhs) const noexcept {
		return m_node_ptr == rhs.m_node_ptr && m_owner == rhs.m_owner;
	}
	bool operator!=(const iterator &rhs) const noexcept {
		return !(*this == rhs);
	}

private:
    node *m_node_ptr;
	const list<T, Allocator> *m_owner;
};