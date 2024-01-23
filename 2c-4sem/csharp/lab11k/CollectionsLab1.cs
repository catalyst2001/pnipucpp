using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab11k
{
    public class CollectionsLab1 : ConInFormat
    {
        private Stack<Organization> organizations;
        public CollectionsLab1()
        {
            organizations = new Stack<Organization>();
        }

        private enum ORG_TYPE
        {
            ORGANIZATION = 0,
            FACTORY,
            INSURANCE_COMPANY,
            SHIPBUILDING_COMPANY,
            LIBRARY
        };

        protected void InsertDefaultOrgs()
        {
            organizations.Push(new Organization("Организация1", "ул. Петрова 44/2", "10:00-22:00"));
            organizations.Push(new Factory("Системы отопления", "ул. Аносова 2", "09:00-20:00", 12000, 10000));
            organizations.Push(new InsuranceCompany("Страховое АО ВСК", "ул. 25 Октября", "09:00–18:00", 254003));
            organizations.Push(new ShipbuildingCompany("Hanse LLC", "10600 1349/127 Charoennakorn Road", "10AM-10PM", 18000, 6000, 24000, 2400));
            organizations.Push(new Library("Библиотека", "ул. Смирнова", "12:00-18:00", 1400));
        }

        void PrintOrganizationObjects()
        {
            object[] objects = GetObjects();
            Console.WriteLine("List of objects:");
            for (int i = 0; i < objects.Length; i++)
                Console.WriteLine("  {0}. '{1}' object", i, objects[i].ToString());
        }

        Organization CreateOrganization(ORG_TYPE type)
        {
            Organization orgobj;
            switch (type)
            {
                default:
                case ORG_TYPE.ORGANIZATION:
                    orgobj = new Organization();
                    break;
                case ORG_TYPE.FACTORY:
                    orgobj = new Factory();
                    break;
                case ORG_TYPE.INSURANCE_COMPANY:
                    orgobj = new InsuranceCompany();
                    break;
                case ORG_TYPE.SHIPBUILDING_COMPANY:
                    orgobj = new ShipbuildingCompany();
                    break;
                case ORG_TYPE.LIBRARY:
                    orgobj = new Library();
                    break;
            }
            orgobj.FillInfo();
            return orgobj;
        }

        private void PrintMenu()
        {
            Console.WriteLine(
                "0. Exit\n" +
                "1. Print menu\n" +
                "2. Add objects to collection\n" +
                "3. Remove object from collection\n" +
                "4. List objects in collection\n" +
                "5. Count specific objects\n" +
                "6. Print specific objects\n" +
                "7. Foreach print\n" +
                "8. Copy and sort\n" +
                "9. Find company in collection\n\n"
               );
        }

        private void PrintObjectsInCollection()
        {
            Console.WriteLine("List of objects in collection:");
            Organization[] arr = organizations.ToArray();
            Array.Reverse(arr);
            for (int i = 0; i < arr.Length; i++)
            {
                Console.WriteLine(" [{0}] object name: {1}, full info: {2}", i, arr[i].ToString(), arr[i].GetFullInfo());
            }
        }

        private void InsertNewOrgs()
        {
            /* add new organizations to collection */
            int count = (int)ReadLineWithDescription(FORMAT_TYPE.FORMAT_INT32, "enter number of objects");
            for (int i = 0; i < count; i++)
            {
                int orgtype = (int)ReadLineWithDescription(FORMAT_TYPE.FORMAT_INT32, "enter object index");
                Organization org = CreateOrganization((ORG_TYPE)orgtype);
                organizations.Push(org);
                Console.WriteLine("Element added on index {0}", organizations.Count - 1);
            }
        }

        private object[] GetObjects()
        {
            object[] objects = {
                new Organization(),
                new Factory(),
                new InsuranceCompany(),
                new ShipbuildingCompany(),
                new Library()
            };
            return objects;
        }

        public Stack<T> SortStack<T>(Stack<T> stack) where T : IComparable<T>
        {
            T[] elements = stack.ToArray();
            Array.Sort(elements);
            Array.Reverse(elements);
            return new Stack<T>(elements);
        }

        private bool FindObjectByName(ref object dst, string? name)
        {
            object[] objects = GetObjects();
            foreach (object obj in objects)
            {
                if (name == obj.ToString())
                {
                    dst = obj;
                    return true;
                }
            }
            return false;
        }

        private void PrintSpecificObjectsByObjectName(string? objname)
        {
            Organization[] arr = organizations.ToArray();
            Array.Reverse(arr);
            for (int i = 0; i < arr.Length; i++)
            {
                if (arr[i].ToString() == objname)
                {
                    Console.WriteLine(" [{0}] object name: {1}, full info: {2}", i, arr[i].ToString(), arr[i].GetFullInfo());
                }
            }
        }

        private int CountObjectsByObjectName(string? objname)
        {
            int count = 0;
            Organization[] arr = organizations.ToArray();
            for (int i = 0; i < arr.Length; i++)
            {
                if (arr[i].ToString() == objname)
                {
                    count++;
                }
            }
            return count;
        }

        private static Stack<_Ty> RemoveFromStack<_Ty>(Stack<_Ty> previous_stack, int elem_index)
        {
            Stack<_Ty> newStack = new Stack<_Ty>();
            _Ty[] arr = previous_stack.ToArray();
            Array.Reverse(arr);
            for (int i = 0; i < arr.Length; i++)
                if (i != elem_index)
                    newStack.Push(arr[i]);

            newStack.Reverse();
            return newStack;
        }

        private void DeleteOrg()
        {
            int idx;
            do
            {
                idx = (int)ReadLineWithDescription(FORMAT_TYPE.FORMAT_INT32, "type a index of object in collection");
                try
                {
                    organizations = RemoveFromStack<Organization>(organizations, idx);
                    Console.WriteLine("OK  Element on index {0} deleted.", idx);
                    PrintObjectsInCollection();
                    return;
                }
                catch (ArgumentOutOfRangeException)
                {
                    Console.WriteLine("Out of range index. Retype please");
                }
            } while (true);
        }

        private void PrintSpecificObjects()
        {
            string objname = (string)ReadLineWithDescription(FORMAT_TYPE.FORMAT_STRING, "type object name");
            Console.WriteLine("Printing objects with name {0}:", objname);
            PrintSpecificObjectsByObjectName(objname);
        }

        private void CountSpecificObjects()
        {
            string objname = (string)ReadLineWithDescription(FORMAT_TYPE.FORMAT_STRING, "type object name");
            Console.WriteLine(" {0} objects with name {1}:", CountObjectsByObjectName(objname), objname);
        }

        static private void PrintForeach(Stack<Organization> orgsList)
        {
            Organization[] arr = orgsList.ToArray();
            Array.Reverse(arr);
            foreach (Organization org in arr)
                Console.WriteLine("object name: {0}, full info: {1}", org.ToString(), org.GetFullInfo());

            Console.Write("\n");
        }

        private void CloneAndSortCollection()
        {
            Console.WriteLine("Source elements:");
            PrintForeach(organizations); //print original
            Stack<Organization> sortedСollectionCopy = SortStack<Organization>(new Stack<Organization>(organizations));
            Console.WriteLine("Sorted elements copy:");
            PrintForeach(sortedСollectionCopy); //print sorted copy
        }

        private void FindCompanyInCollectionByName()
        {
            int foundCount = 0;
            string company = (string)ReadLineWithDescription(FORMAT_TYPE.FORMAT_STRING, "type company name for search in collection");
            foreach (Organization org in organizations)
            {
                if (company.CompareTo(org.GetOrgName()) == 0)
                {
                    Console.WriteLine(org.GetFullInfo());
                    foundCount++;
                }
            }

            if (foundCount == 0)
                Console.WriteLine("Not found result for this search\n");
        }

        private void BeginMenu()
        {
            int menuItem;
            while (true)
            {
                menuItem = (int)ReadLineWithDescription(FORMAT_TYPE.FORMAT_INT32, ">>");
                switch (menuItem)
                {
                    case 0:
                        Console.WriteLine("performing exiting");
                        return;

                    case 1:
                        PrintMenu();
                        break;

                    case 2:
                        InsertNewOrgs();
                        break;

                    case 3:
                        DeleteOrg();
                        break;

                    case 4:
                        PrintObjectsInCollection();
                        break;

                    case 5:
                        PrintSpecificObjects();
                        break;

                    case 6:
                        CountSpecificObjects();
                        break;

                    case 7:
                        PrintForeach(organizations);
                        break;

                    case 8:
                        CloneAndSortCollection();
                        break;

                    case 9:
                        FindCompanyInCollectionByName();
                        break;
                }
            }


        }

        public void Start()
        {
            PrintOrganizationObjects();
            InsertDefaultOrgs();
            PrintMenu();
            BeginMenu();
        }
    }
}
