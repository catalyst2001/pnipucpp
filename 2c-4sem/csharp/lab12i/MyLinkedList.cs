using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;

namespace lab12i
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

    public class MyLinkedList<_Ty> : IEnumerable<_Ty>
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
            MyLinkedList<_Ty2> collectionQueue;

            public MyCollectionQueueEnumeraror2(MyLinkedList<_Ty2>? thisQueue) {
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

        int count;
        MyCollectionNode<_Ty>? head;
        MyCollectionNode<_Ty>? tail;

        public int Count {
            get { return count; }
        }

        public MyLinkedList() {
            head = null;
            tail = null;
            count = 0;
        }

        public MyLinkedList(int capacity) {
            head = null;
            tail = null;
            count = capacity; // define queue size

            // if number of elements greater 0
            if(count > 0) {
                _Ty data = default(_Ty);
                MyCollectionNode<_Ty>? newNode = new MyCollectionNode<_Ty>(null, data);
                // create new empty nodes
                for (int i = 0; i < count; i++) {                         
                    if (head != null)  // if head exists element
                        head.Next = newNode; // set next ref to exists element

                    head = newNode;
                }
            }
        }

        public MyLinkedList(MyLinkedList<_Ty> ?queueWithInit) {
            if(queueWithInit != null) {
                MyLinkedList<_Ty> copy = queueWithInit.Copy2();
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
     
        public MyLinkedList<_Ty> Copy() {  // DEPTH copy
            return new MyLinkedList<_Ty>(this);
        }

        public MyLinkedList<_Ty> Copy2() {
            MyLinkedList<_Ty> queueCopy = new MyLinkedList<_Ty>();
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
    }
}
