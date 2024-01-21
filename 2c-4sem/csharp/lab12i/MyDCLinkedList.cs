using System;
using System.Collections;
using System.Diagnostics;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static System.Runtime.InteropServices.JavaScript.JSType;

/* Double Connected Linked List */
namespace lab12i
{
    public class MyDCLinkedListNode<_Ty>
    {
        public MyDCLinkedListNode<_Ty>? Last;
        public MyDCLinkedListNode<_Ty>? Next;
        public _Ty Data;

        public MyDCLinkedListNode() {
            Last = null;
            Next = null;
        }

        public MyDCLinkedListNode(_Ty data) {
            Data = data;
            Last = null;
            Next = null;
        }

        public MyDCLinkedListNode(MyDCLinkedListNode<_Ty>? alast, MyDCLinkedListNode<_Ty>? anext) {
            Last = alast;
            Next = anext;
        }

        public void InsertFirst(MyDCLinkedListNode<_Ty>? node) {
            // skip addition null ptr
            if (node == null)
                return;

            MyDCLinkedListNode<_Ty>? lastNode = Last; // save last ptr in temp variable
            Last = node; // set last to new node
            node.Last = lastNode; // set last node to new node
            node.Next = this; // next node for this new node - this
        }

        public void InsertNext(MyDCLinkedListNode<_Ty>? node) {
            // skip addition null ptr
            if (node == null)
                return;

            MyDCLinkedListNode<_Ty>? nextNode = Next;
            Next = node; // set Next ptr to this node
            node.Next = nextNode; // set Next node ptr to new node
            node.Last = this; // set last node ptr to this node
        }

        /* default insert new node to next position */
        public MyDCLinkedListNode<_Ty>? NewNode() { 
            MyDCLinkedListNode<_Ty>? newNode = new MyDCLinkedListNode<_Ty>();
            InsertNext(newNode);
            return newNode;
        }

        /* insert new node to last */
        public MyDCLinkedListNode<_Ty>? NewNode(bool bInsertFirst)
        {
            MyDCLinkedListNode<_Ty>? newNode = new MyDCLinkedListNode<_Ty>();
            InsertFirst(newNode);
            return newNode;
        }

        public void Unlink()
        {
            MyDCLinkedListNode<_Ty>? last = Last;
            MyDCLinkedListNode<_Ty>? next = Next;
            if(last != null)
                last.Next = next;
            if (next != null)
                next.Last = last;
        }
    }

    public class MyDCLinkedList<_Ty> : IEnumerable<MyDCLinkedListNode<_Ty>>
    {
        public MyDCLinkedListNode<_Ty> ?Tail; // first added element
        public MyDCLinkedListNode<_Ty> ?Head; // last added element

        public IEnumerator<MyDCLinkedListNode<_Ty>> GetEnumerator() {
            MyDCLinkedListNode<_Ty>? current = Tail;
            while (current != null) {
                yield return current;
                current = current.Next;
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        public MyDCLinkedList() {
            Tail = null;
            Head = null;
        }

        public MyDCLinkedListNode<_Ty>? InsertFirst(_Ty data) {
            MyDCLinkedListNode < _Ty > ?newNode = new MyDCLinkedListNode<_Ty>(data);

            /* add node to empty linked list */
            //TODO: use IsEmpty()
            if (Tail == null) {
                Tail = newNode;
                Head = newNode;
                return newNode;
            }

            /* add next node */
            Tail.InsertNext(newNode);
            Tail = newNode;
            return newNode;
        }

        public bool IsEmpty()
        {
            //Debug.Assert((Head != null && Tail == null) || (Head == null && Tail != null), "What happened?!"); //error state
            return Head == null && Tail == null;
        }

        public MyDCLinkedListNode<_Ty>? InsertLast(_Ty data) {
            MyDCLinkedListNode<_Ty>? newNode = new MyDCLinkedListNode<_Ty>(data);

            /* add node to empty linked list */
            //TODO: use IsEmpty()
            if (Head == null) {
                Head = newNode;
                Tail = newNode;
                return newNode;
            }

            /* add next node */
            Head.InsertNext(newNode);
            Head = newNode;
            return newNode;
        }

        public MyDCLinkedListNode<_Ty>? Find(_Ty data) {
            MyDCLinkedListNode<_Ty>? node = Tail;
            while(node != null) {
                if(node.Data != null && node.Data.Equals(data)) {
                    return node;
                }
                node = node.Next;
            }
            return null;
        }

        public bool RemoveElem(_Ty data) {
            MyDCLinkedListNode<_Ty>? node = Find(data);
            if (node == null)
                return false;

            node.Unlink();
            return true;
        }
    }
}
