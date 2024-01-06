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

            private List<TREE_PATH_DIR>        nodesWalkDirections;
            private MyBinaryTreeNode<Library>? endNodeRef;
            private NodeSearchContext          seatrchData;

            public MyBinaryTreeNode<Library>? EndNodeRef {
                get {  return endNodeRef; }
            }

            public List<TREE_PATH_DIR> WalkDirections {
                get { return nodesWalkDirections; }
            }

            public NodesWalkPath(NodeSearchContext search) {
                seatrchData = search;
            }

            // returns true if needed element found in tree branch
            private bool SearchRecursive(MyBinaryTreeNode<Library>? node) {
                bool b_left_node_found;
                bool b_right_node_found;

                // node is not exists (end nodes)
                if (node == null) {
                    nodesWalkDirections.Clear(); //clear directions
                    return false; //break searching
                }

                string? str = node.data.GetOrgName();
                if (seatrchData.charIdx < str.Length && str[seatrchData.charIdx] == seatrchData.firstChar) {
                    endNodeRef = node; // save found node ref
                    return true; //break searching
                }

                // recursive search in left branch and in right branch
                // if needed value found in one on the branches, return true
                b_left_node_found = SearchRecursive(node.left);
                if (b_left_node_found)
                    nodesWalkDirections.Add(TREE_PATH_DIR.DIR_LEFT_NODE); // elem found in left node

                b_right_node_found = SearchRecursive(node.right);
                if(b_right_node_found)
                    nodesWalkDirections.Add(TREE_PATH_DIR.DIR_RIGHT_NODE); // elem found in right node

                Debug.Assert(!(b_left_node_found && b_right_node_found), "It is impossible to find elements in both trees at once!");
                return b_left_node_found || b_right_node_found;
            }

            public bool Search(MyBinaryTreeNode<Library>? rootNode) {
                // recursive search in left branch and in right branch
                return SearchRecursive(rootNode.left) || SearchRecursive(rootNode.right);
            }
        }

        char                                             firstChar;
        int                                              charIdx;
        public readonly List<NodesWalkPath?>              walkPathes;
        public readonly List<MyBinaryTreeNode<Library>?> searchTreeRootNodes;

        private bool SearchAllPaths(MyBinaryTreeNode<Library>? rootNode, NodeSearchContext search)
        {
            // 1-st search
            // if this call walked on all tree and not found needed elem, break searching
            NodesWalkPath firstWalk = new NodesWalkPath(search);
            if (!firstWalk.Search(rootNode)) {
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
                if (walkPathes[i].EndNodeRef != null && newWalk.Search(walkPathes[i].EndNodeRef)) {
                    walkPathes.Add(newWalk);
                    i = 0; // repeat cycle from start and break current iteration
                    break;
                }
            }
            Console.WriteLine("In binary tree searched {0} pathes", walkPathes.Count);
            return true;
        }

        public void BuildSearchTrees() {
            /* search path from end node to root node and generate new search tree */
            MyBinaryTreeNode<Library>? searchNewTreeRootNode = new MyBinaryTreeNode<Library>(); // is is root node
            for (int i = 0; i < walkPathes.Count; i++) {
                NodesWalkPath? walkPath = walkPathes[i];

                //TODO: generate seatch tree by direction info and root nodes info

            }
        }

        public MyBSTBuilder(MyBinaryTreeNode<Library>? rootNode, char firstSym, int charIdx = 0) {
            firstChar = firstSym;
            charIdx = charIdx;
            if (SearchAllPaths(rootNode))
                BuildSearchTrees();
        }
    }
}
