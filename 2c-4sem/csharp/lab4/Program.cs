using Microsoft.VisualBasic.FileIO;
using System;
using System.Diagnostics.CodeAnalysis;
using System.Reflection.Metadata;
using System.Reflection.Metadata.Ecma335;
using System.Runtime.CompilerServices;
using System.Windows.Markup;

namespace lab4
{
    internal class Program
    {
        static bool RemoveFromArray(ref int[] array, int from_idx, int count)
        {
            if ((from_idx + count) >= array.Length)
                return false;

            int j = 0;
            int[] copy = new int[array.Length - count];
            for (int i = 0; i < from_idx; i++)
                copy[j++] = array[i];

            for (int i = from_idx + count; i < array.Length; i++)
                copy[j++] = array[i];

            array = copy;
            return true;
        }

        static bool InsertElemFrom(ref int[] array, int idx, int elem)
        {
            if (idx >= array.Length)
                return false;

            int i, j = 0;
            int[] copy = new int[array.Length + 1];
            for (i = 0; i < idx; i++)
                copy[j++] = array[i];         

            for (i = idx; i < array.Length; i++)
                copy[i+1] = array[i];

            copy[idx] = elem;
            array = copy;
            return true;
        }

        static void PrintArray(int[] array)
        {
            for (int i = 0; i < array.Length; i++)
                Console.Write(array[i] + " ");

            Console.WriteLine();
        }

        static void SwapOddEven(int[] array)
        {
            for (int i = 0; i < array.Length - 1; i += 2)
            {
                int temp = array[i];
                array[i] = array[i + 1];
                array[i + 1] = temp;
            }
        }

        static int FindElemEqualsAverageElems(ref int avg, int[] array)
        {
            int i, j;
            int idx = -1;
            for (i = 0; i < array.Length; i++)
            {
                avg = 0;
                for (j = 0; j < i; j++)
                    avg += array[j];

                for (j = i + 1; j < array.Length; j++)
                    avg += array[j];

                avg /= (array.Length - 1);
                if (array[i] == avg)
                {
                    idx = i;
                    break;
                }
            }
            return idx;
        }

        static void SimpleSwapSort(int[] array)
        {
            int cmpscount = 0;
            for (int i = 0; i < array.Length - 1; i++)
            {
                for (int j = i + 1; j < array.Length; j++)
                {
                    if (array[i] > array[j])
                    {
                        int temp = array[i];
                        array[i] = array[j];
                        array[j] = temp;                   
                        continue;
                    }
                    cmpscount++;
                }
            }
            Console.WriteLine("SimpleSwapSort: number of comparisons {0}", cmpscount);
        }

        public static void PrintMenu()
        {
            Console.WriteLine("1. Remove from N K elems");
            Console.WriteLine("2. Add elem from N index");
            Console.WriteLine("3. Swap odd/even indices");
            Console.WriteLine("4. Find element equal to avg value among all elements");
            Console.WriteLine("5. Refill array");
            Console.WriteLine("\n0. Exit");
        }

        static void Main(string[] args)
        {
            int from_idx, count;
            Console.Write("N: ");
            int n = Math.Abs(Convert.ToInt32(Console.ReadLine()));
            if(n == 0)
                return;

            Random rn = new Random();         
            int[] array = new int[n];
            for (int i = 0; i < n; i++)
                array[i] = rn.Next(0, 100);

            Console.WriteLine("\n--- unsorted array ---");
            PrintArray(array);

            Console.WriteLine("\n--- sorted array ---");
            SimpleSwapSort(array);
            PrintArray(array);

            /* menu */
            PrintMenu();
            bool bcontinue = true;
            do
            {
                Console.Write(">> ");
                int item = Convert.ToInt32(Console.ReadLine());
                switch (item)
                {
                    case 0:
                        bcontinue = false;
                        continue;

                    case 1:
                        Console.Write("From index: ");
                        from_idx = Convert.ToInt32(Console.ReadLine());
                        Console.Write("Count: ");
                        count = Convert.ToInt32(Console.ReadLine());

                        PrintArray(array);
                        if (!RemoveFromArray(ref array, from_idx, count))
                            Console.WriteLine("Index or Index+count out of bounds");

                        PrintArray(array);
                        break;

                    case 2:
                        Console.Write("add from index: ");
                        from_idx = Convert.ToInt32(Console.ReadLine());
                        Console.Write("Value: ");
                        int val = Convert.ToInt32(Console.ReadLine());

                        PrintArray(array);
                        if (!InsertElemFrom(ref array, from_idx, val))
                            Console.WriteLine("Index out of bounds");

                        PrintArray(array);
                        break;

                    case 3:
                        PrintArray(array);
                        SwapOddEven(array);
                        PrintArray(array);
                        break;


                    case 4:
                        int attempt = 1;
                        int avg = 0;
                        int idx = FindElemEqualsAverageElems(ref avg, array);
                        while (idx == -1)
                        {
                            Console.WriteLine("({0}) Element equals avg all elements not found. Refilling array...", attempt);

                            /* refill array */
                            for (int i = 0; i < array.Length; i++)
                                array[i] = rn.Next(0, 100);

                            SimpleSwapSort(array);
                            idx = FindElemEqualsAverageElems(ref avg, array);
                            PrintArray(array);
                            attempt++;
                        }
                        Console.WriteLine("Element equals avg({0}) all elements (index: {1} | value: {2})", avg, idx, array[idx]);
                        break;

                    case 5:
                        for (int i = 0; i < array.Length; i++)
                            array[i] = rn.Next(0, 100);

                        SimpleSwapSort(array);
                        break;

                    default:
                        PrintMenu();
                        break;
                }
            } while (bcontinue);
        }
    }
}