using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters;
using System.Text;
using System.Threading.Tasks;

// 38
namespace lab2
{
    public static class Task3
    {
        static public int SumNumbers(int n)
        {
            int sum = 0;
            int s = 15;
            for (int i = 1; i <= n; i++, s += 2)
            {
                int curr_num = s * ((i % 3) == 0 ? -1 : 1);
                Console.Write(curr_num + " ");
                sum += curr_num;
            }
            return sum;
        }


    }
}
