using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab11k
{
    public class TestCollections
    {
        List<Organization> list1;
        List<string> list2;
        Dictionary<Organization, Factory> list3;
        Dictionary<string, Factory> list4;

        public TestCollections(int numberOfElements)
        {
            Random random = new Random();
            list1 = new List<Organization>();
            list2 = new List<string>();
            list3 = new Dictionary<Organization, Factory>();
            list4 = new Dictionary<string, Factory>();

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

                Organization organization = new Organization(name, street, worktime);
                Factory factory = new Factory(name, street, worktime, planmax, plancur);
                list1.Add(organization);
                list2.Add(organization.ToString());
                list3.Add(organization, factory);
                list4.Add($"{factory.ToString()}_{i}", factory);
            }
        }

        public void FindTests()
        {
            Organization first = list1[0];
            Organization center = list1[list1.Count >> 1];
            Organization end = list1[list1.Count - 1];
            Organization noexists = new Organization("gogogo", "oaoaoao", "00:00-00:00");

            Stopwatch stopwatch = new Stopwatch();
            stopwatch.Start();
            list1.Contains(first);
            stopwatch.Stop();
            Console.WriteLine("1. find first element. elapsed time {0} ms", stopwatch.Elapsed);
            stopwatch.Reset();

            stopwatch.Start();
            list1.Contains(center);
            stopwatch.Stop();
            Console.WriteLine("2. find center element. elapsed time {0} ms", stopwatch.Elapsed);
            stopwatch.Reset();

            stopwatch.Start();
            list1.Contains(end);
            stopwatch.Stop();
            Console.WriteLine("3. find end element. elapsed time {0} ms", stopwatch.Elapsed);
            stopwatch.Reset();

            stopwatch.Start();
            list1.Contains(noexists);
            stopwatch.Stop();
            Console.WriteLine("4. find no exists element. elapsed time {0} ms", stopwatch.Elapsed);
            stopwatch.Reset();
        }
    }
}
