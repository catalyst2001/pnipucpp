using System.Collections;
using System.Collections.Generic;
using System.Net.Http.Headers;
using System.Runtime.ExceptionServices;

namespace lab11i
{
    internal class Program
    {
        static void Main(string[] args)
        {
            //new Work1().main();
            //new Work2().main();
            new TestCollections(5).FindTests();
        }
    }
}