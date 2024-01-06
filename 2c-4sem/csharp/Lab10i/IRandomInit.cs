using System;
using System.Collections;

namespace Lab10_1
{
    public interface IRandomInit: IComparable, IComparer, ICloneable
    {
        int random { get; }
        void RandomInit();
    }
}
