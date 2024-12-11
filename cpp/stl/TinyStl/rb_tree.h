//
// Created by 王少泽 on 2024/12/8.
//

#ifndef MYTINYSTL_RB_TREE_H_
#define MYTINYSTL_RB_TREE_H_

// 这个头文件包含一个模板类 rb_tree
// rb_tree : 红黑树

#include <initializer_list>

#include <cassert>

#include "functional.h"
#include "iterator.h"
#include "memory.h"
#include "type_traits.h"
#include "exceptdef.h"
#include "heap_algo.h"

namespace mystl
{
    typedef bool rb_tree_color_type;

    static constexpr rb_tree_color_type rb_tree_red=false;//设置红色节点为false
    static constexpr rb_tree_color_type rb_tree_black=true;//黑色节点为true

    template<class T>
    struct rb_tree_node_base;

    template<class T>
    struct rb_tree_node;

    template<class T,bool>
    struct rb_tree_value_traits_imp {
        typedef T key_type;
        typedef T mapped_type;
        typedef T value_type;

        template<class Ty>
        static const key_type& get_key(const Ty& value) {
            return value;
        }

        template<class Ty>
        static const value_type& get_value(const Ty& value) {
            return value;
        }
    };

    template<class T>//有键值对的map
    struct rb_tree_value_traits_imp<T,true> {
        typedef typename std::remove_cv<typename T::first_type>::type key_type;
        typedef typename T::second_type mapped_type;
        typedef typename T value_type;

        template<class Ty>
        static const key_type& get_key(const Ty& value) {
            return value.first;
        }

        template<class Ty>
        static const value_type& get_value(const Ty& value) {
            return value;
        }
    };

    template<class T>
    struct rb_tree_value_traits {
        static constexpr bool is_map=mystl::is_pair<T>::value;

        typedef rb_tree_value_traits_imp<T,is_map> value_traits_type;

        typedef typename value_traits_type::key_type key_type;
        typedef typename value_traits_type::mapped_type mapped_type;
        typedef typename value_traits_type::value_type value_type;

        template<class Ty>
        static const key_type& get_key(const Ty& value) {
            return value_traits_type::get_key(value);
        }

        template<class Ty>
        static const value_type& get_value(const Ty& value) {
            return value_traits_type::get_value(value);
        }
    };

    template<class T>
    struct rb_tree_node_traits {
        typedef rb_tree_color_type color_type;//树节点的颜色 black or red

        typedef rb_tree_value_traits<T> value_traits;
        typedef typename value_traits::key_type key_type;
        typedef typename value_traits::mapped_type mapped_type;
        typedef typename value_traits::value_type value_type;

        typedef rb_tree_node_base<T>* base_ptr;
        typedef rb_tree_node<T>* node_ptr;
    };

    template<class T>
    struct rb_tree_node_base {
        typedef rb_tree_color_type color_type;
        typedef rb_tree_node_base<T>* base_ptr;
        typedef rb_tree_node<T>* node_ptr;

        base_ptr parent;
        base_ptr left;
        base_ptr right;
        color_type color;

        base_ptr get_base_ptr() {
            return &*this;
        }

        node_ptr get_node_ptr() {
            return reinterpret_cast<node_ptr> (&*this);
        }

        node_ptr& get_node_ref() {
            return reinterpret_cast<node_ptr> (*this);
        }
    };

    template <class T>
    struct rb_tree_node:public rb_tree_node_base<T> {
        typedef rb_tree_node_base<T>* base_ptr;
        typedef rb_tree_node<T>* node_ptr;

        T value;

        base_ptr get_base_ptr() {
            return static_cast<base_ptr>(&*this);
        }

        node_ptr get_node_ptr() {
            return &*this;
        }
    };

    template<class T>
    struct rb_tree_traits {
        typedef rb_tree_value_traits<T> value_traits;

        typedef typename value_traits::key_type key_type;
        typedef typename value_traits::mapped_type mapped_type;
        typedef typename value_traits::value_type value_type;

        typedef value_type* pointer;
        typedef value_type& reference;
        typedef const value_type* const_pointer;
        typedef const value_type& const_reference;

        typedef rb_tree_node_base<T> base_type;
        typedef rb_tree_node<T> node_type;

        typedef base_type* base_ptr;
        typedef node_type* node_ptr;
    };

    template<class T>
    struct rb_tree_iterator_base:public mystl::iterator<mystl::bidirectional_iterator_tag,T> {
        typedef typename rb_tree_traits<T>::base_ptr base_ptr;

        base_ptr node; //一个指向节点本身的指针

        //迭代器的前进
        void inc() {
            if(node->right!=nullptr) {
                node=rb_tree_min(node->right);//右子树的最小
            }else {//没有右孩子
                auto y=node->parent;
                while(y->right==node) {//往上寻找
                    node=y;
                    y=y->parent;
                }
                if(node->right!=y) node=y;//上一个比该节点大的
            }
        }

        void dec() {
            if(node->parent->parent==node && rb_tree_is_red(node)) {
                node=node->right;
            }
            else if(node->left!=nullptr) {
                node=rb_tree_max(tree->left);//左子树的最大
            }else {
                auto y=node->partnt;
                while(node==y->left) {
                    node=y;
                    y=y->parent;//上一个比该节点小的
                }
                node=y;
            }
        }

        bool operator==(const rb_tree_iterator_base& rhs) {
            return node==rhs.node;
        }
        bool operator!=(const rb_tree_iterator_base& rhs) {
            return node!=rhs.node;
        }
    };

    template<class T>
    struct rb_tree_iterator:rb_tree_iterator_base<T> {
        typedef rb_tree_traits<T> tree_traits;

        typedef typename tree_traits::value_type value_type;
        typedef typename tree_traits::pointer pointer;
        typedef typename tree_traits::reference reference;
        typedef typename tree_traits::base_ptr base_ptr;
        typedef typename tree_traits::node_ptr node_ptr;

        typedef rb_tree_iterator<T> iterator;
        typedef rb_tree_const_iterator<T> const_iterator;
        typedef iterator self;

        using rb_tree_iterator_base<T>::node; //声明使用的是基类的node属性

        rb_tree_iterator(){};
        rb_tree_iterator(base_ptr x) {
            node=x;
        }
        rb_tree_iterator(node_ptr x) {
            node=x;
        }
        rb_tree_iterator(const iterator& rhs) {
            node=rhs.node;
        }
        rb_tree_iterator(const const_iterator& rhs) {
            node=rhs.node;
        }

        reference operator*() const {
            return node->get_node_ptr()->value;
        }

        pointer operator->() const {
            return &(operator*());
        }

        self& operator++() {
            this->inc();
            return *this;
        }

        self& operator++(int) {
            self tmp(*this);
            this->inc();
            return tmp;
        }

        self& operator--() {
            this->dec();
            return *this;
        }

        slef& operator--(int) {
            self tmp(*this);
            this->dec();
            return tmp;
        }
    };

    template<class T>
    struct rb_tree_const_iterator : public rb_tree_iterator_base<T> {
        typedef rb_tree_traits<T> tree_traits;

        typedef typename tree_traits::value_type value_type;
        typedef typename tree_traits::const_pointer pointer;
        typedef typename tree_traits::const_reference reference;
        typedef typename tree_traits::base_ptr base_ptr;
        typedef typename tree_traits::node_ptr node_ptr;

        typedef rb_tree_iterator<T> iterator;
        typedef rb_tree_const_iterator const_iterator;
        typedef const_iterator self;

        using rb_tree_iterator_base<T>::node;

        rb_tree_const_iterator() {

        }

        rb_tree_const_iterator(base_ptr x) {
            node=x;
        }
        rb_tree_const_iterator(node_ptr x) {
            node=x;
        }
        rb_tree_const_iterator(const iterator& rhs) {
            node=rhs.node;
        }
        rb_tree_const_iterator(const const_iterator& rhs) {
            node=rhs.node;
        }
        reference operator*()  const { return node->get_node_ptr()->value; }
        pointer   operator->() const { return &(operator*()); }

        self& operator++()
        {
            this->inc();
            return *this;
        }
        self operator++(int)
        {
            self tmp(*this);
            this->inc();
            return tmp;
        }
        self& operator--()
        {
            this->dec();
            return *this;
        }
        self operator--(int)
        {
            self tmp(*this);
            this->dec();
            return tmp;
        }
    };

    template<class Nodeptr>
    Nodeptr rb_tree_min(Nodeptr x) noexcept {
        while(x->left!=nullptr) {
            x=x->left;
        }
        return x;
    }

    template<class Nodeptr>
    Nodeptr rb_tree_max(Nodeptr) noexcept {
        while(x->right!=nullptr) {
            x=x->right;
        }
        return x;
    }

    //判断该节点是不是左孩子
    template<class Nodeptr>
    bool rb_tree_is_lchild(Nodeptr node) noexcept {
        return node==node->parent->left;
    }
    //该节点是否为红节点
    template<class Nodeptr>
    bool rb_tree_is_red(Nodeptr node) noexcept {
        return node->color==rb_tree_red;
    }
    //该节点是否为黑节点
    template<class Nodeptr>
    bool rb_tree_is_black(Nodeptr node) noexcept {
        return node->color==rb_tree_black;
    }
    //设置该节点为黑色
    template<class Nodeptr>
    void rb_tree_set_black(Nodeptr& node) noexcept {
        node->color=rb_tree_black;
    }
    //设置该节点为红色
    template<class Nodeptr>
    void rb_tree_set_red(Nodeptr& node) noexcept {
        node->color=rb_tree_red;
    }
    template<class Nodeptr>
    Nodeptr rb_tree_next(Nodeptr node) noexcept {
        if(node->right!=nullptr) return rb_tree_min(node->right);
        while(!rb_tree_is_lchild(node)) {
            node=node->parent;
        }
        return node->parent;
    }

    template<class Nodeptr>
    void rb_tree_rotate_left(Nodeptr x,Nodeptr& root) noexcept {
        auto y=x->right;
        x->right=y->left;
        if(y->left!=nullptr) {
            y->left->parent=x;
        }
        y->parent=x->parent;//x和y变成兄弟

        if(x==root) {
            //如果x是根节点，让y顶替x成为根节点
            root=y;
        }
        else if(rb_tree_is_lchild(x)) {
            x->parent->left=y;
        }else {
            //x是右节点
            x->parent->right=y;
        }
        y->left=x;
        x->parent=y;
    }
    //右旋
    template<class Nodeptr>
    void rb_tree_rorate_right(Nodeptr x,Nodeptr& root) noexcept {
        auto y=x->left;
        x->left=y->right;
        if(y->right) {
            y->right->parent=x;
        }
        y->parent=x->parent;

        if(x==root) {
            root=y;
        }
        else if(rb_tree_is_lchild(x)) {
            x->parent->left=y;
        }else {
            x->parent->right=y;
        }
        y->right=x;
        x->parent=y;
    }

    //插入节点后使红黑树重新平衡
    //红黑树的性质：任何一条路径上红黑节点的数量相同
    template<class Nodeptr>
    void rb_tree_insert_rebalance(Nodeptr x,Nodeptr& root) noexcept {
        rb_tree_set_red(x);//新增的节点设置为红色
        while(x!=root && rb_tree_is_red(x->parent)) {
            if(rb_tree_is_lchild(x->parent)) {
                auto uncle=x->parent->parent->right;
                if(uncle!=nullptr && rb_tree_is_red(uncle)) {
                    rb_tree_set_red(x->parent);
                    rb_tree_set_red(uncle);
                    x=x->parent->parent;
                    rb_tree_set_red(x);
                }
                else {
                    //无叔叔或者叔叔为黑
                    if(!rb_tree_is_lchild(x)) { //当前插入的是右节点
                        x=x->parent;
                        rb_tree_rotate_left(x,root);//父节点左旋
                    }
                    rb_tree_set_black(x->parent);
                    rb_tree_set_red(x->parent->parent);
                    rb_tree_rorate_right(x->parent->parent,root);
                    break;
                }
            }
            else {
                //父节点是右孩子
                auto uncle = x->parent->parent->left;
                if(uncle!=nullptr&&rb_tree_is_red(uncle)) {
                    rb_tree_set_red(x->parent);
                    rb_tree_set_red(uncle);
                    x=x->parent->parent;
                    rb_tree_set_red(x);
                }
                else {
                    if(rb_tree_is_lchild(x)) {
                        x=x->parent;
                        rb_tree_rotate_right(x,root);
                    }
                    rb_tree_set_black(x->parent);
                    rb_tree_set_red(x->parent->parent);
                    rb_tree_rotate_left(x->parent->parent,root);
                    break;
                }
            }
        }
        rb_tree_set_black(root);//根节点永远为黑
    }

    //删除节点后使红黑树平衡,要删除的节点，根节点，最小节点，最大节点
    template <class NodePtr>
    NodePtr rb_tree_erase_rebalance(NodePtr z, NodePtr& root, NodePtr& leftmost, NodePtr& rightmost)
    {
      // y 是可能的替换节点，指向最终要删除的节点
      auto y = (z->left == nullptr || z->right == nullptr) ? z : rb_tree_next(z);
      // x 是 y 的一个独子节点或 NIL 节点
      auto x = y->left != nullptr ? y->left : y->right;
      // xp 为 x 的父节点
      NodePtr xp = nullptr;

      if (y != z) //y=rb_tree_next(z)，z没有子节点
      {
          // 用 y 顶替 z 的位置，用 x 顶替 y 的位置，最后用 y 指向 z
        z->left->parent = y;
        y->left = z->left;

        // 如果 y 不是 z 的右子节点，那么 z 的右子节点一定有左孩子
        if (y != z->right)
        { // x 替换 y 的位置
          xp = y->parent;
          if (x != nullptr)
            x->parent = y->parent;

          y->parent->left = x;
          y->right = z->right;
          z->right->parent = y;
        }
        else//y是z的右子节点
        {
          xp = y;
        }

        // 连接 y 与 z 的父节点
        if (root == z)
          root = y;
        else if (rb_tree_is_lchild(z))
          z->parent->left = y;
        else
          z->parent->right = y;
        y->parent = z->parent;
        mystl::swap(y->color, z->color);
        y = z;
      }
      // y == z 说明 z 至多只有一个孩子
      else
      {
        xp = y->parent;
        if (x)
          x->parent = y->parent;

        // 连接 x 与 z 的父节点
        if (root == z)
          root = x;
        else if (rb_tree_is_lchild(z))
          z->parent->left = x;
        else
          z->parent->right = x;

        // 此时 z 有可能是最左节点或最右节点，更新数据
        if (leftmost == z)
          leftmost = x == nullptr ? xp : rb_tree_min(x);
        if (rightmost == z)
          rightmost = x == nullptr ? xp : rb_tree_max(x);
      }

      // 此时，y 指向要删除的节点，x 为替代节点，从 x 节点开始调整。
      // 如果删除的节点为红色，树的性质没有被破坏，否则按照以下情况调整（x 为左子节点为例）：
      // case 1: 兄弟节点为红色，令父节点为红，兄弟节点为黑，进行左（右）旋，继续处理
      // case 2: 兄弟节点为黑色，且两个子节点都为黑色或 NIL，令兄弟节点为红，父节点成为当前节点，继续处理
      // case 3: 兄弟节点为黑色，左子节点为红色或 NIL，右子节点为黑色或 NIL，
      //         令兄弟节点为红，兄弟节点的左子节点为黑，以兄弟节点为支点右（左）旋，继续处理
      // case 4: 兄弟节点为黑色，右子节点为红色，令兄弟节点为父节点的颜色，父节点为黑色，兄弟节点的右子节点
      //         为黑色，以父节点为支点左（右）旋，树的性质调整完成，算法结束
      if (!rb_tree_is_red(y))
      { // x 为黑色时，调整，否则直接将 x 变为黑色即可
        while (x != root && (x == nullptr || !rb_tree_is_red(x)))
        {
          if (x == xp->left)
          { // 如果 x 为左子节点
            auto brother = xp->right;
            if (rb_tree_is_red(brother))
            { // case 1
              rb_tree_set_black(brother);
              rb_tree_set_red(xp);
              rb_tree_rotate_left(xp, root);
              brother = xp->right;
            }
            // case 1 转为为了 case 2、3、4 中的一种
            if ((brother->left == nullptr || !rb_tree_is_red(brother->left)) &&
                (brother->right == nullptr || !rb_tree_is_red(brother->right)))
            { // case 2
              rb_tree_set_red(brother);
              x = xp;
              xp = xp->parent;
            }
            else
            {
              if (brother->right == nullptr || !rb_tree_is_red(brother->right))
              { // case 3
                if (brother->left != nullptr)
                  rb_tree_set_black(brother->left);
                rb_tree_set_red(brother);
                rb_tree_rotate_right(brother, root);
                brother = xp->right;
              }
              // 转为 case 4
              brother->color = xp->color;
              rb_tree_set_black(xp);
              if (brother->right != nullptr)
                rb_tree_set_black(brother->right);
              rb_tree_rotate_left(xp, root);
              break;
            }
          }
          else  // x 为右子节点，对称处理
          {
            auto brother = xp->left;
            if (rb_tree_is_red(brother))
            { // case 1
              rb_tree_set_black(brother);
              rb_tree_set_red(xp);
              rb_tree_rotate_right(xp, root);
              brother = xp->left;
            }
            if ((brother->left == nullptr || !rb_tree_is_red(brother->left)) &&
                (brother->right == nullptr || !rb_tree_is_red(brother->right)))
            { // case 2
              rb_tree_set_red(brother);
              x = xp;
              xp = xp->parent;
            }
            else
            {
              if (brother->left == nullptr || !rb_tree_is_red(brother->left))
              { // case 3
                if (brother->right != nullptr)
                  rb_tree_set_black(brother->right);
                rb_tree_set_red(brother);
                rb_tree_rotate_left(brother, root);
                brother = xp->left;
              }
              // 转为 case 4
              brother->color = xp->color;
              rb_tree_set_black(xp);
              if (brother->left != nullptr)
                rb_tree_set_black(brother->left);
              rb_tree_rotate_right(xp, root);
              break;
            }
          }
        }
        if (x != nullptr)
          rb_tree_set_black(x);
      }
      return y;
    }
    template<class T,class Compare>
    class rb_tree {
    public:
        typedef rb_tree_traits<T> tree_traits;
        typedef rb_tree_value_traits<T> value_traits;


        typedef typename tree_traits::base_type base_type;
        typedef typename tree_traits::base_ptr base_ptr;
        typedef typename tree_traits::node_type node_type;
        typedef typename tree_traits::node_ptr node_ptr;
        typedef typename tree_traits::key_type key_type;
        typedef typename tree_traits::mapped_type mapped_type;
        typedef typename tree_traits::value_type value_type;
        typedef Compare key_compare;

        typedef mystl::allocator<T> allocator_type;
        typedef mystl::allocator<T> data_allocator;
        typedef mystl::allocator<base_type> base_allocator;
        typedef mystl::allocator<node_type> node_allocator;

        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference;
        typedef typename allocator_type::size_type size_type;
        typedef typename allocator_type::difference_type difference_type;

        typedef rb_tree_iterator<T> iterator;
        typedef rb_tree_const_iterator<T> const_iterator;
        typedef mystl::reverse_iterator<iterator> reverse_iterator;
        typedef mystl::reverse_iterator<const_iterator> const_reverse_iterator;

        allocator_type get_allocator() const {
            return node_allocator();
        }
        key_compare key_comp() const{
            return key_comp_;
        }

    private:
        base_ptr header_;//与根节点互为对方的父节点 header_->parent=root; root->parent=header;
        size_type node_count_;
        key_compare key_comp_;
    private:
        // 根节点
        base_ptr& root() const {
            return header_->parent;
        }
        // 最小节点
        base_ptr& leftmost() const {
            return header_->left;
        }
        //最大节点
        base_ptr& rightmost() const {
            return header_->right;
        }
    public:
        rb_tree() { rb_tree_init(); }

        rb_tree(const rb_tree& rhs);
        rb_tree(rb_tree&& rhs) noexcept;

        rb_tree& operator=(const rb_tree& rhs);
        rb_tree& operator=(rb_tree&& rhs);

        ~rb_tree() { clear(); }
    public:
      // 迭代器相关操作
      iterator               begin()         noexcept
      { return leftmost(); }
      const_iterator         begin()   const noexcept
      { return leftmost(); }
      iterator               end()           noexcept
      { return header_; }
      const_iterator         end()     const noexcept
      { return header_; }

      reverse_iterator       rbegin()        noexcept
      { return reverse_iterator(end()); }
      const_reverse_iterator rbegin()  const noexcept
      { return const_reverse_iterator(end()); }
      reverse_iterator       rend()          noexcept
      { return reverse_iterator(begin()); }
      const_reverse_iterator rend()    const noexcept
      { return const_reverse_iterator(begin()); }

      const_iterator         cbegin()  const noexcept
      { return begin(); }
      const_iterator         cend()    const noexcept
      { return end(); }
      const_reverse_iterator crbegin() const noexcept
      { return rbegin(); }
      const_reverse_iterator crend()   const noexcept
      { return rend(); }

      // 容量相关操作

      bool      empty()    const noexcept { return node_count_ == 0; }
      size_type size()     const noexcept { return node_count_; }
      size_type max_size() const noexcept { return static_cast<size_type>(-1); }

      // 插入删除相关操作

      // emplace

      template <class ...Args>
      iterator  emplace_multi(Args&& ...args);

      template <class ...Args>
      mystl::pair<iterator, bool> emplace_unique(Args&& ...args);

      template <class ...Args>
      iterator  emplace_multi_use_hint(iterator hint, Args&& ...args);

      template <class ...Args>
      iterator  emplace_unique_use_hint(iterator hint, Args&& ...args);

      // insert

      iterator  insert_multi(const value_type& value);
      iterator  insert_multi(value_type&& value)
      {
        return emplace_multi(mystl::move(value));
      }

      iterator  insert_multi(iterator hint, const value_type& value)
      {
        return emplace_multi_use_hint(hint, value);
      }
      iterator  insert_multi(iterator hint, value_type&& value)
      {
        return emplace_multi_use_hint(hint, mystl::move(value));
      }

      template <class InputIterator>
      void      insert_multi(InputIterator first, InputIterator last)
      {
        size_type n = mystl::distance(first, last);
        THROW_LENGTH_ERROR_IF(node_count_ > max_size() - n, "rb_tree<T, Comp>'s size too big");
        for (; n > 0; --n, ++first)
          insert_multi(end(), *first);
      }

      mystl::pair<iterator, bool> insert_unique(const value_type& value);
      mystl::pair<iterator, bool> insert_unique(value_type&& value)
      {
        return emplace_unique(mystl::move(value));
      }

      iterator  insert_unique(iterator hint, const value_type& value)
      {
        return emplace_unique_use_hint(hint, value);
      }
      iterator  insert_unique(iterator hint, value_type&& value)
      {
        return emplace_unique_use_hint(hint, mystl::move(value));
      }

      template <class InputIterator>
      void      insert_unique(InputIterator first, InputIterator last)
      {
        size_type n = mystl::distance(first, last);
        THROW_LENGTH_ERROR_IF(node_count_ > max_size() - n, "rb_tree<T, Comp>'s size too big");
        for (; n > 0; --n, ++first)
          insert_unique(end(), *first);
      }

      // erase

      iterator  erase(iterator hint);

      size_type erase_multi(const key_type& key);
      size_type erase_unique(const key_type& key);

      void      erase(iterator first, iterator last);

      void      clear();

      // rb_tree 相关操作

      iterator       find(const key_type& key);
      const_iterator find(const key_type& key) const;

      size_type      count_multi(const key_type& key) const
      {
        auto p = equal_range_multi(key);
        return static_cast<size_type>(mystl::distance(p.first, p.second));
      }
      size_type      count_unique(const key_type& key) const
      {
        return find(key) != end() ? 1 : 0;
      }

      iterator       lower_bound(const key_type& key);
      const_iterator lower_bound(const key_type& key) const;

      iterator       upper_bound(const key_type& key);
      const_iterator upper_bound(const key_type& key) const;

      mystl::pair<iterator, iterator>
      equal_range_multi(const key_type& key)
      {
        return mystl::pair<iterator, iterator>(lower_bound(key), upper_bound(key));
      }
      mystl::pair<const_iterator, const_iterator>
      equal_range_multi(const key_type& key) const
      {
        return mystl::pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key));
      }

      mystl::pair<iterator, iterator>
      equal_range_unique(const key_type& key)
      {
        iterator it = find(key);
        auto next = it;
        return it == end() ? mystl::make_pair(it, it) : mystl::make_pair(it, ++next);
      }
      mystl::pair<const_iterator, const_iterator>
      equal_range_unique(const key_type& key) const
      {
        const_iterator it = find(key);
        auto next = it;
        return it == end() ? mystl::make_pair(it, it) : mystl::make_pair(it, ++next);
      }

      void swap(rb_tree& rhs) noexcept;

    private:

      // node related
      template <class ...Args>
      node_ptr create_node(Args&&... args);
      node_ptr clone_node(base_ptr x);
      void     destroy_node(node_ptr p);

      // init / reset
      void     rb_tree_init();
      void     reset();

      // get insert pos
      mystl::pair<base_ptr, bool>
               get_insert_multi_pos(const key_type& key);
      mystl::pair<mystl::pair<base_ptr, bool>, bool>
               get_insert_unique_pos(const key_type& key);

      // insert value / insert node
      iterator insert_value_at(base_ptr x, const value_type& value, bool add_to_left);
      iterator insert_node_at(base_ptr x, node_ptr node, bool add_to_left);

      // insert use hint
      iterator insert_multi_use_hint(iterator hint, key_type key, node_ptr node);
      iterator insert_unique_use_hint(iterator hint, key_type key, node_ptr node);

      // copy tree / erase tree
      base_ptr copy_from(base_ptr x, base_ptr p);
      void     erase_since(base_ptr x);
    };

    //复制构造函数
    template<class T, class Compare>
    rb_tree<T, Compare>::rb_tree(const rb_tree &rhs) {
        rb_tree_init();
        if(rhs.node_count_!=0) {
            root()=copy_from(rhs.root(),header_);
            leftmost()=rb_tree_min(root());
            rightmost()=rb_tree_max(root());
        }
        node_count_=rhs.node_count_;
        key_comp_=rhs.key_comp_;
    }

    //移动构造函数
    template<class T, class Compare>
    rb_tree<T, Compare>::rb_tree(rb_tree &&rhs) noexcept:header_(mystl::move(rhs.header_)),key_comp_(rhs.key_comp_),node_count_(rhs.node_count_) {
        rhs.reset();
    }

    template<class T, class Compare>
    rb_tree<T, Compare>& rb_tree<T, Compare>::operator=(const rb_tree &rhs) {
        if(this!=&rhs) {
            clear();
            if(rhs.node_count_!=0) {
                root()=copy_from(rhs.root(),header_);
                leftmost()=rb_tree_min(root());
                rightmost()=rb_tree_max(root());
            }
            node_count_=rhs.node_count_;
            key_comp_=rhs.key_comp_;
        }
        return *this;
    }

    template<class T, class Compare>
    rb_tree<T, Compare>& rb_tree<T, Compare>::operator=(rb_tree &&rhs) {
        clear();
        header_=mystl::move(rhs.header_);
        node_count_=rhs.node_count_;
        key_comp_=rhs.key_comp_;
        rhs.reset();
        return *this;
    }

    //就地插入,key,value允许重复
    template<class T, class Compare>
    template<class... Args>
    typename rb_tree<T, Compare>::iterator rb_tree<T, Compare>::emplace_multi(Args &&... args) {
        THROW_LENGTH_ERROR_IF(node_count_>max_size()-1,"too long");
        node_ptr np=create_node(mystl::forward<Args>(args)...);
        auto res=get_insert_multi_pos(value_traits::get_key(np->value));//返回的是pair
        return insert_node_at(res.first,np,res.second);
    }

    //就地插入，key,value不可重复
    template<class T, class Compare>
    template<class... Args>
    mystl::pair<typename rb_tree<T, Compare>::iterator, bool> rb_tree<T, Compare>::emplace_unique(Args &&... args) {
        THROW_LENGTH_ERROR_IF(node_count_>max_size()-1,"size is too big");
        node_ptr np=create_node(mystl::forward<Args>(args)...);
        auto res=get_insert_unique_pos(value_traits::get_key(np->value));
        if(res.second) {
            //res.second表示无重复，插入成功
            return mystl::make_pair(insert_node_at(res.first.first,np,res.first.second),true);
        }
        destroy_node(np);
        return mystl::make_pair(iterator(res.first.first),false);
    }

    //就地插入元素，键值允许重复，多了个参数hint
    template<class T, class Compare>
    template<class... Args>
    typename rb_tree<T, Compare>::iterator
    rb_tree<T, Compare>::emplace_multi_use_hint(iterator hint, Args &&... args) {
        THROW_LENGTH_ERROR_IF(node_count_>max_size()-1,"size is too long");
        node_ptr np=create_node(mystl::forward<Args>(args)...);
        if(node_count_==0) {
            return insert_node_at(header_,np,true);
        }
        key_type key=value_traits::get_key(np->value);
        if(hint==begin()) {
            //hint位于begin处
            if(key_comp_(key,value_traits::get_key(*hint))) {
                return insert_node_at(hint.node,np,true);
            }
            else {
                auto pos=get_insert_multi_pos(key);
                return insert_node_at(pos.first,np,pos.second);
            }
        }else if(hint==end()) {
            if(!key_comp_(key,value_traits::get_key(rightmost()->get_node_ptr()->value))) {
                //np比树中最大的节点还要大
                return insert_node_at(rightmost(),np,false);
            }
            else {
                auto pos=get_insert_multi_pos(key);
                return insert_node_at(pos.first,np,pos.second);
            }
        }
        return insert_multi_use_hint(hint,key,np);
    }

    template<class T, class Compare>
    template<class... Args>
    typename rb_tree<T, Compare>::iterator
    rb_tree<T, Compare>::emplace_unique_use_hint(iterator hint, Args &&... args) {
        THROW_LENGTH_ERROR_IF(node_count_>max_size()-1,"size is too long");
        node_ptr np=create_node(mystl::forward<Args>(args)...);
        if(node_count_==0) {
            return insert_node_at(header_,np,true);
        }
        key_type key=value_traits::get_key(np->value);
        if(hint==begin()) {
            if(key_comp_(key,value_traits::get_key(*hint))) {
                //插入节点比hint小，那么就插在hint的左子节点
                return insert_node_at(hint.node,np,true);
            }else {
                auto pos=get_insert_unique_pos(key);
                if(!pos.second) {
                    destroy_node(np);
                    return pos.first.first;//无法插入，返回父节点
                }
                return insert_node_at(pos.first.first,np,pos.first.second);
            }
        }else if(hint == end()) {
            //np比rightmost大，插在end()的右边
            if(key_comp_(value_traits::get_key(rightmost()->get_node_ptr()->value),key)) {
                insert_node_at(rightmost(),np,false);
            }else {
                auto pos=get_insert_unique_pos(key);
                if(!pos.second) {
                    destroy_node(np);
                    return pos.first.first;
                }
                return insert_node_at(pos.first.first,np,pos.first.second);
            }
        }
        return insert_unique_use_hint(hint,key,np);
    }

    template<class T,class Compare>
      typename rb_tree<T,Compare>::iterator
      rb_tree<T,Compare>::insert_multi(const value_type& value) {
        THROW_LENGTH_ERROR_IF(node_count_>max_size()-1,"size is too long");
        auto res=get_insert_multi_pos(value_traits::get_key(value));
        return insert_value_at(res.first,value,res.second);
    }

    template<class T, class Compare>
    mystl::pair<typename rb_tree<T, Compare>::iterator, bool>
    rb_tree<T, Compare>::insert_unique(const value_type &value) {
        THROW_LENGTH_ERROR_IF(node_count_>max_size()-1,"size is too long");
        auto res=get_insert_unique_pos(value_traits::get_key(value));
        if(res.second) {
            //插入红黑树中
            return mystl::make_pair(insert_value_at(res.first.first,value,res.first.second),true);
        }
        //没有插入红黑树
        return mystl::make_pair(res.first.first,false);
    }

    template<class T, class Compare>
    typename rb_tree<T, Compare>::iterator rb_tree<T, Compare>::erase(iterator hint) {
        auto node=hint.node->get_node_ptr();
        iterator next(node);
        ++next;
        rb_tree_erase_rebalance(hint.node,root(),leftmost(),rightmost());
        destroy_node(node);
        --node_count_;
        return next;//返回下一个节点
    }

    //删除键值等于key的元素，返回删除的数量
    template<class T, class Compare>
    typename rb_tree<T, Compare>::size_type
    rb_tree<T, Compare>::erase_multi(const key_type &key) {
            auto p=equal_range_multi(key);
            size_type n=mystl::distance(p.first,p.second);
            erase(p.first,p.second);
            return n;
    }

    template<class T, class Compare>
    typename rb_tree<T, Compare>::size_type
    rb_tree<T, Compare>::erase_unique(const key_type &key) {
        auto it=find(key);
        if(it!=end()) {
            erase(it);
            return 1;
        }
        return 0;
    }

    template<class T, class Compare>
    void rb_tree<T, Compare>::erase(iterator first, iterator last) {
        if(first==begin()&&last==end()) {
            clear();
        }else {
            while (first!=last) {
                erase(first++);
            }
        }
    }



    //返回红黑树插入节点的父节点以及插到父节点的左孩子还是右孩子
    template<class T, class Compare>
    mystl::pair<typename rb_tree<T, Compare>::base_ptr, bool>
    rb_tree<T, Compare>::get_insert_multi_pos(const key_type &key) {
        auto x=root();
        auto y=header_;
        bool add_to_left=true;
        while (x!=nullptr) {
            y=x;
            add_to_left=key_comp_(key,value_traits::get_key(x->get_node_ptr()->value));
            x=add_to_left?x->left:x->right;
        }
        return mystl::make_pair(y,add_to_left);
    }

    template<class T, class Compare>
    mystl::pair<mystl::pair<typename rb_tree<T, Compare>::base_ptr, bool>, bool>
    rb_tree<T, Compare>::get_insert_unique_pos(const key_type &key) {
        //内层的pair表示插入的父节点，和是否插在左边；外层pair的那个bool表示是否插入成功
        auto x=root();
        auto y=header_;
        bool add_to_left=true;
        while(x!=nullptr) {
            y=x;
            add_to_left=key_comp_(key,value_traits::get_key(x->get_node_ptr()->value));
            x=add_to_left?x->left:x->right;
        }
        //--------------------下面是为了保证unique而新增的逻辑-------------------
        iterator j=iterator(y);//y是插入节点的父节点
        if(add_to_left) {
            if(y==header_||j==begin()) {
                return mystl::make_pair(mystl::make_pair(y,add_to_left),true);
            }
            else {
                --j;
            }
        }
        if(key_comp_(value_traits::get_key(*j),key)) {
            //j<key
            return mystl::make_pair(mystl::make_pair(y,add_to_left),true);
        }
        return mystl::make_pair(mystl::make_pair(y,add_to_left),false);
    }

    template<class T, class Compare>
    typename rb_tree<T, Compare>::iterator
    //node是要插入的节点
    rb_tree<T, Compare>::insert_multi_use_hint(iterator hint, key_type key, node_ptr node) {
        //在hint附近查找可插入的位置
        auto np=hint.node;
        auto before=hint;
        --before;
        auto bnp=befoe.node;//上一个比hint位置的节点小的节点
        if(!key_comp_(key,value_traits::get_key(*before))&&!key_comp_(value_traits::get_key(*hint),key)) {
            //before<=key<=hint
            if(bnp->right==nullptr) {
                return insert_node_at(bnp,node,false);
            }
            else if(np->left==nullptr) {
                return insert_node_at(np,node,true);
            }
        }
        auto pos=get_insert_multi_pos(key);
        return insert_node_at(pos.first,node,pos.second);
    }

    template<class T, class Compare>
    typename rb_tree<T, Compare>::iterator
    rb_tree<T, Compare>::insert_unique_use_hint(iterator hint, key_type key, node_ptr node) {
            auto np=hint.node;
            auto before=hint;
            --before;
            auto bnp=before.node;
            if(key_comp_(value_traits::get_key(*before),key)&&key_comp_(key,value_traits::get_key(*hint))) {
                if(bnp->right==nullptr) {
                    return insert_node_at(bnp,node,false);
                }else if(np->left==nullptr) {
                    return insert_node_at(np,node,true);
                }
            }
            auto pos=get_insert_unique_pos(key);
            if(!pos.second) {
                destroy_node(node);
                return pos.first.second;
            }
            return insert_node_at(pos.first.first,node,pos.first.second);
    }



    template<class T, class Compare>
    void rb_tree<T, Compare>::reset() {
        header_=nullptr;
        node_count_=0;
    }

    template<class T,class Compare>
    void rb_tree<T,Compare>::clear() {
        if(node_count_!=0) {
            erase_since(root());
            leftmost()=header_;
            root()=nullptr;
            rightmost()=header_;
            node_count_=0;
        }
    }

    template<class T, class Compare>
    void rb_tree<T, Compare>::erase_since(base_ptr x) {
        while (x!=nullptr) {
            erase_since(x->right);
            auto y=x->left;
            destroy_node(x->get_node_ptr());
            x=y;
        }
    }

    //递归复制一棵树，节点从x开始，p为x的父节点
    template<class T, class Compare>
    typename rb_tree<T, Compare>::base_ptr rb_tree<T, Compare>::copy_from(base_ptr x, base_ptr p) {
        auto top=clone_node(x);//克隆x节点，x的left和right指针为空
        top->parent=p;
        try {
            if(x->right) {
                top->right=copy_from(x->right,top);//递归
            }
            p=top;
            x=x->left;
            while(x!=nullptr) {
                auto y=clone_node(x);
                p->left=y;
                if(x->right) {
                    y->right=copy_from(x->right,y);
                }
                p=y;
                x=x->left;
            }
        }catch (...) {
            erase_since(top);
            throw;
        }
        return top;
    }

    //x是插入的父节点，node是插入的节点，bool表示是否在左边插入
    template<class T, class Compare>
    typename rb_tree<T, Compare>::iterator
    rb_tree<T, Compare>::insert_node_at(base_ptr x, node_ptr node, bool add_to_left) {
        node->parent=x;
        auto base_node=node->get_base_ptr();
        if(x==header_) {
            root()=base_node;
            leftmost()=base_node;
            rightmost()=base_node;
        }
        else if(add_to_left) {
            x->left=base_node;
            if(leftmost()==x) {
                leftmost()=x;
            }
        }else {
            x->right=base_node;
            if(rightmost()==x) {
                rightmost()=base_node;
            }
        }
        rb_tree_insert_rebalance(base_node,root());//重新平衡
        ++node_count_;
        return iterator(node);
    }

    template<class T, class Compare>
    typename rb_tree<T, Compare>::iterator
    //x是插入的父节点，value是要插入节点的value，bool表示是否在左边插入
    rb_tree<T, Compare>::insert_value_at(base_ptr x, const value_type &value, bool add_to_left) {
            node_ptr node=create_node(value);
            node->parent=x;
            auto base_node=node->get_base_ptr();
            if(x==header_) {
                root()=base_node;  
                leftmost()=base_node;
                rightmost()=base_node;
            }else if(add_to_left) {
                x->left=base_node;
                if(leftmost()==x) leftmost()=base_node;
            }
            else {
                x->right=base_node;
                if(rightmost()==x) rightmost()=base_node;
            }
            rb_tree_insert_rebalance(base_node,root());
            ++node_count_;
        return iterator(node);
    }



    template<class T, class Compare>
    void rb_tree<T, Compare>::rb_tree_init() {
        header_=base_allocator::allocate(1);
        header_->color=rb_tree_red;
        root()=nullptr;
        leftmost()=header_;
        rightmost()=header_;
        node_count_=0;
    }

    //创造一个节点
    template<class T, class Compare>
    template<class... Args>
    typename rb_tree<T, Compare>::node_ptr rb_tree<T, Compare>::create_node(Args &&... args) {
        auto tmp=node_allocator(1);
        try {
            data_allocator::construct(mystl::address_of(tmp->value),mystl::forward<Args>(args)...);
            tmp->left=nullptr;
            tmp->right=nullptr;
            tmp->parent=nullptr;
        }catch(...) {
            node_allocator::deallocate(tmp);
            throw;
        }
        return tmp;
    }

    template<class T, class Compare>
    typename rb_tree<T, Compare>::node_ptr rb_tree<T, Compare>::clone_node(base_ptr x) {
            node_ptr tmp=create_node(x->get_node_ptr()->value);
            tmp->color=x->color;
            tmp->left=nullptr;
            tmp->right=nullptr;
            return tmp;
    }

    //键值不小于key的第一个位置
    template<class T, class Compare>
    typename rb_tree<T, Compare>::iterator
    rb_tree<T, Compare>::lower_bound(const key_type &key) {
        auto y=header_;
        auto x=root();
        while (x!=nullptr) {
            if(!key_comp_(value_traits::get_key(x->get_node_ptr()->value),key)) {
                //x>=key
                y=x;
                x=x->left;
            }else {
                x=x->right;
            }
        }
        return iterator(y);
    }

    //键值不大于key的第一个位置
    template<class T, class Compare>
    typename rb_tree<T, Compare>::iterator
    rb_tree<T, Compare>::upper_bound(const key_type &key) {
        auto x=header_;
        auto y= root();
        while(x!=nullptr) {
            if(key_comp_(key,value_traits::get_key(x->get_node_ptr()->value))) {
                //key<x
                y=x;
                x=x->left;
            }else {
                x=x->right;
            }
        }
        return iterator(y);
    }

    template<class T, class Compare>
   typename rb_tree<T, Compare>::iterator
   rb_tree<T, Compare>::lower_bound(const key_type &key) const {
        auto y=header_;
        auto x=root();
        while (x!=nullptr) {
            if(!key_comp_(value_traits::get_key(x->get_node_ptr()->value),key)) {
                //x>=key
                y=x;
                x=x->left;
            }else {
                x=x->right;
            }
        }
        return const_iterator(y);
    }

    template<class T, class Compare>
    typename rb_tree<T, Compare>::iterator
    rb_tree<T, Compare>::upper_bound(const key_type &key) const {
        auto x=header_;
        auto y= root();
        while(x!=nullptr) {
            if(key_comp_(key,value_traits::get_key(x->get_node_ptr()->value))) {
                //key<x
                y=x;
                x=x->left;
            }else {
                x=x->right;
            }
        }
        return const_iterator(y);
    }

    //查找键值为k的节点，返回它的迭代器
    template<class T, class Compare>
    typename rb_tree<T, Compare>::iterator rb_tree<T, Compare>::find(const key_type &key) {
        auto y=header_;
        auto x=root();
        while(x!=nullptr) {
            if(!key_comp_(value_traits::get_key(x->get_node_ptr()->value),key)) {
                //x>=key
                y=x;
                x=x->left;
            }else {
                x=x->right;
            }
        }
        iterator j=iterator(y);
        return j==end()||key_comp_(key,value_traits::get_value(*j))?end():j;
    }

    template <class T, class Compare>
    typename rb_tree<T, Compare>::const_iterator
    rb_tree<T, Compare>::
    find(const key_type& key) const
        {
            auto y = header_;  // 最后一个不小于 key 的节点
            auto x = root();
            while (x != nullptr)
            {
                if (!key_comp_(value_traits::get_key(x->get_node_ptr()->value), key))
                { // key 小于等于 x 键值，向左走
                    y = x, x = x->left;
                }
                else
                { // key 大于 x 键值，向右走
                    x = x->right;
                }
            }
            const_iterator j = const_iterator(y);
            return (j == end() || key_comp_(key, value_traits::get_key(*j))) ? end() : j;
        }

    template <class T, class Compare>
    void rb_tree<T, Compare>::
    swap(rb_tree& rhs) noexcept
        {
            if (this != &rhs)
            {
                mystl::swap(header_, rhs.header_);
                mystl::swap(node_count_, rhs.node_count_);
                mystl::swap(key_comp_, rhs.key_comp_);
            }
        }

    template<class T, class Compare>
    void rb_tree<T, Compare>::destroy_node(node_ptr p) {
        data_allocator::destroy(&p->value);
        node_allocator::deallocate(p);
    }

    // 重载比较操作符
    template <class T, class Compare>
    bool operator==(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs)
    {
        return lhs.size() == rhs.size() && mystl::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template <class T, class Compare>
    bool operator<(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs)
    {
        return mystl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <class T, class Compare>
    bool operator!=(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs)
    {
        return !(lhs == rhs);
    }

    template <class T, class Compare>
    bool operator>(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs)
    {
        return rhs < lhs;
    }

    template <class T, class Compare>
    bool operator<=(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs)
    {
        return !(rhs < lhs);
    }

    template <class T, class Compare>
    bool operator>=(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs)
    {
        return !(lhs < rhs);
    }

    // 重载 mystl 的 swap
    template <class T, class Compare>
    void swap(rb_tree<T, Compare>& lhs, rb_tree<T, Compare>& rhs) noexcept
    {
        lhs.swap(rhs);
    }

}
#endif