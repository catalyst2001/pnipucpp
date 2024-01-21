using lab10;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab13k2
{
    public class MyCollection : MyCollectionQueue<Organization>
    {
        public MyCollection() : base() {
        }

        public MyCollection(int cap) : base(cap) {
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
