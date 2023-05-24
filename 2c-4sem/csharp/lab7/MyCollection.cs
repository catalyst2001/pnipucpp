using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab7
{
    public class MyCollection
    {
        Triangle[] array;

        public MyCollection() { array = new Triangle[0]; }
        public MyCollection(Triangle[] arr) {
            array = arr;
        }

        public MyCollection(int numItems, double min = 0.0, double max = 100.0)
        {
            Random rnd = new Random();
            array = new Triangle[numItems];
            for (int i = 0; i < array.Length; i++)
            {
                Triangle newtri;
                do
                {
                    newtri = new Triangle();
                    newtri.a = rnd.NextDouble() * 10.0;
                    newtri.b = rnd.NextDouble() * 10.0;
                    newtri.c = rnd.NextDouble() * 10.0;
                } while (!newtri.IsValid());
                array[i] = newtri;
            }
        }

        public int Length => array.Length;

        public Triangle this[int i] {
            get {
                if (i >= array.Length)
                    throw new ArgumentException();

                return array[i];
            }
        
            set
            {
                if (i >= array.Length)
                    throw new ArgumentException();

                array[i] = value;
            }
        }
    }
}
