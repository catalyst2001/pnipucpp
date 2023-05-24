using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Dynamic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab7
{
    public class Triangle
    {
        public double a, b, c;

        public double GetHalfPerimeter() { return (a + b + c) / 2.0; }

        public bool IsValid() { return a + b > c && a + c > b && b + c > a; }

        public double GetSquare()
        {
            double p = GetHalfPerimeter();
            return Math.Sqrt(p * (p - a) * (p - b) * (p - c));
        }

        public static Triangle operator++(Triangle a)
        {
            return new Triangle {
                a = a.a + 1.0,
                b = a.b + 1.0,
                c = a.c + 1.0
            };
        }

        public static Triangle operator--(Triangle a)
        {
            return new Triangle
            {
                a = a.a - 1.0,
                b = a.b - 1.0,
                c = a.c - 1.0
            };
        }

        // явн
        public static explicit operator double(Triangle a)
        {
            if (!a.IsValid())
                return double.MinValue;

            return a.GetSquare();
        }

        public static implicit operator bool(Triangle a)
        {
            return a.IsValid();
        }

        public static bool operator<=(Triangle a, Triangle b) { return a.GetSquare() <= b.GetSquare(); }
        public static bool operator>=(Triangle a, Triangle b) { return a.GetSquare() >= b.GetSquare(); }
    }
}
