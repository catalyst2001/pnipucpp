using Microsoft.Win32;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using static System.Runtime.InteropServices.JavaScript.JSType;

namespace lab11i
{
    public class Work1
    {
        ArrayList states;

        public Work1()
        { 
            states = new ArrayList();
        }

        private bool removeFromCollection(int index)
        {
            if (index >= states.Count)
                return false;

            states.RemoveAt(index);
            return true;
        }

        private void fillCollection()
        {
            states.Add(new State("Kazakstan", "Republic"));
            states.Add(new Republic("Belarus"));
            states.Add(new Monarchy("UK"));
            states.Add(new Kingdom("Spain"));
        }

        private void printObjects()
        {
            Console.WriteLine("-- objects --");
            string lines =
                "1. State\n" +
                "2. Republic\n" +
                "3. Monarchy\n" +
                "4. Kingdom\n";
            Console.WriteLine(lines);
        }

        private void printObjectsList()
        {
            Console.WriteLine("-- objects list --");
            for (int i = 0; i < states.Count; i++)
                Console.WriteLine("[{0}] object {1}", i, states[i].ToString());
        }

        private void addObjectsMenu()
        {
            printObjects();
            printObjectsList();
            Console.Write("Type object: ");
            int menuItem = ConsoleRead.readInt32();
            switch (menuItem)
            {
                case 1:
                    states.Add(new State("Kazakstan", "Republic"));
                    break;
                        
                case 2:
                    states.Add(new Republic("Belarus"));
                    break;

                case 3:
                    states.Add(new Monarchy("UK"));
                    break;

                case 4:
                    states.Add(new Kingdom("Spain"));
                    break;
            }
        }

        private void deleteObjectsMenu()
        {
            printObjectsList();
            Console.Write("Type object index for deleting: ");
            int objectIndex = ConsoleRead.readInt32();
            if (removeFromCollection(objectIndex))
            {
                Console.WriteLine("Object width index {0} deleted OK", objectIndex);
                return;
            }
            Console.WriteLine("ERROR. failed to delete object with index {0}!", objectIndex);
        }

        private void printMenu()
        {
            Console.WriteLine("1. Add new objects");
            Console.WriteLine("2. Delete objects");
            Console.WriteLine("3. List objects in collection");
            Console.WriteLine("4. Print specific objects");
            Console.WriteLine("5. Count specific objects");
            Console.WriteLine("6. Foreach print");
            Console.WriteLine("7. Copy and sort");
            Console.WriteLine("8. Find state in collection");
            Console.WriteLine("9. Exit");
        }

        public void main()
        {
            fillCollection();
            while (true)
            {
                printMenu();
                int menuItem = ConsoleRead.readInt32();
                switch (menuItem)
                {
                    case 1:
                        addObjectsMenu();
                        break;

                    case 2:
                        deleteObjectsMenu();
                        break;

                    case 3:
                        PrintObjectsInCollection();
                        break;

                    case 4:
                        PrintSpecificObjects();
                        break;

                    case 5:
                        CountSpecificObjects();
                        break;

                    case 6:
                        PrintForeach(states);
                        break;

                    case 7:
                        CloneAndSortCollection();
                        break;

                    case 8:
                        FindStateInCollectionByName();
                        break;

                    case 9:
                        return; //exit
                }
            }
        }

        private void PrintObjectsInCollection()
        {
            Console.WriteLine("List of objects in collection:");
            for (int i = 0; i < states.Count; i++)
            {
                Console.WriteLine(" [{0}] object name: {1}, full info: {2}", i, states[i].ToString(), ((State)states[i]).GetFullInfo());
            }
        }

        private void PrintSpecificObjects()
        {
            Console.WriteLine("type object name:");
            string objectName = Console.ReadLine();
            Console.WriteLine("Printing objects with name {0}:", objectName);
            for (int i = 0; i < states.Count; i++)
            {
                if (((State)states[i]).GetName() == objectName)
                {
                    Console.WriteLine(" [{0}] object name: {1}, full info: {2}", i, states[i].ToString(), ((State)states[i]).GetFullInfo());
                }
            }
        }

        private void CountSpecificObjects()
        {
            Console.WriteLine("type object name:");
            string objectName = Console.ReadLine();
            Console.WriteLine(" {0} objects with name {1}", states.ToArray().Count(s => ((State)s).GetName() == objectName), objectName);
        }

        static private void PrintForeach(ArrayList array)
        {
            foreach (object state in array.ToArray())
                Console.WriteLine("object name: {0}, full info: {1}", state.ToString(), ((State)state).GetFullInfo());

            Console.Write("\n");
        }

        private void CloneAndSortCollection()
        {
            Console.WriteLine("Source elements:");
            PrintForeach(states); //print original
            states.Sort();
            ArrayList sortedСollectionCopy = new ArrayList(states);
            Console.WriteLine("Sorted elements copy:");
            PrintForeach(sortedСollectionCopy); //print sorted copy
        }

        private void FindStateInCollectionByName()
        {
            Console.WriteLine("type state name for search in collection:");
            int foundCount = 0;
            string stateName = Console.ReadLine();
            foreach (State state in states)
            {
                if (state.CompareTo(state.GetName()) == 0)
                {
                    Console.WriteLine(state.GetFullInfo());
                    foundCount++;
                }
            }

            if (foundCount == 0)
                Console.WriteLine("Not found result for this search\n");
        }

    }
}
