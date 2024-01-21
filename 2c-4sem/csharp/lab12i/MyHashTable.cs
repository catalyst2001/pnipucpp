using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab12i
{
    public class MyHashTable<_TyKey, _TyVal>
    {
        int hashTableSize;
        _TyVal[] tbl;

        public int Count
        {
            get
            {
                return tbl.Length;
            }
        }

        static private int MyHashFunc(_TyKey data) {
            return data.GetHashCode();
        }

        public MyHashTable(int size) {
            hashTableSize = size;
            if (hashTableSize == 0)
                hashTableSize++;

            tbl = new _TyVal[hashTableSize];
        }

        private int GetIndexByKey(_TyKey key) {
            return Math.Abs(MyHashFunc(key)) % tbl.Length;
        }

        public int Add(_TyKey key, _TyVal data) {
            int idx = GetIndexByKey(key);
            if (tbl[idx] == null) {
                tbl[idx] = data;
                return idx;
            }
            return -1;
        }

        public void Print() {
            for (int i = 0; i < hashTableSize; i++) {
                if (tbl[i] != null) {
                    Console.WriteLine("[{0}] Obj: {1}", i, tbl[i]);
                }
            }
        }

        public bool Exists(_TyKey key) { return tbl[GetIndexByKey(key)] != null; }
        public _TyVal Find(_TyKey key) { return tbl[GetIndexByKey(key)]; }

        public bool Remove(_TyKey key) {
            int idx = GetIndexByKey(key);
            if(tbl[idx] != null) {
                tbl[idx] = default(_TyVal);
                return true;
            }
            return false;
        }
    }
}
