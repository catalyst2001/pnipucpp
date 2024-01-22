using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab14i
{
    using Continent = Dictionary<string, State>;
    using Earth = Queue<Dictionary<string, State>>;

    public class Lab
    {
        public Lab() {}

        public void Start()
        {
            var earth = new Earth();
            var continents = new Continent()
            {
                ["Eurasia"] = new State("Kazakstan", "Республика"),
                ["Australia"] = new Kingdom("Australia"),
                ["Africa"] = new Republic("Niger"),
            };
            earth.Enqueue(continents);

            Work1(earth);
            HorizontalLine();
            Work2(earth);
            HorizontalLine();
            Work3(earth);
            HorizontalLine();
            Work4(earth);
            HorizontalLine();
            Work5(earth);
        }

        public void HorizontalLine()
        {
            Console.WriteLine("\n\n------------------------\n");
        }

        // выборка
        public void Work1(Earth earth)
        {
            Console.WriteLine("Select");
            var eurasiaState1 = earth.First().Where(c => c.Key == "Eurasia").Select(c => c.Value.GetName()).ToList();
            var eurasiaState2 = from continent in earth.First()
                                where continent.Key == "Eurasia"
                                select continent.Value.GetName();

            Console.WriteLine(string.Join(',', eurasiaState1.ToArray()));
            Console.WriteLine(string.Join(',', eurasiaState2.ToArray()));
        }

        // Получение счетчика
        public void Work2(Earth earth)
        {
            Console.WriteLine("Count");
            var eurasiaStateCount1 = earth.First().Count(e => e.Key == "Eurasia");
            var eurasiaStateCount2 = (from continent in earth.First() where continent.Key == "Eurasia" select continent).Count();

            Console.WriteLine(eurasiaStateCount1);
            Console.WriteLine(eurasiaStateCount2);
        }

        // Использование операций над множествами 
        public void Work3(Earth earth)
        {
            Console.WriteLine("Intersection");
            var earth2 = new Earth();
            var continents2 = new Continent()
            {
                ["Africa"] = new Republic("Niger"),
            };
            earth2.Enqueue(continents2);

            var intersection1 = earth.First().Keys.Intersect(earth2.First().Keys);
            Console.WriteLine("Intersection1:");
            foreach (var item in intersection1)
            {
                Console.WriteLine(item);
            }

            var intersection2 = (from continent in earth.First() select continent.Key).Intersect(earth2.First().Keys);
            Console.WriteLine("Intersection2:");
            foreach (var item in intersection2)
            {
                Console.WriteLine(item);
            }
        }

        // Агрегирование данных 
        public void Work4(Earth earth)
        {
            Console.WriteLine("Aggregate");
            var eurasiaStateAggregate1 = earth.First().Aggregate("|");
            var eurasiaStateAggregate2 = (from continent in earth.First() select continent).ToDictionary().Aggregate("|");

            Console.WriteLine(eurasiaStateAggregate1);
            Console.WriteLine(eurasiaStateAggregate2);
        }

        // Группировка данных  
        public void Work5(Earth earth)
        {
            Console.WriteLine("Group");

            var group1 = earth.First().GroupBy(e => e.Value.GetGovernmentForm());
            Console.WriteLine("Group1:");
            foreach (var item in group1)
            {
                Console.WriteLine(item.Key + ":");
                foreach (var states in item)
                {
                    Console.WriteLine("\t" + states.Value.GetName());
                }
            }


            var group2 = from continent in earth.First() group continent by continent.Value.GetGovernmentForm();
            Console.WriteLine("\nGroup2:");
            foreach (var item in group2)
            {
                Console.WriteLine(item.Key + ":");
                foreach (var states in item)
                {
                    Console.WriteLine("\t" + states.Value.GetName());
                }
            }
        }
    }
}
