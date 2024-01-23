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

            Console.WriteLine("-------- printing mc1 journal --------");
            joun1.PrintJournal();


            Console.WriteLine("\n\n\n\n");

            /********** collection 2 ***********/
            mc2 = new MyNewCollection("COLLECTION2");
            Journal joun2 = new Journal();
            mc2.CollectionCountChanged += new CollectionHandler(joun2.CollectionCountChanged);
            mc2.CollectionReferenceChanged += new CollectionHandler(joun2.CollectionReferenceChanged);
            for (int i = 0; i < 16; i++) {
                Organization org = new Organization($"Organization {i + 1}", "Address", "Closed");
                mc2.Add(org);
            }

            Organization orgForDeletingFromMc2 = new Organization("Organization For deleting", "For deleting", "For deleting");
            mc2.Add(orgForDeletingFromMc2);

            Console.WriteLine("-------- printing mc2 --------");
            foreach (var i in mc2)
                Console.WriteLine(i.GetFullInfo());

            if (mc2.Remove(mc2.Count - 1))
                Console.WriteLine("OK  Deleted end element");

            Console.WriteLine("-------- printing mc2 with deleted end element --------");
            foreach (var i in mc2)
                Console.WriteLine(i.GetFullInfo());

            Console.WriteLine("-------- printing mc2 journal --------");
            joun2.PrintJournal();
        }
    }
}
