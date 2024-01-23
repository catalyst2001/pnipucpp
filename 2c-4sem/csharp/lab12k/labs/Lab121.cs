using Microsoft.VisualBasic.FileIO;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab12k.labs
{
    public class Lab121
    {
        private static void DraftPrint(MyDCLinkedList<Library>? linkedList)
        {
            if (linkedList == null)
            {
                Console.WriteLine("linkedList is null");
                return;
            }

            MyDCLinkedListNode<Library>? node = linkedList.Tail;
            while (node != null)
            {
                Console.WriteLine(node.Data.GetFullInfo());
                node = node.Next;
            }
        }

        public void Start()
        {
            MyDCLinkedList<Library> linkedList = new MyDCLinkedList<Library>();
            for (int i = 0; i < 5; i++)
                linkedList.InsertLast(new Library($"library_{i}", $"street {i + 25}", "10:00-20:00", i + 100));

            /* object for deleting from LL */
            Library libForDeleteFromLL = new Library("Unique library", "Chistyakova 15b", "8:00-20:00", 1000);
            linkedList.InsertLast(libForDeleteFromLL);

            Console.WriteLine("Source elements list:");
            //DraftPrint(linkedList);
            foreach (var value in linkedList)
                Console.WriteLine("value {0}", value.Data.GetFullInfo());

            Console.Write("\n");

            /* remove element by information */
            Console.WriteLine("/* remove element by information */");
            if (!linkedList.RemoveElem(libForDeleteFromLL))
                Console.WriteLine("failed to delete object from linked list");

            Console.WriteLine("Modified elements list:");
            foreach (var value in linkedList)
                Console.WriteLine("value {0}", value.Data.GetFullInfo());
        }
    }
}
