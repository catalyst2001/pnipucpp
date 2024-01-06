using System.Collections;
using System.Collections.Generic;
using System.Net.Http.Headers;
using System.Runtime.ExceptionServices;
using static lab11k.ConInFormat;

namespace lab11k
{
    internal class Program
    {
        static void Main(string[] args)
        {
            //new CollectionsLab1().Start(); //Stack<T> 1
            //new CollectionsLab2().Start(); //List<T> 2
            new TestCollections(5).FindTests(); // 3
        }
    }
}