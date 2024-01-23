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

        public void Start()
        {
            /********** collection 1 ***********/
            mc1 = new MyNewCollection("Collection1");
            Journal joun1 = new Journal();
            mc1.CollectionCountChanged += new CollectionHandler(joun1.CollectionCountChanged);
            mc1.CollectionReferenceChanged += new CollectionHandler(joun1.CollectionReferenceChanged);

            State stateForDel = new State("State for delete", "Republic");
            mc1.Add(new State("NewState", "Republic"));
            mc1.Add(stateForDel);

            int j = 0;
            foreach (var i in mc1)
                Console.WriteLine("[{0}]: {1}", j++, i.GetFullInfo());

            MyCollectionNode<State>? node = mc1.Find(stateForDel);
            if (mc1.Remove(node))
            {
                Console.WriteLine("element removed from collection\n");
            }

            j = 0;
            foreach (var i in mc1)
                Console.WriteLine("[{0}]: {1}", j++, i.GetFullInfo());

            mc1[0] = stateForDel;


            Console.WriteLine("-------- printing mc1 journal --------");
            joun1.PrintJournal();


            Console.WriteLine("\n\n\n\n");

            /********** collection 2 ***********/
            mc2 = new MyNewCollection("Collection2");
            Journal joun2 = new Journal();
            mc2.CollectionCountChanged += new CollectionHandler(joun2.CollectionCountChanged);
            mc2.CollectionReferenceChanged += new CollectionHandler(joun2.CollectionReferenceChanged);
            for (int i = 0; i < 16; i++)
            {
                State state = new State($"State {i + 1}", "Republic");
                mc2.Add(state);
            }

            State stateForDeletingFromMc2 = new State("State2", "For deleting");
            mc2.Add(stateForDeletingFromMc2);

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
