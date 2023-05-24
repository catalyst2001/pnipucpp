using lab2;
using System;

namespace Lab2 // Note: actual namespace depends on the project name.
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Task1.SumOdd();
            Task2.MultipleK(Convert.ToInt32(Console.ReadLine()));
            Task3.SumNumbers(6);
        }
    }
}