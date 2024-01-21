using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;

namespace lab13k2
{
    public class MyCollectionNode<_Ty>
    {
        MyCollectionNode<_Ty>? next_node;
        _Ty node_data;

        public MyCollectionNode(MyCollectionNode<_Ty>? next, _Ty data) {
            next_node = next;
            node_data = data;
        }

        public _Ty Data {
            get { return node_data; }
            set { node_data = value; }
        }

        public MyCollectionNode<_Ty>? Next {
            get { return next_node; }
            set { next_node = value; }
        }
    };

    public class MyCollectionQueue<_Ty> : IEnumerable<_Ty>
    {

        public class MyCollectionQueueEnumeraror<_Ty2> : IEnumerator<_Ty2>
        {
            private MyCollectionNode<_Ty2>? tail;

            public MyCollectionQueueEnumeraror(MyCollectionNode<_Ty2>? tailref) {
                tail = tailref;
            }

            public _Ty2 Current {
                get { return tail.Data; }
            }
            object IEnumerator.Current {
                get { return Current; }
            }
            public bool MoveNext()
            {
                if (tail != null)
                    tail = tail.Next;

                return tail != null;
            }
            public void Reset()
            {
                tail = null;
            }
            public void Dispose() { }

        };

        public class MyCollectionQueueEnumeraror2<_Ty2> : IEnumerator<_Ty2>
        {
            _Ty2 data;
            MyCollectionQueue<_Ty2> collectionQueue;

            public MyCollectionQueueEnumeraror2(MyCollectionQueue<_Ty2>? thisQueue) {
                collectionQueue = thisQueue.Copy2();
            }

            public _Ty2 Current
            {
                get { return data; }
            }
            object IEnumerator.Current
            {
                get { return Current; }
            }
            public bool MoveNext()
            {
                if (!collectionQueue.IsEmpty()) {
                    data = collectionQueue.Front();
                    return true;
                }
                return false;
            }
            public void Reset() { }
            public void Dispose() { }
        };

        public IEnumerator<_Ty> GetEnumerator() {
            //return new MyCollectionQueueEnumeraror<_Ty>(tail);
            return new MyCollectionQueueEnumeraror2<_Ty>(this);
        }

        IEnumerator IEnumerable.GetEnumerator() {
            return GetEnumerator();
        }

        protected int count;
        protected MyCollectionNode<_Ty>? head;
        protected MyCollectionNode<_Ty>? tail;

        public int Count {
            get { return count; }
        }

        public MyCollectionQueue() {
            head = null;
            tail = null;
            count = 0;
        }

        protected void FillByDefaults() {
            // if number of elements greater 0
            if (count > 0)
            {
                _Ty data = default(_Ty);
                MyCollectionNode<_Ty>? newNode = new MyCollectionNode<_Ty>(null, data);
                // create new empty nodes
                for (int i = 0; i < count; i++)
                {
                    if (head != null)  // if head exists element
                        head.Next = newNode; // set next ref to exists element

                    head = newNode;
                }
            }
        }

        public MyCollectionQueue(int capacity) {
            head = null;
            tail = null;
            count = capacity; // define queue size
            FillByDefaults();
        }

        public MyCollectionQueue(MyCollectionQueue<_Ty> ?queueWithInit) {
            if(queueWithInit != null) {
                MyCollectionQueue<_Ty> copy = queueWithInit.Copy2();
                while (!copy.IsEmpty()) {
                    PushBack(copy.Front());
                }
            }
        }

        public void PushBack(_Ty data) {
            MyCollectionNode<_Ty>? newNode = null;
            newNode = new MyCollectionNode<_Ty>(null, data);
            if (head != null) // if previous node exists
                head.Next = newNode; // next node for previous - this new node

            head = newNode; //set new node to head ref
            if (tail == null)
                tail = head; // queue is empty or not initialized. Set tail to head ref

            count++; // increment count elements in queue
        }

        public void PushBackMultiple(_Ty[] dataArray, int count) {
            if(count > 0) {
                for (int i = 0; i < count; i++) {
                    PushBack(dataArray[i]);
                }
            }
        }

        public MyCollectionNode<_Ty>? Find(_Ty dataForFind) {
            MyCollectionNode<_Ty>? nodeRef = tail;
            if (nodeRef != null) {
                while(nodeRef != null) {
                    if(nodeRef.GetHashCode() == dataForFind.GetHashCode()) {
                        return nodeRef; // element found
                    }
                    nodeRef = nodeRef.Next;
                }
            }
            return null; // not found
        }

        public bool Remove(MyCollectionNode<_Ty>? nodeRefForDel) {
            if (nodeRefForDel == null)
                return false;

            MyCollectionNode<_Ty>? nodeRef = tail; // tail is start
            while (nodeRef != null) { // if start node is not null
                MyCollectionNode<_Ty>? nextRef = nodeRef.Next; // save ref to next node
                if(nextRef != null) { // if ref to next node is not null
                    if (nodeRefForDel == nextRef) { // if ref to next node equals ref node for delete
                        nodeRef.Next = nodeRefForDel.Next; // set 'next' this node ref to 'next' node ref in deleting
                        return true;
                    }
                }
                nodeRef = nextRef;
            }
            return false;
        }

        public bool RemoveMultiple(MyCollectionNode<_Ty>?[] nodesRefForDel, int count) {
            bool bSuccess = true; // return is OK
            for (int i = 0; i < count; i++) // for each element
                bSuccess &= Remove(nodesRefForDel[i]); // change bSuccess to false if one of function failed

            return bSuccess; // return bSuccess
        }

        public bool IsEmpty() {
            return tail == null; // tail ref is null. queue is empty
        }

        public _Ty Front() {
            _Ty data = default(_Ty); // init new empty object instance data
            if (tail != null) { // if tail not null
                data = tail.Data; // copy data from queue node
                tail = tail.Next; // move to next ref and set tail to this ref
                count--; // element readed from queue, decrement count
            }
            return data; // return copied data
        }
     
        public MyCollectionQueue<_Ty> Copy() {  // DEPTH copy
            return new MyCollectionQueue<_Ty>(this);
        }

        public MyCollectionQueue<_Ty> Copy2() {
            MyCollectionQueue<_Ty> queueCopy = new MyCollectionQueue<_Ty>();
            queueCopy.head = head;
            queueCopy.tail = tail;
            queueCopy.count = count;
            return queueCopy;
        }

        // free memory
        public void Free() {
            head = null;
            tail = null;
        }

        public _Ty this[int index] { 
            get {
                int i = 0;
                MyCollectionNode<_Ty>? nodeRef = tail; 
                while (nodeRef != null) {
                    if(index == i)
                        return nodeRef.Data;
                    
                    i++;
                    nodeRef = nodeRef.Next;
                }
                throw new IndexOutOfRangeException($"index {index} out of bounds");
            }
            set
            {
                int i = 0;
                MyCollectionNode<_Ty>? nodeRef = tail;
                while (nodeRef != null) {
                    if (index == i)
                        nodeRef.Data = value;

                    i++;
                    nodeRef = nodeRef.Next;
                }
                throw new IndexOutOfRangeException($"index {index} out of bounds");
            }
        }


    }
}
