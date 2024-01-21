using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab13k2
{
    public class CollectionHandlerEventArgs : System.EventArgs {
    }

    public delegate void CollectionHandler(object source, CollectionHandlerEventArgs args);//делегат

    public class MyNewCollection : MyCollection
    {
        public event CollectionHandler CollectionCountChanged;
        public event CollectionHandler CollectionReferenceChanged;

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

        //обработчик события CollectionCountChanged
        public virtual void OnCollectionCountChanged(object source, CollectionHandlerEventArgs args)
        {
            if (CollectionCountChanged != null)
                CollectionCountChanged(source, args);
        }
        //обработчик события OnCollectionReferenceChanged
        public virtual void OnCollectionReferenceChanged(object source, CollectionHandlerEventArgs args)
        {
            if (CollectionReferenceChanged != null)
                CollectionReferenceChanged(source, args);
        }

        public override bool Remove(int position)
        {
            OnCollectionCountChanged(this, new CollectionHandlerEventArgs(this.Name, "delete", list[position]));
            return base.Remove(position);
        }
        public override int Add(Person p)
        {
            OnCollectionCountChanged(this, new CollectionHandlerEventArgs(this.Name, "add", p));
            return base.Add(p);
        }
        public override Person this[int index]
        {
            get
            {
                return base[index];
            }
            set
            {
                OnCollectionReferenceChanged(this, new CollectionHandlerEventArgs(this.Name, "changed", list[index]));
                base[index] = value;
            }
        }


    }
}
