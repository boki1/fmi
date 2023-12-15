#pragma once

#include <cstdint>
#include <cassert>
#include <stdexcept>
#include <sstream>

#define MYSTD_LIST

#ifndef MYSTD_LIST
#include <list>
#endif

#include "stats_allocator.h"

namespace mystd {

#ifndef MYSTD_LIST

    /*
     * For hw testing purposes.
     */

    template <typename T>
    using list = std::list<T>;
#else

    namespace details {

        template<typename T>
        struct list_node {
            explicit list_node(const T &t_data = T()) : data{t_data} {}

            list_node(const T &t_data, list_node *t_prev, list_node *t_next) : data{t_data}, next{t_next},
                                                                               prev{t_prev} {}

            T data;
            list_node *next{nullptr}, *prev{nullptr};
        };

    }

///
/// Interface
///

    template<typename T, typename Allocator = stats_allocator<details::list_node<T>>>
    class list {
    private: // Types
        using node = details::list_node<T>;

        struct iterator;

#include "list_iterator.h"

    public: // Lifetime
        list() = default;

        list(const list &);

        list &operator=(const list &);

        ~list() noexcept;

        list(const std::initializer_list<T> &values);

    public: // Iterators
        iterator begin();

        iterator begin() const;

        iterator end();

        iterator end() const;

    public: // Element access
        const T &front() const;

        T &front();

        const T &back() const;

        T &back();

    public: // Capacity
        [[nodiscard]] std::uint64_t size() const noexcept;

        [[nodiscard]] bool empty() const noexcept;

    public: // Modifiers
        iterator insert(iterator pos, const T &value);

        iterator erase(iterator pos);

        void push_front(const T &value);

        void push_back(const T &value);

        void pop_front();

        void pop_back();

        void clear() noexcept;

        void concat(list &&other) {
            if (m_tail)
                m_tail->next = other.m_head;
            else {
                assert(!m_head);
                m_head = other.m_head;
            }
            m_tail = other.m_tail;
            m_size += other.size();
            other.detach();
        }

    public: // Operations
        template<class Compare>
        void merge(list &&other, Compare comp);

        template<class Compare>
        void sort(Compare curr);

    private:
        void detach();

        void attach_back(node *new_node);

    private: // Data members
        node *m_head{nullptr}, *m_tail{nullptr};
        std::uint64_t m_size{0};
        Allocator m_allocator;
    };

///
/// Implementation
///


/*
 * Lifetime
 */

    template<typename T, typename A>
    list<T, A>::list(const list<T, A> &rhs) {
        for (auto &n: rhs)
            push_back(n);
    }

    template<typename T, typename A>
    list<T, A> &list<T, A>::operator=(const list<T, A> &rhs) {
        if (this != &rhs) {
            clear();
            for (auto &n: rhs)
                push_back(n);
        }
    }

    template<typename T, typename A>
    list<T, A>::~list() noexcept {
        clear();
    }

    template<typename T, typename A>
    list<T, A>::list(const std::initializer_list<T> &values) {
        for (const auto &v : values)
            push_back(v);
    }

/*
 * Iterators
 */

    template<typename T, typename A>
    typename list<T, A>::iterator list<T, A>::begin() {
        return iterator{m_head, this};
    }

    template<typename T, typename A>
    typename list<T, A>::iterator list<T, A>::end() {
        return iterator{nullptr, this};
    }


    template<typename T, typename A>
    typename list<T, A>::iterator list<T, A>::begin() const {
        return iterator{m_head, this};
    }

    template<typename T, typename A>
    typename list<T, A>::iterator list<T, A>::end() const {
        return iterator{nullptr, this};
    }

/*
 * Capacity
 */

    template<typename T, typename A>
    std::uint64_t list<T, A>::size() const noexcept {
        return m_size;
    }

    template<typename T, typename A>
    bool list<T, A>::empty() const noexcept {
        return !m_head;
    }

    /*
 * Modifiers
 */

    template<typename T, typename A>
    typename list<T, A>::iterator list<T, A>::insert(
            list<T, A>::iterator pos,
            const T &value) {
        if (pos == end()) {
            push_back(value);
            return iterator{m_tail, this};
        } else {
            node *new_node = m_allocator.alloc(value, pos.m_node_ptr ? pos.m_node_ptr->prev : nullptr, pos.m_node_ptr);
            if (!new_node->prev)
                m_head = new_node;
            else
                new_node->prev->next = new_node;
            if (!new_node->next)
                m_tail = new_node;
            else
                new_node->next->prev = new_node;
            ++m_size;
            return iterator{new_node, this};
        }
    }

    template<typename T, typename A>
    typename list<T, A>::iterator list<T, A>::erase(
            list<T, A>::iterator pos) {
        node *old_node = pos.m_node_ptr;
        node *node_it_rv = old_node->next;
        if (old_node->prev) {
            assert(old_node != m_head);
            old_node->prev->next = old_node->next;
        } else {
            assert(old_node == m_head);
            m_head = old_node->next;
        }

        if (old_node->next) {
            assert(old_node != m_tail);
            old_node->next->prev = old_node->prev;
        } else {
            assert(old_node == m_tail);
            m_tail = old_node->prev;
        }
        m_allocator.free(old_node);
        --m_size;
        return iterator{node_it_rv, this};
    }

    template<typename T, typename A>
    void list<T, A>::push_front(const T &value) {
        insert(begin(), value);
    }

    template<typename T, typename A>
    void list<T, A>::push_back(const T &value) {
        node *new_node = m_allocator.alloc(value);
        attach_back(new_node);
    }

    template<typename T, typename A>
    void list<T, A>::attach_back(node *new_node) {
        if (!m_head) {
            assert(m_tail == nullptr);
            m_head = m_tail = new_node;
        } else {
            m_tail->next = new_node;
            new_node->prev = m_tail;
            m_tail = new_node;
        }
        ++m_size;
    }

    template<typename T, typename A>
    void list<T, A>::pop_front() {
        erase(begin());
    }

    template<typename T, typename A>
    void list<T, A>::pop_back() {
        erase(iterator{m_tail, this});
    }

    template<typename T, typename A>
    void list<T, A>::clear() noexcept {
        for (node *curr = m_head; curr;) {
            node *old_node = curr;
            curr = curr->next;
            m_allocator.free(old_node);
        }
        m_head = m_tail = nullptr;
        m_size = 0;
    }

    template<typename T, typename A>
    void list<T, A>::detach() {
        m_head = m_tail = nullptr;
        m_size = 0;
    }

/*
 * Operations
 */

    template<typename T, typename A>
    template<class Compare>
    void list<T, A>::merge(list<T, A> &&other, Compare comp) {
        if (other.size() < 1)
            return;
        list l;
        auto it_this = begin(), it_other = other.begin();
        while (it_this != end() && it_other != other.end())
            if (comp(*it_this, *it_other))
                l.attach_back(it_this++.m_node_ptr);
            else
                l.attach_back(it_other++.m_node_ptr);
        while (it_this != end())
            l.attach_back(it_this++.m_node_ptr);
        while (it_other != other.end())
            l.attach_back(it_other++.m_node_ptr);
        m_head = l.m_head;
        m_tail = l.m_tail;
        m_size += other.size();
        other.detach(), l.detach();
    }

    template<typename T, typename A>
    template<class Compare>
    void list<T, A>::sort(Compare comp) {
        if (m_size <= 1)
            return;

        /*
         * Very ugly bubble sort.
         * I would not do that in a "real case" :).
         *
         * FYI Did not improve this because I decided on a different approach
         * for the problem solution and sort was not actually needed.
         */

        auto local_swap = [](node **curr) {
            node *tmp = *curr;
            *curr = (*curr)->next;
            tmp->next = (*curr)->next;
            (*curr)->next = tmp;
            (*curr)->prev = tmp->prev;
            tmp->prev = *curr;
            if (tmp->next)
                tmp->next->prev = tmp;
        };

        bool initial = true;
        for (node **first = &m_head, *sorted = NULL, *last = NULL;
             (*first)->next != last;
             last = sorted) {
            node **curr = first;
            sorted = (*first)->next;
            for (; (*curr)->next != last; curr = &(*curr)->next) {
                if (comp((*curr)->next->data, (*curr)->data)) {
                    local_swap(curr);
                    sorted = (*curr)->next;
                }
            }
            if (initial) {
                m_tail = *curr;
                initial = false;
            }
        }
    }

    /*
 * Element access
 */

    template<typename T, typename A>
    T &list<T, A>::front() {
        if (!m_head)
            throw std::runtime_error{"No elements in list."};
        return m_head->data;
    }

    template<typename T, typename A>
    const T &list<T, A>::front() const {
        if (!m_head)
            throw std::runtime_error{"No elements in list."};
        return m_head->data;
    }

    template<typename T, typename A>
    T &list<T, A>::back() {
        if (!m_tail)
            throw std::runtime_error{"No elements in list."};
        return m_tail->data;
    }

    template<typename T, typename A>
    const T &list<T, A>::back() const {
        if (!m_tail)
            throw std::runtime_error{"No elements in list."};
        return m_tail->data;
    }

#endif
}
