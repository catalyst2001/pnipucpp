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
                    states.Push(new State("State Added", "123"));
                    break;
                        
                case 2:
                    states.Push(new Republic("Republic  Added"));
                    break;

                case 3:
                    states.Push(new Monarchy("Monarchy  Added"));
                    break;

                case 4:
                    states.Push(new Kingdom("Kingdom  Added"));
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
            Console.WriteLine("3. Exit");
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
                        return; //exit
                }
            }
        }
    }
}
