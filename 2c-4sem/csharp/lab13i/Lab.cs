using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab13i
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

            State stateForDel = new State("State for delete", "Republic");
            mc1.Add(new State("NewState", "Republic"));
            mc1.Add(stateForDel);

            int j = 0;
            foreach (var i in mc1)
                Console.WriteLine("[{0}]: {1}", j++, i.GetFullInfo());

            MyCollectionNode<State> ?node = mc1.Find(stateForDel);
            if(mc1.Remove(node)) {
                Console.WriteLine("element removed from collection\n");
            }

            j = 0;
            foreach (var i in mc1)
                Console.WriteLine("[{0}]: {1}", j++, i.GetFullInfo());

            mc1[0] = stateForDel;
        }
    }
}
