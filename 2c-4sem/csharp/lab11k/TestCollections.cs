using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace lab11k
{
    public class TestCollections : ConInFormat
    {
        LinkedList<Organization> collection1;
        Dictionary<string, Factory> collection2;

        /* get linked list node by index */
        public static bool LinkedListGetNodeByIndex<_Ty>(ref LinkedListNode<_Ty>? dest, LinkedList<_Ty> ?list, int index)
        {
            int                  i;
            LinkedListNode<_Ty>? node = null;
            int                  hfalfCount = list.Count >> 1;

            /* check */
            if (index < 0 || index >= list.Count)
                return false;

            if(index <= hfalfCount) {
                // if elem index less or equal half size, find from start
                node = list.First;
                for (i = 0; i < index; i++) {
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

        private void CollectionsAddElement(int uniqueid, string? name, string? street, string? worktime, int planmax, int plancur)
        {
            Organization organization = new Organization(name, street, worktime); // add new base class object
            Factory factory = new Factory(name, street, worktime, planmax, plancur); // add new derived class object
            collection1.AddLast(organization);
            collection2.Add($"{factory.ToString()}_{uniqueid}", factory);
        }

        private bool CollectionsRemoveElement(int elemidx)
        {
            /* delete node from linked list */
            LinkedListNode<Organization>? nodeToRemove = null;
            if (!LinkedListGetNodeByIndex<Organization>(ref nodeToRemove, collection1, elemidx))
            {
                Console.WriteLine("Failed to remove node with index {0} from {1}", elemidx, collection1.ToString());
                return false; // index out of bounds
            }
            collection1.Remove(nodeToRemove); //remove node

            /* remove from list */
            Factory empty = new Factory(); //create new object for get object name
            string? keystr = $"{empty.ToString()}_{elemidx}";
            Console.WriteLine("Delete element by key {0}", keystr);
            if (!collection2.Remove(keystr)) {
                Console.WriteLine("Failed to remove key-value with index {0} from {1}", elemidx, collection2.ToString());
                return false; // index out of bounds
            }
            return true; //OK
        }

        public TestCollections(int numberOfElements)
        {
            Random random = new Random();
            collection1 = new LinkedList<Organization>();
            collection2 = new Dictionary<string, Factory>();

            for (int i = 0; i < numberOfElements; i++) {
                int hour = random.Next(9, 13);
                int mins = random.Next(0, 50);
                int currhour = hour % 12;
                int minval = Math.Min(hour + 1, currhour);
                int maxval = Math.Max(hour + 1, currhour);
                int endhour = random.Next(minval, maxval);
                int endmins = random.Next(0, 50);
                int planmax = random.Next(10, 1000);
                int plancur = random.Next(0, planmax);
                string name = $"organization{i}";
                string street = $"street {i + 20}";
                string worktime = $"{hour}:{mins}-{endhour}:{endmins}";
                CollectionsAddElement(i, name, street, worktime, planmax, plancur);
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
            int          elemidx;
            Organization center = new Organization();
            Organization first = collection1.First();
            Organization end = collection1.Last();
            Organization noexists = new Organization("This Organization Is No Exists!", "Ababsdbsdb", "00:00-00:00");

            /* COLLECTION 1 */
            Console.WriteLine("COLLECTION 1");
            if (!LinkedListGetNodeDataByIndex<Organization>(ref center, collection1, collection1.Count >> 1)) {
                Console.WriteLine("Index out of bounds!");
                return;
            }
            Stopwatch stopwatch = new Stopwatch();
            stopwatch.Start();
            collection1.Contains(first);
            stopwatch.Stop();
            Console.WriteLine("1. find first element in collection1. elapsed time {0} ms", stopwatch.Elapsed);
            stopwatch.Reset();

            stopwatch.Start();
            collection1.Contains(center);
            stopwatch.Stop();
            Console.WriteLine("2. find center element in collection1. elapsed time {0} ms", stopwatch.Elapsed);
            stopwatch.Reset();

            stopwatch.Start();
            collection1.Contains(end);
            stopwatch.Stop();
            Console.WriteLine("3. find end element in collection1. elapsed time {0} ms", stopwatch.Elapsed);
            stopwatch.Reset();

            stopwatch.Start();
            collection1.Contains(noexists);
            stopwatch.Stop();
            Console.WriteLine("4. find no exists element in collection1. elapsed time {0} ms", stopwatch.Elapsed);
            stopwatch.Reset();

            Console.Write("\n\n");

            /* COLLECTION 2 */
            Console.WriteLine("COLLECTION 2");
            string keystr;
            Factory empty = new Factory(); //create new object for get object name

            keystr = $"{empty.ToString()}_0";
            stopwatch.Start();
            collection2.ContainsKey(keystr);
            stopwatch.Stop();
            Console.WriteLine("1. find first element in collection2. elapsed time {0} ms", stopwatch.Elapsed);
            stopwatch.Reset();

            int centeridx = collection2.Count >> 1;
            keystr = $"{empty.ToString()}_{centeridx}";
            stopwatch.Start();
            collection2.ContainsKey(keystr);
            stopwatch.Stop();
            Console.WriteLine("2. find center element in collection2. elapsed time {0} ms", stopwatch.Elapsed);
            stopwatch.Reset();

            keystr = $"{empty.ToString()}_{collection2.Count-1}";
            stopwatch.Start();
            collection2.ContainsKey(keystr);
            stopwatch.Stop();
            Console.WriteLine("3. find end element in collection2. elapsed time {0} ms", stopwatch.Elapsed);
            stopwatch.Reset();

            keystr = $"THIS IS NOT EXISTS KEY";
            stopwatch.Start();
            collection2.ContainsKey(keystr);
            stopwatch.Stop();
            Console.WriteLine("4. find no exists element in collection2. elapsed time {0} ms", stopwatch.Elapsed);
            stopwatch.Reset();

            Console.Write("\n\n");

            while (true) {
                PrintCollections();
                elemidx = (int)ReadLineWithDescription(FORMAT_TYPE.FORMAT_INT32, "Type element index from delete from collections");
                if(!CollectionsRemoveElement(elemidx)) {
                    Console.WriteLine("Failed to delete element with index {0} from collections", elemidx);
                    continue;
                }
                Console.WriteLine("Deleted element with index {0} from collections", elemidx);
            }
        }
    }
}