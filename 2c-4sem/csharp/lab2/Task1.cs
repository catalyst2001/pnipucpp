using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab2
{
    public static class Task1
    {
        public static int SumOdd()
        {        
            int result = 0, number = 0;
            Console.Write("n=");
            int n = Convert.ToInt32(Console.ReadLine());
            for (int i = 0; i < n; i++)
            {
                Console.Write("{1} number:", i + 1);
                number = Convert.ToInt32(Console.ReadLine());
                if ((number % 2) != 0)
                    result += number;
            }
            return result;
        }
    }
}
