using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static System.Runtime.InteropServices.JavaScript.JSType;

namespace lab12k
{
    public class MyBinaryTreeNode<_Ty>
    {
        public MyBinaryTreeNode<_Ty>? root;
        public MyBinaryTreeNode<_Ty>? left;
        public MyBinaryTreeNode<_Ty>? right;
        public _Ty data;

        public MyBinaryTreeNode(_Ty Data) {
            data = Data;
            root = null;
            left = null;
            right = null;
        }

        public MyBinaryTreeNode(_Ty Data, MyBinaryTreeNode<_Ty> rootNode) {
            data = Data;
            root = rootNode;
            left = null;
            right = null;
        }

        public MyBinaryTreeNode<_Ty>? AddLeftNode(_Ty Data)
        {
            return left = new MyBinaryTreeNode<_Ty>(Data, this);
        }

        public MyBinaryTreeNode<_Ty>? AddRightNode(_Ty Data)
        {
            return right = new MyBinaryTreeNode<_Ty>(Data, this);
        }
    }
}
