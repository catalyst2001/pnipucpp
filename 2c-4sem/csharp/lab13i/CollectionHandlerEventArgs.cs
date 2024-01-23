using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab13i
{
    public class CollectionHandlerEventArgs : System.EventArgs
    {
        public string NameCollection { get; set; }
        public string ChangeCollection { get; set; }
        public object Obj { get; set; }

        public CollectionHandlerEventArgs(string collectionName, string eventName, object obj)
        {
            this.NameCollection = collectionName;
            this.ChangeCollection = eventName;
            this.Obj = obj;
        }
    }
}
