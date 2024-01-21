using System;
using System.Collections;

namespace lab12i
{
    public interface IRandomInit: IComparable, IComparer, ICloneable
    {
        int random { get; }
        void RandomInit();
    }
}
