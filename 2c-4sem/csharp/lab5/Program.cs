using System;

namespace lab5
{
    public class Program
    {
        public const int arraySize = 10;

        static void PrintMainMenu()
        {
            Console.WriteLine("1. 1-dim array (Deleting element equals avg array elems)");
            Console.WriteLine("2. 2-dim array (Add col to end of matrix)");
            Console.WriteLine("3. ragged array (Delete all rows containing the given number K)");
            Console.WriteLine("\n0. exit");
        }

        public static int FindElemEqualsAverageElems(ref int avg, int[] array)
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

        public static bool RemoveFromArray(ref int[] array, int from_idx, int count)
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

        public static void RemoveFromArray(ref int[][] array, int from_idx, int count)
        {
            int j = 0;
            int dim1Length = array.GetLength(0);
            int[][] copy = new int[dim1Length - count][];
            for (int i = 0; i < from_idx; i++)
                copy[j++] = array[i];

            for (int i = from_idx + count; i < dim1Length; i++)
                copy[j++] = array[i];

            array = copy;
        }

        static void PrintArray(int[] array)
        {
            for (int i = 0; i < array.Length; i++)
                Console.Write(array[i] + " ");

            Console.WriteLine();
        }

        static void PrintRaggedArray(int[][] array)
        {
            for (int i = 0; i < array.GetLength(0); i++)
            {
                Console.Write("[{0}] ", i);
                PrintArray(array[i]);
            }
        }

        static void FillArray(int[] array, int min = 0, int max = 100)
        {
            try
            {
                Console.Write("Auto fill mode? y/n: ");
                if (Console.ReadLine() == "y")
                {
                    Random rn = new Random();
                    for (int i = 0; i < array.Length; i++)
                        array[i] = rn.Next(min, max);

                    Console.WriteLine("Array filled");
                    return;
                }

                for (int i = 0; i < array.Length; i++)
                {
                    Console.Write("Write {0} number: ", i);
                    array[i] = Convert.ToInt32(Console.ReadLine());
                }
            }
            catch (Exception e) {
                Console.WriteLine(e.ToString());
            }
        }

        static void PrintMatrix(int[,] matrix)
        {
            Console.WriteLine();
            int rows = matrix.GetLength(0);
            int cols = matrix.GetLength(1);
            for (int i = 0; i < rows; i++)
            {
                Console.Write("| ");
                for(int j = 0; j < cols; j++)
                {
                    Console.Write(matrix[i, j] + " ");
                }
                Console.WriteLine("|");
            }
        }

        public static void MakeIdentityMatrix(int[,] matrix)
        {
            int rows = matrix.GetLength(0);
            int cols = matrix.GetLength(1);
            for(int i = 0; i < rows; i++)
                for(int j = 0;j < cols; j++)
                    matrix[i, j] = (i == j) ? 1 : 0;
        }

        public static void ResizeMatrix(ref int[,] matrix, int newWidth = 4, int newHeight = 4)
        {
            try
            {
                int srcRows = matrix.GetLength(0);
                int srcCols = matrix.GetLength(1);
                int[,] matCopy = new int[newWidth, newHeight];
                for (int i = 0; i < matCopy.GetLength(0); i++)
                    for (int j = 0; j < matCopy.GetLength(1); j++)
                        matCopy[i, j] = (i < srcRows && j < srcCols) ? matrix[i, j] : 0;

                matrix = matCopy;
            }
            catch(Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }

        public static void RandFillRaggedArray(ref Random rn, int[][] raggedArray, int min = 0, int max = 100)
        {
            for (int i = 0; i < raggedArray.GetLength(0); i++)
                for (int j = 0; j < raggedArray[i].Length; j++)
                    raggedArray[i][j] = rn.Next(min, max);
        }

        public static int FindRowsWithK(int[][] array, int k)
        {
            for (int i = 0; i < array.GetLength(0); i++)
                for (int j = 0; j < array[i].Length; j++)
                    if (array[i][j] == k)
                        return i;

            return -1;
        }

        public static int[][] GenRaggedArray(ref Random rn)
        {
            int[][] raggedArray = new int[arraySize][];
            for (int i = 0; i < raggedArray.GetLength(0); i++)
                raggedArray[i] = new int[5 + rn.Next(5, 10)];

            return raggedArray;
        }
        
        static void Main(string[] args)
        {
            int item;
            PrintMainMenu();
            try
            {
                do
                {
                    Console.Write(">> ");
                    item = Convert.ToInt32(Console.ReadLine());
                    switch (item)
                    {
                        case 1:
                            int avg = 0, idx;
                            int[] arr1d = new int[arraySize];
                            FillArray(arr1d);
                            PrintArray(arr1d);
                            if ((idx = FindElemEqualsAverageElems(ref avg, arr1d)) == -1)
                            {
                                Console.WriteLine("Element with average value not found in array");
                                break;
                            }

                            if (!RemoveFromArray(ref arr1d, idx, 1))
                                Console.WriteLine("Failed to remove element by index ({1})", idx);

                            PrintArray(arr1d);
                            break;

                        case 2:
                            int[,] mat = new int[arraySize, arraySize];
                            MakeIdentityMatrix(mat);
                            PrintMatrix(mat);

                            ResizeMatrix(ref mat, arraySize, arraySize + 1);
                            PrintMatrix(mat);
                            break;

                        case 3:
                            int k;
                            Random rn = new Random();
                            int[][] raggedArray = GenRaggedArray(ref rn);
                            RandFillRaggedArray(ref rn, raggedArray);
                            PrintRaggedArray(raggedArray);

                            Console.Write("K elem?: ");
                            k = Convert.ToInt32(Console.ReadLine());

                            int rowidx;
                            while ((rowidx = FindRowsWithK(raggedArray, k)) != -1)
                                RemoveFromArray(ref raggedArray, rowidx, 1);

                            PrintRaggedArray(raggedArray);
                            break;


                        default:
                            Console.WriteLine("Unknown parameter!\n");
                            break;
                    }
                } while (item != 0);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
    }
}