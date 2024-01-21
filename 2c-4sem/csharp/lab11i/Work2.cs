using Microsoft.Win32;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace lab11i
{
    public class Work2
    {
        Stack<State> states;

        public Work2()
        { 
            states = new Stack<State>();
        }

        private bool removeFromCollection(int index)
        {
            if (index >= states.Count)
                return false;

            Stack<State> stackCopy = new Stack<State>();
            State[] values = states.ToArray();
            for (int i = 0; i < values.Length; i++)
                if(i != index)
                    stackCopy.Push(values[i]);

            states = stackCopy;
            return true;
        }

        private void fillCollection()
        {
            states.Push(new State("State", "123"));
            states.Push(new Republic("Republic"));
            states.Push(new Monarchy("Monarchy"));
            states.Push(new Kingdom("Kingdom"));
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
            State[] values = states.ToArray();
            for (int i = 0; i < states.Count; i++)
                Console.WriteLine("[{0}] object {1}", i, values[i].ToString());
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
                    states.Push(new State("State", "123"));
                    break;
                        
                case 2:
                    states.Push(new Republic("Republic"));
                    break;

                case 3:
                    states.Push(new Monarchy("Monarchy"));
                    break;

                case 4:
                    states.Push(new Kingdom("Kingdom"));
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
            Console.WriteLine("6. Clone print");
            Console.WriteLine("6. Sort print");
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
                        PrintObjectsInStack();
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
                        CloneStack();
                        break;

                    case 8:
                        SortStack();
                        break;

                    case 9:
                        return; //exit
                }
            }
        }

        private void PrintObjectsInStack()
        {
            Console.WriteLine("List of objects in Stack:");
            var statesCopy = new Stack<State>(states);
            int i = 0;
            while (statesCopy.Count > 0)
            {
                State state = statesCopy.Pop();
                Console.WriteLine(" [{0}] object name: {1}, full info: {2}", i, state.ToString(), state.GetFullInfo());
                i++;
            }
        }

        private void PrintSpecificObjects()
        {
            Console.WriteLine("type object name:");
            string objectName = Console.ReadLine();

            var statesCopy = new Stack<State>(states);
            int i = 0;
            while (statesCopy.Count > 0)
            {
                State state = statesCopy.Pop();
                if (state.GetName() == objectName)
                {
                    Console.WriteLine(" [{0}] object name: {1}, full info: {2}", i, state.ToString(), state.GetFullInfo());
                }
                i++;
            }
        }

        private void CountSpecificObjects()
        {
            Console.WriteLine("type object name:");
            string objectName = Console.ReadLine();
            int statesCount = 0;
            var statesCopy = new Stack<State>(states);
            int i = 0;
            while (statesCopy.Count > 0)
            {
                State state = statesCopy.Pop();
                if (state.GetName() == objectName)
                    statesCount++;
                i++;
            }

            Console.WriteLine(" {0} objects with name {1}", statesCount, objectName);
        }

        private void PrintForeach(Stack<State> array)
        {
            foreach (State state in array)
                Console.WriteLine("object name: {0}, full info: {1}", state.ToString(), state.GetFullInfo());

            Console.Write("\n");
        }

        private void CloneStack()
        {
            /*
             * Класс Stack<T> в C# не предоставляет прямого метода Clone, 
             * как некоторые другие классы коллекций.
             * Однако мы можем вручную создать копию стека,
             * создав новый Stack<T> и скопировав элементы.
             * Один из способов добиться этого — использовать метод ToArray
             * и передать массив конструктору нового стека.
             */
            var cloneStates = new Stack<State>(states.ToArray());
            PrintForeach(cloneStates);
        }

        private void SortStack()
        {
            /*
             * Класс Stack<T> в C# не предоставляет прямого метода Sort, 
             * как некоторые другие классы коллекций.
             * Однако мы можем вручную создать копию стека,
             * создав новый Stack<T> и скопировав элементы.
             * Один из способов добиться этого — использовать метод ToArray и OrderBy
             * и передать массив конструктору нового стека.
             */
            var sortedStates = new Stack<State>(states.ToArray().OrderBy(state => state.GetName()));
            PrintForeach(sortedStates);
        }

    }
}
