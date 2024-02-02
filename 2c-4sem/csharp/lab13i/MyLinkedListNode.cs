using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab13i
{
    public class MyLinkedListNode<T>
    {
        MyLinkedListNode<T>? next_node;
        T node_data;

        public MyLinkedListNode(MyLinkedListNode<T>? next, T data)
        {
            next_node = next;
            node_data = data;
        }

        public T Data
        {
            get { return node_data; }
            set { node_data = value; }
        }

        public MyLinkedListNode<T>? Next
        {
            get { return next_node; }
            set { next_node = value; }
        }
    };
}
