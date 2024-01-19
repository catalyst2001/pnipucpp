using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab11i
{
    public class ConsoleRead
    {
        private static void printException(FormatException e)
        {
            Console.WriteLine("Format exception: " + e.Message + " Please retype data");
        }

        public static int readInt32()
        {
            while (true)
            {
                try
                {
                    return Convert.ToInt32(Console.ReadLine());
                }
                catch (FormatException e)
                {
                    printException(e);
                }
            }
        }
    }
}
