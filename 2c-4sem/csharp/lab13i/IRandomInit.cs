using System;
using System.Collections;

namespace lab13i
{
    public interface IRandomInit: IComparable, IComparer, ICloneable
    {
        int random { get; }
        void RandomInit();
    }
}
