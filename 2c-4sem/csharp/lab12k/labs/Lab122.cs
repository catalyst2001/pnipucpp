using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace lab12k.labs
{
    /* node creator impl for create and set data in tree node */
    public class TreeNodeCreatorImpl : ITreeBuilderNodeCreator<Library>
    {
        private int nodeNum;

        public TreeNodeCreatorImpl() {
            nodeNum = 0;
        }

        public MyBinaryTreeNode<Library>? CreateNode(MyBinaryTreeNode<Library> ?rootNode, int level) {
            MyBinaryTreeNode<Library>? node = new MyBinaryTreeNode<Library>(rootNode);
            node.data = new Library($"{nodeNum}Library", $"street {nodeNum + 100}", $"8:00-20:00", nodeNum + 222);
            nodeNum++;
            return node;
        }
    }

    /* tree node printer implementation for print data in node graph */
    public class TreeNodePrinterImpl : ITreeNodePrinter<Library>
    {
        public string? Print(MyBinaryTreeNode<Library>? node, int level) {
            return $"{node.data.GetOrgName()} ({level})";
        }
    }

    public class Lab122
    {
        public void Start()
        {
            const int numTreeLevels = 3;

            TreeNodeCreatorImpl creatorImpl = new TreeNodeCreatorImpl(); // create NodeCreator implementation
            IdealBinaryTreeBuilder<Library> treeBuilder = new IdealBinaryTreeBuilder<Library>(creatorImpl, numTreeLevels); // create IdealBinaryTreeBuilder
            MyBinaryTreeNode<Library>? idealTreeRootNode = treeBuilder.GetRootNode(); // get ref to root ideal tree node

            TreeNodePrinterImpl printerImpl = new TreeNodePrinterImpl(); // create NodePrinter implementation
            new MyBinaryTreePrinter<Library>(printerImpl, idealTreeRootNode, numTreeLevels); // print tree

            MyBinaryTreeNodesCounter treeElementsCounter = new MyBinaryTreeNodesCounter(idealTreeRootNode, '8'); //create tree elements counter
            Console.WriteLine("{0} tree elements starts with '8' first symbol", treeElementsCounter.GetNumNodes()); //print count elements starts with 'L' sym



        }
    }
}
