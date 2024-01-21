using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab12k
{
    public class Lab123
    {
        MyHashTable<Organization, string> ?ht;

        public void Start()
        {
            ht = new MyHashTable<Organization, string>(256);
            Organization forDeleting = new Organization("For deleting", "", "10:00-20:00");
            ht.Add(forDeleting, "ObjectForDeleting");

            for (int i = 0; i < 8; i++) {
                string str = $"org_{i}";
                Organization org = new Organization(str, $"address {i}", "10:00-20:00");
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
                Organization org = new Organization(str, $"address {i}", "12:00-22:00");
                if (-1 == ht.Add(org, str)) {
                    Console.WriteLine("Collision on {0} index!\n", i);
                }
            }
        }
    }
}
