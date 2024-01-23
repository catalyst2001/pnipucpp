using System.Collections;
using System.Collections.Generic;
using System.Net.Http.Headers;
using System.Runtime.ExceptionServices;
using lab12k.labs;

namespace lab12k
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("\n\n*********************** 1 ***********************");
            new Lab121().Start();

            Console.WriteLine("\n\n*********************** 2 ***********************");
            new Lab122().Start();

            Console.WriteLine("\n\n*********************** 3 ***********************");
            new Lab123().Start();

            Console.WriteLine("\n\n*********************** 4 ***********************");
            new Lab124().Start();
        }
    }
}