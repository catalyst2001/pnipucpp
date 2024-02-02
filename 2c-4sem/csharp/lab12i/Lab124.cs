using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab12i
{
    public class Lab124
    {
        public void Start() {
            MyLinkedList<State> myQueue = new MyLinkedList<State>();
            if (myQueue.IsEmpty())
                Console.WriteLine("collection is empty\n");

            for(int i = 0; i < 10; i++) {
                State org = new State($"State{i}", "govermentForm");
                myQueue.PushBack(org);
                Console.WriteLine("Pushed {0} data to queue\n", org.ToString());
            }

            Console.WriteLine("\n--- printing by foreach ---");
            foreach (var data in myQueue)
                Console.WriteLine("Data in myQueue: {0}", data.GetFullInfo());

            Console.WriteLine("\n--- read from queue ---");
            if (!myQueue.IsEmpty()) {
                while (!myQueue.IsEmpty()) {
                    State dataFromQueue = myQueue.Front();
                    Console.WriteLine("Readed from queue: {0}", dataFromQueue.GetFullInfo());
                }
            }
        }
    }
}
