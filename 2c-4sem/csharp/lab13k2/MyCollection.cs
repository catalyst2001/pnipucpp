using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab13k2
{
    public class CollectionHandlerEventArgs : System.EventArgs
    {
        public string collectionName { get; set; }
        public string eventName { get; set; }
        public object obj { get; set; }

        public CollectionHandlerEventArgs(string collectionName, string eventName, object obj)
        {
            this.collectionName = collectionName;
            this.eventName = eventName;
            this.obj = obj;
        }
    }

    public delegate void CollectionHandler(object source, CollectionHandlerEventArgs args);//делегат

    public class MyCollection : MyCollectionQueue<Organization>
    {
        public string Name { get; set; }

        public MyCollection() : base() {
        }

        public MyCollection(int cap) : base(cap) {
        }


        public override Organization this[int index]
        {
            get
            {
                return base[index];
            }
            set
            {
                OnCollectionReferenceChanged(this, new CollectionHandlerEventArgs(this.Name, "changed", this[index]));
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

        public virtual bool Add(MyCollectionQueue<Organization> org)
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
