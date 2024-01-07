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
            Library lib = new Library($"{nodeNum}Library", $"street {nodeNum + 100}", $"8:00-20:00", nodeNum + 222);
            MyBinaryTreeNode<Library>? node = new MyBinaryTreeNode<Library>(lib, rootNode);
            nodeNum++;
            return node;
        }
    }

    /* tree node printer implementation for print data in node graph */
    public class TreeNodePrinterImpl : ITreeNodePrinter<Library>
    {
        public string? Print(MyBinaryTreeNode<Library>? node, int level) {
            if (node == null)
                return "node was null";

            if (node.data == null)
                return "node data is null";

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

            MyBinaryTreeNodesCounter treeElementsCounter = new MyBinaryTreeNodesCounter(idealTreeRootNode, '1'); //create tree elements counter
            Console.WriteLine("{0} tree elements starts with '8' first symbol", treeElementsCounter.GetNumNodes()); //print count elements starts with 'L' sym

            MyBSTBuilder binaryTreeSearchTreeBuilder = new MyBSTBuilder(idealTreeRootNode, '1');
            Console.WriteLine("Num Pathes {0}", binaryTreeSearchTreeBuilder.walkPathes.Count);

            foreach(MyBinaryTreeNode<Library> ?rootNode in binaryTreeSearchTreeBuilder.searchTreeRootNodes) {
                Console.WriteLine("-------------------------------------------");
                new MyBinaryTreePrinter<Library>(printerImpl, rootNode, numTreeLevels); // print BST
            }
        }
    }
}
