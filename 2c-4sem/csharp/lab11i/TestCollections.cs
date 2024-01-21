using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace lab11i
{
    public class TestCollections : ConInFormat
    {
        LinkedList<State> collection1;
        SortedDictionary<string, Republic> collection2;

        /* get linked list node by index */
        public static bool LinkedListGetNodeByIndex<_Ty>(ref LinkedListNode<_Ty>? dest, LinkedList<_Ty>? list, int index)
        {
            int i;
            LinkedListNode<_Ty>? node = null;
            int hfalfCount = list.Count >> 1;

            /* check */
            if (index < 0 || index >= list.Count)
                return false;

            if (index <= hfalfCount)
            {
                // if elem index less or equal half size, find from start
                node = list.First;
                for (i = 0; i < index; i++)
                {
                    if (node == null)
                        return false;

                    node = node.Next;
                }
            }
            else
            {
                // find from end if elem index greater half size
                node = list.Last;
                for (i = list.Count - 1; i > index; i--)
                {
                    if (node == null)
                        return false;

                    node = node.Previous;
                }
            }
            dest = node;
            return true;
        }

        public static bool LinkedListGetNodeDataByIndex<_Ty>(ref _Ty dest, LinkedList<_Ty>? list, int index)
        {
            LinkedListNode<_Ty>? node = null;
            if (!LinkedListGetNodeByIndex<_Ty>(ref node, list, index))
                return false;

            dest = node.Value;
            return true;
        }

        private void CollectionsAddElement(int uniqueid, string? name, string? govermentForm)
        {
            State state = new State(name, govermentForm); // add new base class object
            Republic republic = new Republic(name); // add new derived class object
            collection1.AddLast(state);
            collection2.Add($"{republic.ToString()}_{uniqueid}", republic);
        }

        private bool CollectionsRemoveElement(int elemidx)
        {
            /* delete node from linked list */
            LinkedListNode<State>? nodeToRemove = null;
            if (!LinkedListGetNodeByIndex<State>(ref nodeToRemove, collection1, elemidx))
            {
                Console.WriteLine("Failed to remove node with index {0} from {1}", elemidx, collection1.ToString());
                return false; // index out of bounds
            }
            collection1.Remove(nodeToRemove); //remove node

            /* remove from list */
            Republic empty = new Republic(""); //create new object for get object name
            string? keystr = $"{empty.ToString()}_{elemidx}";
            Console.WriteLine("Delete element by key {0}", keystr);
            if (!collection2.Remove(keystr))
            {
                Console.WriteLine("Failed to remove key-value with index {0} from {1}", elemidx, collection2.ToString());
                return false; // index out of bounds
            }
            return true; //OK
        }

        public TestCollections(int numberOfElements)
        {
            Random random = new Random();
            collection1 = new LinkedList<State>();
            collection2 = new SortedDictionary<string, Republic>();

            for (int i = 0; i < numberOfElements; i++)
            {
                string name = $"State{i}";
                string govermentForm = $"govermentForm{i}";
                CollectionsAddElement(i, name, govermentForm);
            }
        }

        private void PrintCollections()
        {
            /* enum collection keys */
            foreach (string? key in collection2.Keys)
                Console.WriteLine("key '{0}'", key);
        }

        public void FindTests()
        {
            int elemidx;
            State center = new State("State", "GovermentForm");
            State first = collection1.First();
            State end = collection1.Last();
            State noexists = new State("This state Is No Exists!", "123");

            if (!LinkedListGetNodeDataByIndex<State>(ref center, collection1, collection1.Count >> 1))
            {
                Console.WriteLine("Index out of bounds!");
                return;
            }
            Stopwatch stopwatch = new Stopwatch();
            stopwatch.Start();
            collection1.Contains(first);
            stopwatch.Stop();
            Console.WriteLine("1. find first element. elapsed time {0} ms", stopwatch.Elapsed);
            stopwatch.Reset();

            stopwatch.Start();
            collection1.Contains(center);
            stopwatch.Stop();
            Console.WriteLine("2. find center element. elapsed time {0} ms", stopwatch.Elapsed);
            stopwatch.Reset();

            stopwatch.Start();
            collection1.Contains(end);
            stopwatch.Stop();
            Console.WriteLine("3. find end element. elapsed time {0} ms", stopwatch.Elapsed);
            stopwatch.Reset();

            stopwatch.Start();
            collection1.Contains(noexists);
            stopwatch.Stop();
            Console.WriteLine("4. find no exists element. elapsed time {0} ms", stopwatch.Elapsed);
            stopwatch.Reset();

            Console.Write("\n\n --------------\n\n ");

            Republic center_republic = this.GetCenterElement(collection2).Value;
            Republic first_republic = collection2.First().Value;
            Republic end_republic = collection2.Last().Value;
            Republic noexists_republic = new Republic("This state Is No Exists!");

            stopwatch = new Stopwatch();
            stopwatch.Start();
            collection2.ContainsValue(first_republic);
            stopwatch.Stop();
            Console.WriteLine("1. find first element. elapsed time {0} ms", stopwatch.Elapsed);
            stopwatch.Reset();

            stopwatch.Start();
            collection2.ContainsValue(center_republic);
            stopwatch.Stop();
            Console.WriteLine("2. find center element. elapsed time {0} ms", stopwatch.Elapsed);
            stopwatch.Reset();

            stopwatch.Start();
            collection2.ContainsValue(end_republic);
            stopwatch.Stop();
            Console.WriteLine("3. find end element. elapsed time {0} ms", stopwatch.Elapsed);
            stopwatch.Reset();

            stopwatch.Start();
            collection2.ContainsValue(noexists_republic);
            stopwatch.Stop();
            Console.WriteLine("4. find no exists element. elapsed time {0} ms", stopwatch.Elapsed);
            stopwatch.Reset();

            Console.Write("\n\n");

            while (true)
            {
                PrintCollections();
                elemidx = (int)ReadLineWithDescription(FORMAT_TYPE.FORMAT_INT32, "Type element index from delete from collections");
                if (!CollectionsRemoveElement(elemidx))
                {
                    Console.WriteLine("Failed to delete element with index {0} from collections", elemidx);
                    continue;
                }
                Console.WriteLine("Deleted element with index {0} from collections", elemidx);
            }
        }

        private KeyValuePair<K, V> GetCenterElement<K, V>(SortedDictionary<K, V> dictionary)
        {
            int middleIndex = dictionary.Count / 2;

            return dictionary.ElementAt(middleIndex);
        }
    }
}