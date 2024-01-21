using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab12i
{
    public class MyBinaryTreeHeight
    {
        int height;

        MyBinaryTreeHeight() {
            height = 0;
        }

        private void SearchRecursive(MyBinaryTreeNode<Republic>? node, int level)
        {
            if(node == null)
            {
                if (height < level)
                    height = level;
                return;
            }

            SearchRecursive(node.left, level+1);
            SearchRecursive(node.right, level+1);
        }

        public MyBinaryTreeHeight(MyBinaryTreeNode<Republic>? rootNode)
        {
            height = 0;
            SearchRecursive(rootNode.left, 0);
            SearchRecursive(rootNode.right, 0);
        }

        public int GetHeight() { return height; }
    }
}
