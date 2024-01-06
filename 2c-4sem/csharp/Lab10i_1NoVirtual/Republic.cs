using System;
using System.Security.Cryptography;

namespace Lab10_1NoVirtual
{
    public class Republic: State
    {
        public Republic(string name) : base(name, "Республика"){}

        public void SetGovernmentForm(string governmentForm)
        {
            return;
        }
    }
}
