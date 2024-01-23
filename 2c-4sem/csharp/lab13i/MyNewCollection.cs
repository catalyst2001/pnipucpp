using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab13i
{
    public class MyNewCollection : MyCollection
    {
        public MyNewCollection(string name) : base(name) {
        }

        public MyNewCollection(string name, int cap) : base(name, cap) {
        }

        public event CollectionHandler CollectionCountChanged;
        public event CollectionHandler CollectionReferenceChanged;

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
            OnCollectionCountChanged(this, new CollectionHandlerEventArgs(this.Name, "delete", this[position]));
            return base.Remove(position);
        }

        public override bool Add(State p)
        {
            OnCollectionCountChanged(this, new CollectionHandlerEventArgs(this.Name, "add", p));
            return base.Add(p);
        }

        public override State this[int index]
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
    }
}
