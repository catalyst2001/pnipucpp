using System;

namespace _10_3
{
    public class MyComparer : IComparer<Organization>
    {
        public int Compare(Organization x, Organization y)
        {
            return x.GetOrganizationId().CompareTo(y.GetOrganizationId());
        }
    }

    public class Program
    {
        public static void PrintOrgs(Organization[] organizations)
        {
            foreach (Organization org in organizations)
                Console.WriteLine(org.GetFullInfo() + " ORGID: " + org.GetOrganizationId());

            Console.WriteLine();
        }

        public static int BinSearch(Organization[] array, int value)
        {
            int left = 0;
            int right = array.Length - 1;
            int middle;

            while (left <= right)
            {
                middle = (left + right) / 2;
                if (array[middle].GetOrganizationId() == value)
                {
                    return middle; //found
                }
                else if (array[middle].GetOrganizationId() < value)
                {
                    left = middle + 1;
                }
                else
                {
                    right = middle - 1;
                }
            }
            return -1;
        }


        static void Main(string[] args)
        {
            //--------------------------------------------
            Organization[] array = new Organization[] {
                new Factory("Системы отопления", "ул. Аносова 2", "09:00-20:00", 12000, 10000),
                new InsuranceCompany("Страховое АО ВСК", "ул. 25 Октября", "09:00–18:00", 254003),
                new ShipbuildingCompany("Hanse LLC", "10600 1349/127 Charoennakorn Road", "10AM-10PM", 18000, 6000, 24000, 2400),
                new Library("Библиотека", "ул. Смирнова", "12:00-18:00", 1400)
            };
            PrintOrgs(array);

            //--------------------------------------------
            foreach (IRandomInit rndinit in array)
                rndinit.RandomInit();

            PrintOrgs(array);

            //--------------------------------------------
            Array.Sort(array);
            PrintOrgs(array);

            //--------------------------------------------
            Array.Sort(array, new MyComparer());
            PrintOrgs(array);

            //--------------------------------------------
            int elem_to_find = array[array.Length - 1].GetOrganizationId();
            int idx = BinSearch(array, elem_to_find);
            Console.WriteLine("Elem to find: {0} == Found: {1} of index {2}", elem_to_find,
               array[idx].GetOrganizationId(), idx);

            //--------------------------------------------
            Organization array2 = new Organization("Org2", "адрес", "время работы");
            Console.WriteLine(array2.GetFullInfo());

            Organization clone = (Organization)array2.Clone();
            Console.WriteLine(clone.GetFullInfo());

        }
    }
}