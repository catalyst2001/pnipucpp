using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab12k
{
    public interface ITreeNodePrinter<_Ty>
    {
        public string? Print(MyBinaryTreeNode<_Ty>? node, int level);
    }

    public class MyBinaryTreePrinter<_Ty>
    {
        private int padWidth;
        private int maxLevel;
        private ITreeNodePrinter<_Ty> printer;

        private void PrintTreeBranch(int level, MyBinaryTreeNode<_Ty>? node)
        {
            if (null == node || level == maxLevel)
                return;

            int spacesCount = level * padWidth;
            Console.WriteLine(String.Format("{0," + spacesCount + "}{1}", "", printer.Print(node, level)));
            level++;
            PrintTreeBranch(level, node.left);
            PrintTreeBranch(level, node.right);
        }

        public MyBinaryTreePrinter(ITreeNodePrinter<_Ty> nodePrinter, MyBinaryTreeNode<_Ty> ?rootnode, int numlevels, int padwith = 2)
        {
            if (rootnode == null) {
                Console.WriteLine("root node is null");
                return;
            }
            printer = nodePrinter;
            padWidth = padwith;
            maxLevel = numlevels;
            PrintTreeBranch(0, rootnode.left);
            PrintTreeBranch(0, rootnode.right);
        }
    }
}
