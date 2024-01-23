using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab13i
{
    public delegate void CollectionHandler(object source, CollectionHandlerEventArgs args);//делегат

    public class MyCollection : MyCollectionQueue<State>
    {
        List<JournalEntry> journal;

        public string Name { get; set; }

        public MyCollection(string name) : base() {
            journal = new List<JournalEntry>();
            Name = name;
        }

        public MyCollection(string name, int cap) : base(cap) {
            journal = new List<JournalEntry>();
            Name = name;
        }


        public override State this[int index]
        {
            get
            {
                return base[index];
            }
            set
            {
                CollectionReferenceChanged(this, new CollectionHandlerEventArgs(this.Name, "changed", this[index]));
                base[index] = value;
            }
        }

        public void CollectionCountChanged(object sourse, CollectionHandlerEventArgs e)
        {
            JournalEntry je = new JournalEntry(e.NameCollection, e.ChangeCollection, e.Obj.ToString());
            journal.Add(je);

        }
        public void CollectionReferenceChanged(object sourse, CollectionHandlerEventArgs e)
        {
            JournalEntry je = new JournalEntry(e.NameCollection, e.ChangeCollection, e.Obj.ToString());
            journal.Add(je);
        }

        public virtual bool Remove(int postition)
        {
            Console.WriteLine("MyCollection::Remove({0}) called", postition);
            MyCollectionNode<State>? node = GetNodeByIndex(postition);
            return base.Remove(node);
        }

        public virtual bool Add(State org)
        {
            Console.WriteLine("MyCollection::Add({0}) called", org);
            base.PushBack(org);
            return true;
        }

        public void FillCollection() {
            MyCollectionNode<State>? node = tail;
            while (node != null) {
                node.Data.SetName("State");
                node.Data.SetGovernmentForm("GovermentForm");
                node = node.Next;
            }
        }

        public void Sort() {
            MyCollectionNode<State>? node = tail;
            List<State> list = new List<State>(count);
            while (node != null) {
                list.Add(node.Data);
                node = node.Next;
            }

            list.Sort();

            node = tail;
            foreach (var item in list) {
                Debug.Assert(node != null, "node can never be null here!");
                node.Data = item;
                node = node.Next;
            }
        }

        public void Clear() {
            Free();
        }
    }
}
