using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab12k
{
    public interface ITreeBuilderNodeCreator<_Ty>
    {
        /* for create node and set data to default constructor */
        public MyBinaryTreeNode<_Ty>? CreateNode(MyBinaryTreeNode<_Ty>? rootNode, int level);
    }

    public class IdealBinaryTreeBuilder<_Ty>
    {
        private MyBinaryTreeNode<_Ty> root;
        ITreeBuilderNodeCreator<_Ty> creator;

        private void BuildTreeBranchRecursive(MyBinaryTreeNode<_Ty>? root, int level, ref MyBinaryTreeNode<_Ty>? node) {
            /* end node no have child nodes */
            if (level == 0) {
                node = null;
                return;
            }
            level--;
            node = creator.CreateNode(root, level);
            BuildTreeBranchRecursive(root, level, ref node.left);
            BuildTreeBranchRecursive(root, level, ref node.right);
        }

        public IdealBinaryTreeBuilder(ITreeBuilderNodeCreator<_Ty> nodeCreator, int levels) {
            creator = nodeCreator;
            root = new MyBinaryTreeNode<_Ty>();
            BuildTreeBranchRecursive(root, levels, ref root.right);
            BuildTreeBranchRecursive(root, levels, ref root.left);
        }

        public MyBinaryTreeNode<_Ty> GetRootNode() { return root; }
    }
}
