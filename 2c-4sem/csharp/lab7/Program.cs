namespace lab7
{
    public class Program
    {
        static void Main(string[] args)
        {
            /* Triangle  */
            Console.WriteLine("--- triangle class instance ---");
            Triangle tri1 = new Triangle();
            Console.WriteLine("a={0} b={1} c={2}\n", tri1.a, tri1.b, tri1.c);


            Console.WriteLine("--- operators overloading ---");
            Triangle tri2 = new Triangle();
            tri2++;
            Console.WriteLine("operator++() a={0} b={1} c={2}\n", tri2.a, tri2.b, tri2.c);

            tri2--;
            Console.WriteLine("operator--() a={0} b={1} c={2}\n", tri2.a, tri2.b, tri2.c);

            tri2--;
            Console.WriteLine("operator explicit double(): {0}\n", (double)tri2);

            Triangle tri3 = new Triangle();
            tri3.a = 100000.0;
            tri3.b = -10000.0;
            tri3.c = 1.0;

            Console.WriteLine("operator implicit bool(): {0}\n", tri3 ? "Valid triangle" : "Invalid triangle");

            Console.WriteLine("operator explicit double(): {0}\n", (double)tri2);


            Console.WriteLine("bool operator<=(): S1({0:F10}) <= S2({1:F10}) == {2}\n",
                tri1.GetSquare(), tri2.GetSquare(), tri1 <= tri2);

            Console.WriteLine("bool operator>=(): S1({0:F10}) >= S2({1:F10}) == {2}\n",
                tri1.GetSquare(), tri2.GetSquare(), tri1 >= tri2);

            /* MyCollection */
            Console.WriteLine("\n--- MyCollection empty constructor ---");
            MyCollection collection2 = new MyCollection();
            Console.WriteLine("Length: {0}", collection2.Length);

            Console.WriteLine("\n--- MyCollection random fill ---");
            MyCollection collection1 = new MyCollection(10, 0.0, 50.0);
            for (int i = 0; i < collection1.Length; i++)
                Console.WriteLine("a={0} b={1} c={2}", collection1[i].a, collection1[i].b, collection1[i].c);

            Console.WriteLine("\n--- Program class function ---");

            int minSquareIdx = FindTriangleWithMinimumSquare(collection1);
            Console.WriteLine("Element with min square: {0}", minSquareIdx);
            for (int i = 0; i < collection1.Length; i++)
                Console.WriteLine("{0}[{1}] Square={2}", (i == minSquareIdx) ? "->" : "  ", i, collection1[i].GetSquare());

            Console.WriteLine("\n--- MyCollection init from array ---");
            Console.Write("Write number of elements: ");
            int n = Convert.ToInt32(Console.ReadLine());
            Triangle[] triArray = new Triangle[n];
            for (int i = 0; i < n; i++)
            {
                Console.Write("Write abc: ");
                triArray[i].a = Convert.ToDouble(Console.ReadLine());
                triArray[i].b = Convert.ToDouble(Console.ReadLine());
                triArray[i].c = Convert.ToDouble(Console.ReadLine());
            }

            MyCollection collection3 = new MyCollection(triArray);
            Console.WriteLine("Array length: {0}   MyCollection length: {1}", triArray.Length, collection3.Length);        
        }

        public static int FindTriangleWithMinimumSquare(MyCollection collection)
        {
            int idx = 0;
            double minSquare = double.MaxValue;
            for (int i = 0; i < collection.Length; i++)
            {
                if (minSquare > collection[i].GetSquare())
                {
                    minSquare = collection[i].GetSquare();
                    idx = i;
                }
            }
            return idx;
        }
    }
}