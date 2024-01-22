using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab14k
{
    using District = Queue<Organization>;
    using City = SortedDictionary<string, Queue<Organization>>;

    public class Lab
    {
        public Lab() {}

        public void Start()
        {
            var city = new City();
            var districts = new District();
            districts.Enqueue(new Organization("PSTU", "adr", "12-00"));
            districts.Enqueue(new Organization("Org", "adr", "12-00"));
            city.Add("Perm", districts);

            Work1(city);
            HorizontalLine();
            Work2(city);
            HorizontalLine();
            Work3(city);
            HorizontalLine();
            Work4(city);
            HorizontalLine();
            Work5(city);
        }

        public void HorizontalLine()
        {
            Console.WriteLine("\n\n------------------------\n");
        }

        // выборка
        public void Work1(City city)
        {
            Console.WriteLine("Select");
            var organisations1 = city.First().Value.Where(c => c.name == "PSTU").Select(o => o.GetFullInfo());
            var organisations2 = from c in city.First().Value
                                 where c.name == "PSTU"
                                 select c.GetFullInfo();

            Console.WriteLine(string.Join(',', organisations1.ToArray()));
            Console.WriteLine(string.Join(',', organisations2.ToArray()));
        }

        // Получение счетчика
        public void Work2(City city)
        {
            Console.WriteLine("Count");
            var organisations1Count = city.First().Value.Count(e => e.name == "PSTU");
            var organisations2Count = (from c in city.First().Value where c.name == "PSTU" select c).Count();

            Console.WriteLine(organisations1Count);
            Console.WriteLine(organisations1Count);
        }

        // Использование операций над множествами 
        public void Work3(City city)
        {
            Console.WriteLine("Intersection");
            var city2 = new City();
            var districts2 = new District();
            districts2.Enqueue(new Organization("PSTU-Corp2", "adr", "14-00"));
            city2.Add("Perm", districts2);

            var intersection1 = city.First().Value.Intersect(city2.First().Value, new AddressEqualityComparer());
            Console.WriteLine("Intersection1:");
            foreach (var item in intersection1)
            {
                Console.WriteLine(item);
            }

            var intersection2 = (from c in city.First().Value select c).Intersect(city2.First().Value, new AddressEqualityComparer());
            Console.WriteLine("Intersection2:");
            foreach (var item in intersection2)
            {
                Console.WriteLine(item);
            }
        }

        // Агрегирование данных 
        public void Work4(City city)
        {
            Console.WriteLine("Aggregate");
            var eurasiaStateAggregate1 = city.First().Value.Aggregate("|");
            var eurasiaStateAggregate2 = string.Join('|', (from c in city.First().Value select c.GetFullInfo()).ToArray());

            Console.WriteLine(eurasiaStateAggregate1);
            Console.WriteLine(eurasiaStateAggregate2);
        }

        // Группировка данных  
        public void Work5(City city)
        {
            Console.WriteLine("Group");

            var group1 = city.First().Value.GroupBy(e => e.address);
            Console.WriteLine("Group1:");
            foreach (var item in group1)
            {
                Console.WriteLine(item.Key + ":");
                foreach (var orgs in item)
                {
                    Console.WriteLine("\t" + orgs.GetFullInfo());
                }
            }


            var group2 = from c in city.First().Value group c by c.address;
            Console.WriteLine("\nGroup2:");
            foreach (var item in group2)
            {
                Console.WriteLine(item.Key + ":");
                foreach (var orgs in item)
                {
                    Console.WriteLine("\t" + orgs.GetFullInfo());
                }
            }
        }
    }
}
