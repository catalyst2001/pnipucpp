using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab12k
{
    public class MyBinaryTreeNodesCounter
    {
        int counter;
        char firstChar;
        int charIdx;

        MyBinaryTreeNodesCounter() {
            counter = 0;
            firstChar ='\0';
            charIdx = 0;
        }

        private void SearchRecursive(MyBinaryTreeNode<Library>? node)
        {
            if(node == null)
                return;

            string? str = node.data.GetOrgName();
            if (charIdx < str.Length && str[charIdx] == firstChar)
               counter++;

            SearchRecursive(node.left);
            SearchRecursive(node.right);
        }

        public MyBinaryTreeNodesCounter(MyBinaryTreeNode<Library> ?rootNode, char firstCharForFind, int charIndex = 0) {
            counter = 0;
            firstChar = firstCharForFind;
            charIdx = charIndex;
            SearchRecursive(rootNode.left);
            SearchRecursive(rootNode.right);
        }

        public int GetNumNodes() { return counter; }
    }
}
