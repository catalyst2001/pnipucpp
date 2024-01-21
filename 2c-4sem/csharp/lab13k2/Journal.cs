using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab13k2
{
    public class Journal
    {
        public void CollectionCountChanged(object sender, CollectionHandlerEventArgs e)
        {
            Console.WriteLine($"Collection '{e.NameCollection}' count changed.");
        }

        public void CollectionReferenceChanged(object sender, CollectionHandlerEventArgs e)
        {
            Console.WriteLine($"Reference changed in collection '{e.NameCollection}'.");
        }
    }
}
