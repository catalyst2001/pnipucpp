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

        public MyBinaryTreeNode() {
            root = null;
            left = null;
            right = null;
        }

        public MyBinaryTreeNode(MyBinaryTreeNode<_Ty> rootNode) {
            root = rootNode;
            left = null;
            right = null;
        }

        public MyBinaryTreeNode<_Ty>? AddLeftNode()
        {
            return left = new MyBinaryTreeNode<_Ty>(this);
        }

        public MyBinaryTreeNode<_Ty>? AddRightNode()
        {
            return right = new MyBinaryTreeNode<_Ty>(this);
        }
    }
}
