using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab13k2
{
    public delegate void CollectionHandler(object source, CollectionHandlerEventArgs args);//делегат

    public class MyCollection : MyCollectionQueue<Organization>
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


        public override Organization this[int index]
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
            return true;
        }

        public virtual bool Add(Organization org)
        {
            return true;
        }

        public void FillCollection() {
            MyCollectionNode<Organization>? node = tail;
            while (node != null) {
                node.Data.AutoFill();
                node = node.Next;
            }
        }

        public void Sort() {
            MyCollectionNode<Organization>? node = tail;
            List<Organization> list = new List<Organization>(count);
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
