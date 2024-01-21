using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab12k
{
    public class Lab124
    {
        public void Start() {
            MyCollectionQueue<Organization> myQueue = new MyCollectionQueue<Organization>();
            if (myQueue.IsEmpty())
                Console.WriteLine("collection is empty\n");

            for(int i = 0; i < 10; i++) {
                Organization org = new Organization($"org_{i}", "address", "time");
                myQueue.PushBack(org);
                Console.WriteLine("Pushed {0} data to queue\n", org.ToString());
            }

            Console.WriteLine("\n--- printing by foreach ---");
            foreach (var data in myQueue)
                Console.WriteLine("Data in myQueue: {0}", data.GetFullInfo());

            Console.WriteLine("\n--- read from queue ---");
            if (!myQueue.IsEmpty()) {
                while (!myQueue.IsEmpty()) {
                    Organization dataFromQueue = myQueue.Front();
                    Console.WriteLine("Readed from queue: {0}", dataFromQueue.GetFullInfo());
                }
            }


        }
    }
}
