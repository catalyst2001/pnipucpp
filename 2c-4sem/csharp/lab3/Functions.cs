using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab3
{
    public static class Functions
    {
        public static double ComputeY(double x)
        {
            return x * Math.Atan(x) - Math.Log(Math.Sqrt(1 + Math.Pow(x, 2)));
        }

        public static double ComputeSN(double x, double n)
        {
            double sum = 0.0;
            for (double i = 1; i <= n; i++)
            {
                sum += Math.Pow(-1.0, i + 1.0) * ((Math.Pow(x, 2.0 * i)) / (2.0 * i * (2.0 * i - 1.0)));
            }
            return sum;
        }

        public static double ComputeSE(double x, double e)
        {
            double precision = double.MaxValue;
            double sum = 0;
            double sumLast = 0;
            double n = 1d;
            while (Math.Abs(precision) > e)
            {
                sum += Math.Pow(-1.0, n + 1.0) * ((Math.Pow(x, 2.0 * n)) / (2.0 * n * (2.0 * n - 1.0)));
                precision = sum - sumLast;
                sumLast = sum;
                n++;
            }
            return sum;
        }
    }
}
