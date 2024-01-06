using System;
using System.Security.Cryptography;

namespace Lab10_1
{
    public class Republic: State
    {
        public Republic(string name) : base(name, "Республика"){}

        public override void SetGovernmentForm(string governmentForm)
        {
            return;
        }
    }
}
