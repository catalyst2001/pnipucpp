using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;

namespace lab13i
{
    public class MyLinkedList<Ty> : IEnumerable<Ty>
    {
        public class MyLinkedListEnumeraror<TX> : IEnumerator<TX>
        {
            TX data;
            MyLinkedList<TX> linkedList;

            public MyLinkedListEnumeraror(MyLinkedList<TX>? thisLinkedList)
            {
                linkedList = thisLinkedList.ShallowCopy();
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
                if (!linkedList.IsEmpty())
                {
                    data = linkedList.Front();
                    return true;
                }
                return false;
            }

            public void Reset() { }

            public void Dispose() { }
        };

        public IEnumerator<Ty> GetEnumerator()
        {
            return new MyLinkedListEnumeraror<Ty>(this);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        protected int count;
        protected MyLinkedListNode<Ty>? head;
        protected MyLinkedListNode<Ty>? tail;

        public MyLinkedListNode<Ty>? Head
        {
            get
            {
                return head;
            }
            set
            {
                head = value;
            }
        }

        public MyLinkedListNode<Ty>? Tail
        {
            get
            {
                return tail;
            }
            set
            {
                tail = value;
            }
        }

        public int Count
        {
            get { return count; }
        }

        public MyLinkedList()
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
                MyLinkedListNode<Ty>? newNode = new MyLinkedListNode<Ty>(null, data);
                // create new empty nodes
                for (int i = 0; i < count; i++)
                {
                    if (head != null)  // if head exists element
                        head.Next = newNode; // set next ref to exists element

                    head = newNode;
                }
            }
        }

        public MyLinkedList(int capacity)
        {
            head = null;
            tail = null;
            count = capacity;
            FillByDefaults();
        }

        public MyLinkedList(MyLinkedList<Ty>? linkedListWithInit)
        {
            if (linkedListWithInit != null)
            {
                MyLinkedList<Ty> copy = linkedListWithInit.ShallowCopy();
                while (!copy.IsEmpty())
                {
                    PushBack(copy.Front());
                }
            }
        }

        public void PushBack(Ty data)
        {
            MyLinkedListNode<Ty>? newNode = null;
            newNode = new MyLinkedListNode<Ty>(null, data);
            if (head != null)
                head.Next = newNode;

            head = newNode;
            if (tail == null)
                tail = head;

            count++;
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

        public MyLinkedListNode<Ty>? Find(Ty dataForFind)
        {
            MyLinkedListNode<Ty>? nodeRef = tail;
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

        public bool Remove(MyLinkedListNode<Ty>? nodeRefForDel)
        {
            if (nodeRefForDel == null)
                return false;

            MyLinkedListNode<Ty>? nodeRef = tail; // tail is start
            while (nodeRef != null)
            { // if start node is not null
                MyLinkedListNode<Ty>? nextRef = nodeRef.Next; // save ref to next node
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

        public bool RemoveMultiple(MyLinkedListNode<Ty>?[] nodesRefForDel, int count)
        {
            bool bSuccess = true; // return is OK
            for (int i = 0; i < count; i++) // for each element
                bSuccess &= Remove(nodesRefForDel[i]); // change bSuccess to false if one of function failed

            return bSuccess; // return bSuccess
        }

        public bool IsEmpty()
        {
            return tail == null;
        }

        public Ty Front()
        {
            Ty data = default(Ty); // init new empty object instance data
            if (tail != null)
            { // if tail not null
                data = tail.Data;
                tail = tail.Next;
                count--;
            }
            return data; // return copied data
        }

        public MyLinkedList<Ty> Copy()
        {  // DEPTH copy
            return new MyLinkedList<Ty>(this);
        }

        public MyLinkedList<Ty> ShallowCopy()
        {
            MyLinkedList<Ty> linkedListCopy = new MyLinkedList<Ty>();
            linkedListCopy.head = head;
            linkedListCopy.tail = tail;
            linkedListCopy.count = count;
            return linkedListCopy;
        }

        // free memory
        public void Free()
        {
            head = null;
            tail = null;
        }

        public MyLinkedListNode<Ty>? GetNodeByIndex(int index)
        {
            int i = 0;
            MyLinkedListNode<Ty>? nodeRef = tail;
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
                MyLinkedListNode<Ty>? nodeRef = GetNodeByIndex(index);
                if (nodeRef == null)
                    throw new IndexOutOfRangeException($"index {index} out of bounds");

                return nodeRef.Data;
            }
            set
            {
                MyLinkedListNode<Ty>? nodeRef = GetNodeByIndex(index);
                if (nodeRef == null)
                    throw new IndexOutOfRangeException($"index {index} out of bounds");

                nodeRef.Data = value;
            }
        }


    }
}
