using System;
using System.Runtime.InteropServices;
using System.Xml.Schema;

namespace Lab1n // Note: actual namespace depends on the project name.
{
    public static class Lab1
    {
        public static void Task1()
        {
            int n, m, nsrc, msrc;
            double x;

            Console.Write("n: ");
            nsrc = Convert.ToInt32(Console.ReadLine());
            Console.Write("m: ");
            msrc = Convert.ToInt32(Console.ReadLine());

            Console.Write("x: ");
            x = Convert.ToDouble(Console.ReadLine());

            /* exp 1 */
            n = nsrc;
            m = msrc;
            Console.WriteLine("n={0} m={1}  n++*m={2}", nsrc, msrc, n++ * m);

            /* exp 2 */
            n = nsrc;
            m = msrc;
            Console.WriteLine("n={0} m={1}  n++ < m={2}", nsrc, msrc, n++ < m);

            /* exp 3 */
            n = nsrc;
            m = msrc;
            Console.WriteLine("n={0} m={1}  --m > n={2}", nsrc, msrc, --m > n);

            /* exp 4 */
            Console.WriteLine("x={0} cbrt(x-x^2 + x^5)={1}", x, Math.Cbrt(x - Math.Pow(x, 2.0) + Math.Pow(x, 5.0)));
            Console.WriteLine("\n\n\n");
        }

        public static bool point_in_circle(float cx, float cy, float r, float px, float py)
        {
            return Math.Sqrt(Math.Pow(px - cx, 2.0) + Math.Pow(py - cy, 2.0)) <= r;
        }

        public static void Task2()
        {
            float x, y;
            Console.Write("x: ");
            x = (float)Convert.ToDouble(Console.ReadLine());
            Console.Write("y: ");
            y = (float)Convert.ToDouble(Console.ReadLine());
            Console.WriteLine("x: {0} y: {1}  {2}", x, y, point_in_circle(0, 0, 1, x, y) ? "Point in circle" : "Point outside the circle");
            Console.WriteLine("\n\n\n");
        }

        public static double CalcExpDouble(double a, double b)
        {
            double numr = Math.Pow(a - b, 3.0) - Math.Pow(a, 3.0);
            double denm = 3.0 * a * Math.Pow(b, 2.0) - 3.0 * Math.Pow(a, 2.0) * b;
            return numr / denm;
        }

        public static float CalcExpFloat(float a, float b)
        {
            float numr = (float)Math.Pow(a - b, 3.0) - (float)Math.Pow(a, 3.0);
            float denm = 3.0f * a * (float)Math.Pow(b, 2.0) - 3.0f * (float)Math.Pow(a, 2.0) * b;
            return numr / denm;
        }

        public static void Task3()
        {
            float expResult1 = CalcExpFloat(1000.0f, 0.0001f);
            double expResult2 = CalcExpDouble(1000.0, 0.0001);
            Console.WriteLine("float: " + expResult1 + " double: " + expResult2);
            Console.WriteLine("\n\n\n");
        }

        static void Main(string[] args)
        {
            Task1();
            Task2();
            Task3();
        }
    }
}