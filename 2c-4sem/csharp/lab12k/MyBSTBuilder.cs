using System;
using System.Collections.Generic;
using System.Diagnostics.Metrics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
using System.Diagnostics;

namespace lab12k
{
    /* Binary Search Tree (BST) */
    public class MyBSTBuilder
    {
        public class NodeSearchContext {
            public char firstChar;
            public int  charIdx;

            public NodeSearchContext(char chr, int idx) {
                firstChar = chr;
                charIdx = idx;
            }
        }

        /* this class store all pathes to needed data elem */
        public class NodesWalkPath
        {
            public enum TREE_PATH_DIR
            {
                DIR_LEFT_NODE = 0,
                DIR_RIGHT_NODE = 1
            };

            private List<TREE_PATH_DIR>              nodesWalkDirections;
            private MyBinaryTreeNode<Library>?       endNodeRef;
            private NodeSearchContext                seatrchData;
            public  List<MyBinaryTreeNode<Library>?> ?skipRefs;

            public MyBinaryTreeNode<Library>? EndNodeRef {
                get {  return endNodeRef; }
            }

            public List<TREE_PATH_DIR> WalkDirections {
                get { return nodesWalkDirections; }
            }

            public NodesWalkPath(NodeSearchContext search) {
                nodesWalkDirections = new List<TREE_PATH_DIR>();
                seatrchData = search;
            }


            private bool NodeInSkipList(MyBinaryTreeNode<Library>? node)
            {
                for (int i = 0; i < skipRefs.Count; i++)
                    if (node == skipRefs[i])
                        return true;

                return false;
            }
            // returns true if needed element found in tree branch
            private bool SearchRecursive(MyBinaryTreeNode<Library>? node) {
                bool b_left_node_found;
                bool b_right_node_found;

                // node is not exists (end tree node)
                if (node == null) {
                    nodesWalkDirections.Clear(); //clear directions
                    return false; //break searching
                }

                string? str = node.data.GetOrgName();
                if (seatrchData.charIdx < str.Length && str[seatrchData.charIdx] == seatrchData.firstChar) {
                    if(!NodeInSkipList(node)) {
                        endNodeRef = node; // save found node ref0
                        return true; //break searching
                    }
                }

                // recursive search in left branch and in right branch
                // if needed value found in one on the branches, return true
                b_left_node_found = SearchRecursive(node.left);
                if (b_left_node_found)
                    nodesWalkDirections.Add(TREE_PATH_DIR.DIR_LEFT_NODE); // elem found in left node

                b_right_node_found = SearchRecursive(node.right);
                if(b_right_node_found)
                    nodesWalkDirections.Add(TREE_PATH_DIR.DIR_RIGHT_NODE); // elem found in right node

                Debug.Assert(!(b_left_node_found && b_right_node_found), "Impossible to find elements in both trees at once!");
                return b_left_node_found || b_right_node_found;
            }

            public bool Search(MyBinaryTreeNode<Library>? rootNode, List<MyBinaryTreeNode<Library>?> ?skipList) {
                // recursive search in left branch and in right branch
                skipRefs = skipList; 
                Debug.Assert(rootNode != null, "rootNode must be not null");
                return SearchRecursive(rootNode.left) || SearchRecursive(rootNode.right);
            }
        }

        private NodeSearchContext                         search;
        public readonly List<NodesWalkPath?>              walkPathes;
        public readonly List<MyBinaryTreeNode<Library>?>  searchTreeRootNodes;

        private bool SearchAllPaths(MyBinaryTreeNode<Library>? rootNode, NodeSearchContext search)
        {
            List<MyBinaryTreeNode<Library>?> skipNodesList = new List<MyBinaryTreeNode<Library>?>();

            // 1-st search
            // if this call walked on all tree and not found needed elem, break searching
            NodesWalkPath firstWalk = new NodesWalkPath(search);
            if (!firstWalk.Search(rootNode, skipNodesList)) {
                //There are links in the sheet but the function returned false (not found)
                Debug.Assert(firstWalk.EndNodeRef == null, "Ooops! Something went wrong! Search returned false (elements not found) but firstWalk.EndNodeRef != null");
                return false; // Eelement with needed info not found in all tree. Return false
            }
            walkPathes.Add(firstWalk);

            // next searches
            // Needed element found in tree and this node reference saved in the list. It is one of the pathes :)
            // Continue searching from current node next...
            for (int i = 0; i < walkPathes.Count; i++) {
                NodesWalkPath newWalk = new NodesWalkPath(search);
                if(walkPathes[i].EndNodeRef != null) {
                    if (newWalk.Search(walkPathes[i].EndNodeRef, skipNodesList)) {
                        walkPathes.Add(newWalk);
                        Console.WriteLine("");
                        i = 0; // repeat cycle from start and break current iteration
                        break;
                    }
                }
            }
            Console.WriteLine("In binary tree searched {0} pathes", walkPathes.Count);
            return true;
        }

        private bool SearchPathWithIgnoreExistsRefs(MyBinaryTreeNode<Library>? rootNode, List<MyBinaryTreeNode<Library>?> ? skipNodesList)
        {
            NodesWalkPath firstWalk = new NodesWalkPath(search);
            if (firstWalk.Search(rootNode, skipNodesList)) {
                //There are links in the sheet but the function returned false (not found)
                Debug.Assert(firstWalk.EndNodeRef == null, "Ooops! Something went wrong! Search returned false (elements not found) but firstWalk.EndNodeRef != null");
                return false; // Eelement with needed info not found in all tree. Return false
            }
            walkPathes.Add(firstWalk);
            skipNodesList.Add(firstWalk.EndNodeRef);
            return true;
        }

        private bool SearchAllPaths2(MyBinaryTreeNode<Library>? rootNode, NodeSearchContext search)
        {
            List<MyBinaryTreeNode<Library>?> skipNodesList = new List<MyBinaryTreeNode<Library>?>();
            while (SearchPathWithIgnoreExistsRefs(rootNode, skipNodesList));
            Console.WriteLine("In binary tree searched {0} pathes", walkPathes.Count);
            return true;
        }

        public void BuildSearchTrees() {
            /* search path from end node to root node and generate new search tree */
            for (int i = 0; i < walkPathes.Count; i++) {
                NodesWalkPath? walkPath = walkPathes[i];

                Debug.Assert(walkPath != null, "walkPath was null");
                MyBinaryTreeNode<Library>? endSrcNode = walkPath.EndNodeRef;
                Debug.Assert(endSrcNode != null, "endSrcNode was null");
                Console.WriteLine("walkPath.WalkDirections.Count = {0}", walkPath.WalkDirections.Count);

                //TODO: generate seatch tree by direction info and root nodes info
                MyBinaryTreeNode<Library>? mySrcRoot = null;
                MyBinaryTreeNode<Library>? newNode = null;
                MyBinaryTreeNode<Library>? previousRootNode = null;

                if(endSrcNode != null) {
                    newNode = new MyBinaryTreeNode<Library>(endSrcNode.data);
                    while (endSrcNode != null) {

                        // this node is chlid?!
                        mySrcRoot = endSrcNode.root;
                        if (mySrcRoot != null) {
                            // this node is child
                            // create parent for me and set needed link to my ref
                            MyBinaryTreeNode<Library>? myNewRoot = new MyBinaryTreeNode<Library>(mySrcRoot.data);

                            /* solve node connection */
                            if (mySrcRoot.left == endSrcNode)
                            {
                                myNewRoot.left = newNode;
                            }
                            else if (mySrcRoot.right == endSrcNode)
                            {
                                myNewRoot.right = newNode;
                            }
                            else
                            {
                                Debug.Assert(false, "Unexpected end of tree branch");
                            }
                            newNode = myNewRoot;
                            previousRootNode = newNode;
                        }
                        else {
                            // first root node (no parents)
                            searchTreeRootNodes.Add(previousRootNode);
                            Console.WriteLine("Search tree added. Trees count {0}", searchTreeRootNodes.Count);
                            break;
                        }
                        endSrcNode = mySrcRoot;
                    }
                }

            }
        }

        //public void BuildSearchTrees2()
        //{
        //    /* search path from end node to root node and generate new search tree */
        //    for (int i = 0; i < walkPathes.Count; i++)
        //    {
        //        NodesWalkPath? walkPath = walkPathes[i];
        //        Debug.Assert(walkPath != null, "walkPath was null");
        //        MyBinaryTreeNode<Library>? endSrcNode = walkPath.EndNodeRef;
        //        Debug.Assert(endSrcNode != null, "endSrcNode was null");

        //        MyBinaryTreeNode<Library>? newNode = null;
        //        MyBinaryTreeNode<Library>? newRoot = null;
        //        MyBinaryTreeNode<Library>? parent = null;

        //        while (endSrcNode != null)
        //        {
        //            newNode = new MyBinaryTreeNode<Library>(endSrcNode.data);

        //            if (newRoot == null)
        //            {
        //                newRoot = newNode; // This is the root of the new tree
        //            }
        //            else
        //            {
        //                // Determine whether the current node is the left or right child of its parent
        //                if (parent.left == endSrcNode)
        //                {
        //                    parent.left = newNode;
        //                }
        //                else if (parent.right == endSrcNode)
        //                {
        //                    parent.right = newNode;
        //                }
        //                else
        //                {
        //                    Debug.Assert(false, "Unexpected end of tree branch");
        //                }
        //            }

        //            parent = newNode; // The current new node will be the parent in the next iteration
        //            endSrcNode = endSrcNode.root; // Move up to the parent in the original tree
        //        }

        //        // Add the root of the new tree to the list of search tree root nodes
        //        searchTreeRootNodes.Add(newRoot);
        //    }
        //}

        public MyBSTBuilder(MyBinaryTreeNode<Library>? rootNode, char firstSym, int charIdx = 0) {
            search = new NodeSearchContext(firstSym, charIdx);
            walkPathes = new List<NodesWalkPath?>();
            searchTreeRootNodes = new List<MyBinaryTreeNode<Library>?>();
            if (SearchAllPaths(rootNode, search))
                //if (SearchAllPaths2(rootNode, search))
                BuildSearchTrees();
        }
    }
}
