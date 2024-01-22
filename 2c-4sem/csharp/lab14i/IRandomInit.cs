using System;
using System.Collections;

namespace lab14i
{
    public interface IRandomInit: IComparable, IComparer, ICloneable
    {
        int random { get; }
        void RandomInit();
    }
}
