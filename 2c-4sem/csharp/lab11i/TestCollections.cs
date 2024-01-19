using System;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections.Generic;

namespace lab11i
{
    public class TestCollections
    {
        LinkedList<Organization> collection1;
        LinkedList<string> collection1Strings;
        Dictionary<string, Factory> collection2;


        Monarchy

        public TestCollections(int numberOfElements)
        {
            Random random = new Random();
            collection1 = new LinkedList<Organization>();
            collection2 = new Dictionary<string, Factory>();

            for (int i = 0; i < numberOfElements; i++)
            {
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
            int elemidx;
            Organization center = new Organization();
            Organization first = collection1.First();
            Organization end = collection1.Last();
            Organization noexists = new Organization("This Organization Is No Exists!", "Ababsdbsdb", "00:00-00:00");

            if (!LinkedListGetNodeDataByIndex<Organization>(ref center, collection1, collection1.Count >> 1))
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

            Console.Write("\n\n");

            while (true)
            {
                PrintCollections();

                Console.Write("type element index for delete: ");
                elemidx = ConsoleRead.readInt32();
                if (!CollectionsRemoveElement(elemidx))
                {
                    Console.WriteLine("Failed to delete element with index {0} from collections", elemidx);
                    continue;
                }
                Console.WriteLine("Deleted element with index {0} from collections", elemidx);
            }
        }
    }
}
