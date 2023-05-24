using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _10_3
{
    internal class RandomImplement : IRandomInit
    {
        public int value { get; set; }

        public void RandomInit()
        {
            value = new Random().Next(100);
        }
    }
}
