using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab2
{
    public static class Task2
    {
        public static int MultipleK(int k)
        {
            int result = 0, number;
            while((number = Convert.ToInt32(Console.ReadLine())) != 0)
                if((number % k) == 0)
                    result++;

            return result;
        }
    }
}
