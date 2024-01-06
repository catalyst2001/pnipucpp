using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab10_1
{
    public class SimpleRandom : IRandomInit
    {
        private int _random;
        int IRandomInit.random => _random;

        public SimpleRandom()
        {
            _random = new Random().Next(0, 100);
        }

        public object Clone()
        {
            return new SimpleRandom();
        }

        public int Compare(object? x, object? y)
        {
            if (((IRandomInit)x).random == ((IRandomInit)y).random)
                return 0;
            if (((IRandomInit)x).random > ((IRandomInit)y).random)
                return 1;
            return -1;
        }

        public int CompareTo(object? obj)
        {
            if (((IRandomInit)obj).random == _random)
                return 0;
            if (((IRandomInit)obj).random > _random)
                return 1;
            return -1;
        }

        public void RandomInit()
        {
            Console.WriteLine(_random);
        }
    }
}
