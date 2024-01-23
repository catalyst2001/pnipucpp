using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;

namespace lab13i
{
    public class MyCollectionQueue<Ty> : IEnumerable<Ty>
    {
        public class MyCollectionQueueEnumeraror<TX> : IEnumerator<TX>
        {
            TX data;
            MyCollectionQueue<TX> collectionQueue;

            public MyCollectionQueueEnumeraror(MyCollectionQueue<TX>? thisQueue)
            {
                collectionQueue = thisQueue.Copy2();
            }

            public TX Current
            {
                get { return data; }
            }

            object IEnumerator.Current
            {
                get { return Current; }
            }

            public bool MoveNext()
            {
                if (!collectionQueue.IsEmpty())
                {
                    data = collectionQueue.Front();
                    return true;
                }
                return false;
            }

            public void Reset() { }

            public void Dispose() { }
        };

        public IEnumerator<Ty> GetEnumerator()
        {
            return new MyCollectionQueueEnumeraror<Ty>(this);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        protected int count;
        protected MyCollectionNode<Ty>? head;
        protected MyCollectionNode<Ty>? tail;

        public int Count
        {
            get { return count; }
        }

        public MyCollectionQueue()
        {
            head = null;
            tail = null;
            count = 0;
        }

        protected void FillByDefaults()
        {
            // if number of elements greater 0
            if (count > 0)
            {
                Ty data = default(Ty);
                MyCollectionNode<Ty>? newNode = new MyCollectionNode<Ty>(null, data);
                // create new empty nodes
                for (int i = 0; i < count; i++)
                {
                    if (head != null)  // if head exists element
                        head.Next = newNode; // set next ref to exists element

                    head = newNode;
                }
            }
        }

        public MyCollectionQueue(int capacity)
        {
            head = null;
            tail = null;
            count = capacity; // define queue size
            FillByDefaults();
        }

        public MyCollectionQueue(MyCollectionQueue<Ty>? queueWithInit)
        {
            if (queueWithInit != null)
            {
                MyCollectionQueue<Ty> copy = queueWithInit.Copy2();
                while (!copy.IsEmpty())
                {
                    PushBack(copy.Front());
                }
            }
        }

        public void PushBack(Ty data)
        {
            MyCollectionNode<Ty>? newNode = null;
            newNode = new MyCollectionNode<Ty>(null, data);
            if (head != null) // if previous node exists
                head.Next = newNode; // next node for previous - this new node

            head = newNode; //set new node to head ref
            if (tail == null)
                tail = head; // queue is empty or not initialized. Set tail to head ref

            count++; // increment count elements in queue
        }

        public void PushBackMultiple(Ty[] dataArray, int count)
        {
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    PushBack(dataArray[i]);
                }
            }
        }

        public MyCollectionNode<Ty>? Find(Ty dataForFind)
        {
            MyCollectionNode<Ty>? nodeRef = tail;
            if (nodeRef != null)
            {
                while (nodeRef != null)
                {
                    if (nodeRef.GetHashCode() == dataForFind.GetHashCode())
                    {
                        return nodeRef; // element found
                    }
                    nodeRef = nodeRef.Next;
                }
            }
            return null; // not found
        }

        public bool Remove(MyCollectionNode<Ty>? nodeRefForDel)
        {
            if (nodeRefForDel == null)
                return false;

            MyCollectionNode<Ty>? nodeRef = tail; // tail is start
            while (nodeRef != null)
            { // if start node is not null
                MyCollectionNode<Ty>? nextRef = nodeRef.Next; // save ref to next node
                if (nextRef != null)
                { // if ref to next node is not null
                    if (nodeRefForDel == nextRef)
                    { // if ref to next node equals ref node for delete
                        nodeRef.Next = nodeRefForDel.Next; // set 'next' this node ref to 'next' node ref in deleting
                        return true;
                    }
                }
                nodeRef = nextRef;
            }
            return false;
        }

        public bool RemoveMultiple(MyCollectionNode<Ty>?[] nodesRefForDel, int count)
        {
            bool bSuccess = true; // return is OK
            for (int i = 0; i < count; i++) // for each element
                bSuccess &= Remove(nodesRefForDel[i]); // change bSuccess to false if one of function failed

            return bSuccess; // return bSuccess
        }

        public bool IsEmpty()
        {
            return tail == null; // tail ref is null. queue is empty
        }

        public Ty Front()
        {
            Ty data = default(Ty); // init new empty object instance data
            if (tail != null)
            { // if tail not null
                data = tail.Data; // copy data from queue node
                tail = tail.Next; // move to next ref and set tail to this ref
                count--; // element readed from queue, decrement count
            }
            return data; // return copied data
        }

        public MyCollectionQueue<Ty> Copy()
        {  // DEPTH copy
            return new MyCollectionQueue<Ty>(this);
        }

        public MyCollectionQueue<Ty> Copy2()
        {
            MyCollectionQueue<Ty> queueCopy = new MyCollectionQueue<Ty>();
            queueCopy.head = head;
            queueCopy.tail = tail;
            queueCopy.count = count;
            return queueCopy;
        }

        // free memory
        public void Free()
        {
            head = null;
            tail = null;
        }

        public MyCollectionNode<Ty>? GetNodeByIndex(int index)
        {
            int i = 0;
            MyCollectionNode<Ty>? nodeRef = tail;
            while (nodeRef != null)
            {
                if (index == i)
                    return nodeRef;

                i++;
                nodeRef = nodeRef.Next;
            }
            return null;
        }

        public virtual Ty this[int index]
        {
            get
            {
                MyCollectionNode<Ty>? nodeRef = GetNodeByIndex(index);
                if (nodeRef == null)
                    throw new IndexOutOfRangeException($"index {index} out of bounds");

                return nodeRef.Data;
            }
            set
            {
                MyCollectionNode<Ty>? nodeRef = GetNodeByIndex(index);
                if (nodeRef == null)
                    throw new IndexOutOfRangeException($"index {index} out of bounds");

                nodeRef.Data = value;
            }
        }


    }
}
