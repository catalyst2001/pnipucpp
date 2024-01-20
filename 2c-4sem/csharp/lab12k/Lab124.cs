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
            MyCollectionQueue<int> myQueue = new MyCollectionQueue<int>();
            if (myQueue.IsEmpty())
                Console.WriteLine("collection is empty\n");

            for(int i = 0; i < 10; i++) {
                myQueue.PushBack(i);
                Console.WriteLine("Pushed {0} data to queue\n", i);
            }

            Console.WriteLine("--- printing by foreach ---");
            foreach (var data in myQueue)
                Console.WriteLine("Data in myQueue: {0}", data);

            if (!myQueue.IsEmpty())
            {
                while (!myQueue.IsEmpty())
                {
                    int dataFromQueue = myQueue.Front();
                    Console.WriteLine("Readed from queue: {0}", dataFromQueue);
                }
            }


        }
    }
}
