using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace lab12i
{
    /* node creator impl for create and set data in tree node */
    public class TreeNodeCreatorImpl : ITreeBuilderNodeCreator<Republic>
    {
        private int nodeNum;

        public TreeNodeCreatorImpl()
        {
            nodeNum = 0;
        }

        public MyBinaryTreeNode<Republic>? CreateNode(MyBinaryTreeNode<Republic>? rootNode, int level)
        {
            Republic lib = new Republic($"{nodeNum}Republic");
            MyBinaryTreeNode<Republic>? node = new MyBinaryTreeNode<Republic>(lib, rootNode);
            nodeNum++;
            return node;
        }
    }

    /* tree node printer implementation for print data in node graph */
    public class TreeNodePrinterImpl : ITreeNodePrinter<Republic>
    {
        public string? Print(MyBinaryTreeNode<Republic>? node, int level)
        {
            if (node == null)
                return "node was null";

            if (node.data == null)
                return "node data is null";

            return $"{node.data.GetName()} ({level})";
        }
    }

    public class Lab122
    {
        public void Start()
        {
            const int numTreeLevels = 3;

            TreeNodeCreatorImpl creatorImpl = new TreeNodeCreatorImpl(); // create NodeCreator implementation
            IdealBinaryTreeBuilder<Republic> treeBuilder = new IdealBinaryTreeBuilder<Republic>(creatorImpl, numTreeLevels); // create IdealBinaryTreeBuilder
            MyBinaryTreeNode<Republic>? idealTreeRootNode = treeBuilder.GetRootNode(); // get ref to root ideal tree node

            TreeNodePrinterImpl printerImpl = new TreeNodePrinterImpl(); // create NodePrinter implementation
            new MyBinaryTreePrinter<Republic>(printerImpl, idealTreeRootNode, numTreeLevels); // print tree

            MyBinaryTreeHeight treeHeight = new MyBinaryTreeHeight(idealTreeRootNode);
            Console.WriteLine("tree height is {0}", treeHeight.GetHeight()); //print tree heigth

            MyBSTBuilder binaryTreeSearchTreeBuilder = new MyBSTBuilder(idealTreeRootNode, '1');
            Console.WriteLine("Num Pathes {0}", binaryTreeSearchTreeBuilder.walkPathes.Count);

            foreach (MyBinaryTreeNode<Republic>? rootNode in binaryTreeSearchTreeBuilder.searchTreeRootNodes)
            {
                Console.WriteLine("-------------------------------------------");
                new MyBinaryTreePrinter<Republic>(printerImpl, rootNode, numTreeLevels); // print BST
            }
        }
    }
}
