using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab12i
{
    public class Lab123
    {
        MyHashTable<State, string> ?ht;

        public void Start()
        {
            ht = new MyHashTable<State, string>(256);
            State forDeleting = new State("For deleting", "");
            ht.Add(forDeleting, "ObjectForDeleting");

            for (int i = 0; i < 8; i++) {
                string str = $"org_{i}";
                State org = new State(str, $"State{i}");
                if(-1 == ht.Add(org, str)) {
                    Console.WriteLine("Collision!\n");
                }
            }

            ht.Print();
            if(ht.Find(forDeleting) != null) {
                Console.WriteLine("object found");
                if(ht.Remove(forDeleting)) {
                    if(ht.Find(forDeleting) == null) {
                        Console.WriteLine("object deleted");
                        ht.Print();
                    }
                }
            }

            // show collisions
            for (int i = 8; i < ht.Count; i++) {
                string str = $"next_{i}";
                State org = new State(str, $"State{i}");
                if (-1 == ht.Add(org, str)) {
                    Console.WriteLine("Collision on {0} index!\n", i);
                }
            }
        }
    }
}
