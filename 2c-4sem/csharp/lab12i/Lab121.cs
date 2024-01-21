using Microsoft.VisualBasic.FileIO;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab12i
{
    public class Lab121
    {
        private static void DraftPrint(MyDCLinkedList<Republic>? linkedList)
        {
            if (linkedList == null)
            {
                Console.WriteLine("linkedList is null");
                return;
            }

            MyDCLinkedListNode<Republic>? node = linkedList.Tail;
            while (node != null)
            {
                Console.WriteLine(node.Data.GetFullInfo());
                node = node.Next;
            }
        }

        public void Start()
        {
            MyDCLinkedList<Republic> linkedList = new MyDCLinkedList<Republic>();
            for (int j = 0; j < 5; j++)
                linkedList.InsertLast(new Republic($"Republic{j}"));

            Console.WriteLine("Source elements list:");
            //DraftPrint(linkedList);
            foreach (var value in linkedList)
                Console.WriteLine("value {0}", value.Data.GetFullInfo());

            Console.Write("\n");

            Console.WriteLine("Write element index to add:");
            var elementIndex = int.Parse(Console.ReadLine());

            /* add element by index */
            int i = 0;
            foreach (MyDCLinkedListNode<Republic> linkedListItem in linkedList)
            {
                if (i == elementIndex)
                {
                    linkedList.InsertLast(linkedListItem.Data);
                    break;
                }
                i++;
            }

            Console.WriteLine("Modified elements list:");
            foreach (var value in linkedList)
                Console.WriteLine("value {0}", value.Data.GetFullInfo());
        }
    }
}
