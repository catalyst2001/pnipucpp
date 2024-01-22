using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab13k2
{
    public class Lab
    {
        MyNewCollection mc1;
        MyNewCollection mc2;

        public void Start() {
            mc1 = new MyNewCollection("FIRST");
            mc2 = new MyNewCollection("COLLECTION2");
            Journal joun1 = new Journal();
            mc1.CollectionCountChanged += new CollectionHandler(joun1.CollectionCountChanged);
            mc1.CollectionReferenceChanged += new CollectionHandler(joun1.CollectionReferenceChanged);

            Organization orgForDel = new Organization("Org for delete", "address", "10:00-20:00");
            mc1.Add(new Organization("Added org", "address", "time"));
            mc1.Add(orgForDel);

            int j = 0;
            foreach (var i in mc1)
                Console.WriteLine("[{0}]: {1}", j++, i.GetFullInfo());

            MyCollectionNode<Organization> ?node = mc1.Find(orgForDel);
            if(mc1.Remove(node)) {
                Console.WriteLine("element removed from collection\n");
            }

            j = 0;
            foreach (var i in mc1)
                Console.WriteLine("[{0}]: {1}", j++, i.GetFullInfo());

            mc1[0] = orgForDel;
        }
    }
}
